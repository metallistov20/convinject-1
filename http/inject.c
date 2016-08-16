/*
 (C) Copyright 2015, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT any WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

#include <string.h>
#include <getopt.h>

#include <curl/curl.h>

#include <libxml/parser.h>
#include <libxml/tree.h>


#include "verbose.h"
#include "opcodes.h"
#include "constants.h"
#include "lists.h"
#include "xmls.h"
#include "auxiliary.h"



/* Index of desired operation (can be 'create', 'save', 'ACL', 'firmware' )*/
int iOperation;

/* Intermediate URL structure */
CURL *curl;

int input_pipe[2];//TODO: del
#if (0)
/* Payload of POST method during user authenticate */
const char *cPostMethodString="username=admin&password=admin&logon=Login";

/* Extra payload of POST method during user  authenticate */
const char *cPostMethodString2="user=admin&level=1&userID=0";
#else
/* Payload of POST method during user authenticate. Initializaed in <process_http_target>. */
char * cPostMethodString;

/* Extra payload of POST method during user authenticate. Initializaed in <process_http_target>. */
char * cPostMethodString2;
#endif /* (0) */

char cArg0[MAX_URL_SIZE];

/* pointers onto cPostMethodString, cPostMethodString2 */
char ** pcPtr2Extra1, **pcPtr2Extra2;

/* Extra payload should be attached  */
int iExtra;


/* Ptr to XML data for in memory */
xmlNode *root_element = NULL;

/* Ptr to XML data document */
xmlDoc *doc = NULL;

#define M_PATH	260
/* Single command buffer */
char cCmdDataBuf[M_PATH];// TODO: 2 DEL

/* Data structure type definition */
#include "../ssh/cmds.h"





/* Pointer to a dynamic structure to store command tray */
pCmdType pHttpCmdChain;



static int process_datafile(char * pcFilename)
{
FILE* fp = NULL;

	if(NULL == pcFilename)
	{
		printf("ERROR: assign_host_file - empty filename\n");
		return (-8);//TODO;
	}

	/* Try to open file with commands  */
	if ( NULL == (fp = fopen (pcFilename, "r") ) )
	{
		printf("[%s] %s: can't open file <%s> \n", __FILE__, __func__ , pcFilename);

		return (-9);//TODO;
	}

	/* For each string of Raw Data file */
	while ( ! (feof (fp) ) ) 
	{
		/* Scan whole string into temp. buffer */
		if (NULL == fgets (cCmdDataBuf, M_PATH, fp) )
		{
			/* no string read from data file */
		}
		else
		{
			printf("[%s] %s: scanned:%s", __FILE__, __func__, cCmdDataBuf);

			/* Attach just scanned data */
			EnrollCmd(&pHttpCmdChain, cCmdDataBuf);
		}
	}


	/* Close file, and dispose pointer to Raw Data file */
	fclose(fp);

	return 0;
}


int m_Tocken;

static int m_TockenFound;

static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size)
{
size_t i;
size_t c;
unsigned int width=0x10;
int iTocken=0;
char cBuf[512];
char * cpAjaxPtr;
char * cp1, * cp2;

	fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n", text, (long)size, (long)size);

	if ( NULL != ( cpAjaxPtr = strstr(ptr, "var STR_AJAX_VALUE") ) )
	{
		cp1 = strtok(cpAjaxPtr, "\"");
		cp2 = strtok(NULL, "\"");
		m_TockenFound = 1;
		m_Tocken = atoi (cp2);

		printf("[%s] iTocken = <%s> m_Tocken = <%d>. SUCCESS. TERMINATING. <m_TockenFound=%d>\n", cpAjaxPtr, cp2, m_Tocken,
		m_TockenFound);
	}

	for(i=0; i<size; i+= width)
	{
		fprintf(stream, "%4.4lx: ", (long)i);
		/* show hex to the left */

		for(c = 0; c < width; c++)
		{
			if(i+c < size)
				fprintf(stream, "%02x ", ptr[i+c]);
			else
				fputs(" ", stream);
		}

		/* show data on the right */
		for(c = 0; (c < width) && (i+c < size); c++)
			fputc((ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.', stream);

		fputc('\n', stream); /* newline */
	}
}


static int m_trace(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
const char *text;
(void)handle;

	switch (type)
	{
		case CURLINFO_TEXT:
		//fprintf(stderr, "== Info: %s", data);
		fprintf(stdout, "== Info: %s", data);
		default: /* in case a new one is introduced to shock us */
		return 0;

		case CURLINFO_HEADER_OUT:
		text = "=> Send header";
		break;

		case CURLINFO_DATA_OUT:
		text = "=> Send data";
		break;

		case CURLINFO_SSL_DATA_OUT:
		text = "=> Send SSL data";
		break;

		case CURLINFO_HEADER_IN:
		text = "<= Recv header";
		break;

		case CURLINFO_DATA_IN:
		text = "<= Recv data";
		break;

		case CURLINFO_SSL_DATA_IN:
		text = "<= Recv SSL data";
		break;
	}

	dump(text, stdout, (unsigned char *)data, size);
	return 0;
} /* int my_trace */


/* POST'ing a XML payload over here we put the PIN code into a modem */
//const char * OPEN_PAGE = "http://192.168.1.1/api/pin/operate";
/* On thie page we reconnect reconnect */
//const char * DIAL_PAGE = "http://192.168.1.1/api/dialup/dial";
typedef struct _RespStruct
{
	char * cpResponce;
	size_t iSize;
} RespStruct, *pRespStruct;

/* Suppose 32K is enough to include _any HTML responce frmo _this LTE-modem */
char cBuffer[0x400*32];


/* Callback to execute on arrival of HTML responce */
static size_t RecvClbk(void *contents, size_t size, size_t nmemb, void *userp)
{
size_t realsize = size * nmemb;

	RespStruct * HtmlRespStruct = (RespStruct *)userp;
	HtmlRespStruct->cpResponce = (char*)(cBuffer);
	memcpy(HtmlRespStruct->cpResponce, contents, realsize);
	HtmlRespStruct->cpResponce[realsize] = 0;
	HtmlRespStruct->iSize = realsize;
	return realsize;
} /* size_t RecvClbk */


/* Not implemented by now */
int process_http_target(char * pcAddress, char * pcLogin, char * pcPasswd, char * pcDatafile)
{
int iRes;
RespStruct RespStr;


	/* Payload of POST method during user authenticate. */
	cPostMethodString = (char *) malloc (MAX_URL_SIZE);

	/* Extra payload of POST method during user authenticate. */
	cPostMethodString2 = (char *) malloc (MAX_URL_SIZE);


	sprintf(cPostMethodString, "username=%s&password=%s&logon=Login", pcLogin, pcPasswd);

	sprintf(cPostMethodString2,"user=%s&level=1&userID=0", pcLogin);

	strcpy(cIpAddr, pcAddress);//TODO: rework ? 


	/* TODO: Maybe to use <cPostMethodString, ..> in lists.c::_DeployUrlEx directly ? */
	pcPtr2Extra1 = (char**)&cPostMethodString;
	pcPtr2Extra2 = (char**)&cPostMethodString2;


	process_datafile(pcDatafile);


//++++++++++++++++++++++++++
	
	/* Ensure that file <cast.XXXXX.txt.xml>. Generated by <make_xml.sh> */
	if ( NULL == cXmlName )
	{
		HCOMMON("%s: ERROR: name of XML file to parse should be passed on command line\n", cArg0);

		return -9;//TODO: rem.
	}

	/* To this moment <voc.c> and <voc.h> are nearby, otherwise <autogen.cmd+make> was failed */
	if (INJ_SUCCESS != XmlAuxCreateEx() ) 
	{
		HCOMMON("%s: ERROR: no rules to handle (%s) learned\n", cArg0, cXmlName);

		return -9;//TODO: rem.
	}

	/* Check potential ABI mismatches between the version it was compiled for and the actual shared library used */
	LIBXML_TEST_VERSION
	// TODO: may we skip it ?

	/* Get the contents of <cast.XXXXX.txt.xml> into <doc> */
	doc = xmlReadFile(pcDatafile, NULL, 0);//TODO: put if-else construction here

	if (NULL == doc)
	{
		HCOMMON("%s: ERROR: could not parse file %s\n", cArg0, cXmlName);

		return -9;//TODO: rem.
	}

	/* Get the root node of the XML data stored in the <doc> */
	root_element = xmlDocGetRootElement(doc);//TODO: put if-else construction here


	if(NULL == ( curl = curl_easy_init() ) )
	{
		printf("Can't initialize lib c-url \nERROR\n");// TODO: repl.

		return (-8);// TODO: repl.
	}

	if(curl)
	{
		iRes = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, m_trace);

		/* the DEBUGFUNCTION has no effect until we enable VERBOSE */
		iRes = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		/* we tell libcurl to follow redirection */
		iRes = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//-10..curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");

		/* send all data to this function */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RecvClbk);
		/* we pass our 'RespStr' struct to the callback function */

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&RespStr);
		//-10..curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		/* Clean the buffer before receiving a responce into it */
		memset (&RespStr, sizeof (struct _RespStruct) , 0);
		
		/* Findf here a tocken */
		VERBOSE_STATUS(iOpenSite)

		/* Close URL lib */
		curl_easy_cleanup(curl);

	}


	/* Delete entire list with URLs along with its compounds */
	DeleteUrlEx(&pUrlChain);

	/* Delete vocabuilary, et al*/
	DeleteXmlAuxEx(&pAuxiliary);

	/* Free the document */
	xmlFreeDoc(doc);

	/* Free the global variables that may have been allocated by the parser */
	xmlCleanupParser();


	free(cPostMethodString);

	free(cPostMethodString2);


 	return 0;//TODO: repl.
}


