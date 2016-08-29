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


/* Needed to run iOpenSite, which gives out session ID <g_tID> */
int iOperation;

/* Intermediate URL structure */
CURL *curl;


/* Payload of POST method during user authenticate. Initializaed in <process_http_target>. */
char * cPostMethodString;

/* Extra payload of POST method during user authenticate. Initializaed in <process_http_target>. */
char * cPostMethodString2;


/* pointers onto cPostMethodString, cPostMethodString2 */
char ** pcPtr2Extra1, **pcPtr2Extra2;

/* Extra payload should be attached  */
int iExtra;


/* Ptr to XML data for in memory */
xmlNode *root_element = NULL;

/* Ptr to XML data document */
xmlDoc *doc = NULL;

/* Single command buffer */
char cCmdDataBuf[HTTP_MAX_PATH];

/* Data structure type definition */
#include "../ssh/cmds.h"


int i5428E = 1;//TODO: REMOVE! (FOR DEBUG TIME ONLY!) 

static volatile int m_TockenFound;//TODO: REMOVE! (FOR DEBUG TIME ONLY!) 



typedef struct _tf {
	char * 	pcName;
	int 	iOp;
	int 	iAmt;

	char * pcVar1Name;
	char * pcVar1;
	char * pcVar2Name;
	char * pcVar2;
} tf;

tf tfOpTable[] =
{
	{"id", 		DO_NO_OP,	2, _tid_, NULL, NULL, NULL},
//TODO: 	{"target", 	DO_NO_OP,	2, cIpAddr, NULL, NULL, NULL},

	/* Singletons: operation names */
	{"open\n",	DO_OPEN_OP,	1, NULL, NULL, NULL, NULL},
	{"close\n",	DO_CLOSE_OP,	1, NULL, NULL, NULL, NULL},

	{"create",	DO_CREATE_OP,	2, "community", txt_comname, NULL, NULL},
	{"save", 	DO_SAVE_OP,	1, NULL, NULL, NULL, NULL},

	{"ACL\n",	DO_ACL_OP,	2, "acl-data", aclId, NULL, NULL},
	{"upgrade\n",	DO_FIRMWARE_OP,	1, NULL, NULL, NULL, NULL},
	{"reboot\n",	DO_REBOOT_OP,	1, NULL, NULL, NULL, NULL},
	{"ipassign\n",	DO_IPSET_OP,	3, "ip-addr", ip_address, "ip-mask", ip_mask },

	{"ipV6assign\n",DO_IPV6SET_OP,	1, NULL, NULL, NULL, NULL},
	{"acontrol\n",	DO_ACNTL_OP,	1, NULL, NULL, NULL, NULL},
	{"psecure\n",	DO_PSEC_OP,	1, NULL, NULL, NULL, NULL},
	{"pmirror\n",	DO_PMIR_OP,	1, NULL, NULL, NULL, NULL},
	{"vlancrt\n",	DO_VLANCR_OP,	1, NULL, NULL, NULL, NULL},
	{"iprange\n",	DO_IPRAN_OP,	1, NULL, NULL, NULL, NULL},
	{"pfilter\n",	DO_PFILT_OP,	1, NULL, NULL, NULL, NULL},
	{"ping\n",	DO_PING_OP,	1, NULL, NULL, NULL, NULL},
	{"tracert\n",	DO_TRACERT_OP,	1, NULL, NULL, NULL, NULL},
	{"cable\n",	DO_CABLE_OP,	1, NULL, NULL, NULL, NULL},
	{"looback\n",	DO_LOOPBK_OP,	1, NULL, NULL, NULL, NULL},

	/* Couples: names of variables and their values */
	{"filename",	DO_NO_OP,	3, cFwName, NULL, NULL, NULL},
	{"xml-data",	DO_NO_OP,	2, cXmlName, NULL, NULL, NULL},

	{"t_mode", 	DO_NO_OP,	2, t_mode, NULL, NULL, NULL},
	{"t_key", 	DO_NO_OP,	2, t_key, NULL, NULL, NULL},
	{"t_stat", 	DO_NO_OP,	2, t_stat, NULL, NULL, NULL},
	{"chk_", 	DO_NO_OP,	2, chk_, NULL, NULL, NULL},
	{"cb_", 	DO_NO_OP,	2, cb_, NULL, NULL, NULL},
	{"list_", 	DO_NO_OP,	2, list_, NULL, NULL, NULL},
	{"member_", 	DO_NO_OP,	2, member_, NULL, NULL, NULL},
	{"proof", 	DO_NO_OP,	2, proof, NULL, NULL, NULL},

	{NULL, 0, 0, 			NULL, NULL, NULL, NULL }
};



/* Pointer to a dynamic structure to store command tray */
pCmdType pHttpCmdChain;


int iParseCmdArgs(char ** ppcArgVect)
{
int iOption;

	/* Index of the option */
	int iOptIndex = 0;

	/* Must have each time we start a new URL */
	CleanQuine("CleanQuine");

	/* Assuming <tfOpTable [iOptIndex]> is not null. We can assume it. */
	while ( NULL != tfOpTable [iOptIndex].pcName )
	{
		iOperation = -1;

		/* Singletons */
		if (1 == tfOpTable [iOptIndex].iAmt) 
		{
			if ( 0 == strcmp(tfOpTable[iOptIndex].pcName, ppcArgVect[0]) )
			{

				iOperation= tfOpTable[iOptIndex].iOp;

				break;
			}

		}
		/* Trinities, usw */
		else
			if (3 == tfOpTable [iOptIndex].iAmt)
			{
			char Ctmp[HTTP_MAX_PATH*2];

				/* TODO: *(char**)(tfOpTable[iOptIndex].pcVar1) = *(char**)(tfOpTable[iOptIndex].pcVar2) = NULL; */
				
				sprintf((char * __restrict__)&Ctmp, "%s %s=%%s %s=%%s", tfOpTable[iOptIndex].pcName, 
					tfOpTable[iOptIndex].pcVar1Name, 
						tfOpTable[iOptIndex].pcVar2Name );

				if ( 2 == sscanf (ppcArgVect[0], Ctmp, tfOpTable[iOptIndex].pcVar1, tfOpTable[iOptIndex].pcVar2 )  )
				{
					iOperation= tfOpTable[iOptIndex].iOp;

					break;
				}
			}
			else
				/* Couples */
				if (2 == tfOpTable [iOptIndex].iAmt)
				{
				char Ctmp[HTTP_MAX_PATH];

					/* TODO: *(char**)(tfOpTable[iOptIndex].pcVar1) = NULL; */

					sprintf((char * __restrict__)&Ctmp, "%s %s=%%s", tfOpTable[iOptIndex].pcName, 
						tfOpTable[iOptIndex].pcVar1Name);

					if ( 1 == sscanf (ppcArgVect[0], Ctmp, tfOpTable[iOptIndex].pcVar1 )  )
					{
						iOperation= tfOpTable[iOptIndex].iOp;

						break;
					}
				}

		iOptIndex++;

	} /* while(..) */

	/* Ensure that file <cast.XXXXX.txt.xml>. Generated by <make_xml.sh> */
	if ( NULL == cXmlName )
	{
		HCOMMON("[%s] %s: ERROR: name of XML file to parse should be passed on command line\n", __FILE__, __func__ );

		return HTTP_WRONG_NAME;
	}

	/* To this moment <voc.c> and <voc.h> are nearby, otherwise <autogen.cmd+make> was failed */
	if (HTTP_SUCCESS != XmlAuxCreateEx() ) 
	{
		HCOMMON("[%s] %s: ERROR: no rules to handle (%s) learned\n", __FILE__, __func__, cXmlName);

		return HTTP_BAD_VOC;
	}

	/* Skippable, we've checked ABI compatibility in <main.c::main()> */
	LIBXML_TEST_VERSION

	char pcCastFile[HTTP_MAX_PATH];


	/* Get the contents of <cast.XXXXX.txt.xml> into <doc> */	/* 5428E */
	sprintf(pcCastFile, "./cast.%s.xml", "5428E");//TODO: remove hardcoded stuff


	doc = xmlReadFile(pcCastFile, NULL, 0);

	if (NULL == doc)
	{
		HCOMMON("[%s] %s: ERROR: could not parse file %s\n", __FILE__, __func__, pcCastFile);

		return HTTP_BAD_DATAFILE;
	}

	/* Get the root node of the XML data stored in the <doc> */
	if ( NULL == ( root_element = xmlDocGetRootElement(doc) ) )
	{
		HCOMMON("[%s] %s: badly organized XML doc in file %s\n", __FILE__, __func__, pcCastFile);

		return HTTP_BAD_XML_FORMAT;
	}

	// TODO: reowrk, especially this <i5428E>
	if ((i5428E) && (0 == m_TockenFound) )
//	if (0 == _tid_[0] )
	{
		/* Fulfill <_tid_> */
//		VERBOSE_STATUS(iOpenSite)

		HCOMMON("[%s] %s: got such <_tid_=%s>\n", __FILE__, __func__, _tid_);
	}

	/* At this time point we assume all parameters parsed OK, so let's call inj. primitives */
	switch (iOperation)
	{
		case DO_OPEN_OP:
			VERBOSE_STATUS(iOpenSite)
			break;

		case DO_CLOSE_OP:
			VERBOSE_STATUS(iCloseSite)
			break;

		case DO_CREATE_OP:
			VERBOSE_STATUS(iCreateSnmp)
			break;

		case DO_SAVE_OP:
			VERBOSE_STATUS(iSaveSite)
			break;

		case DO_ACL_OP:
			VERBOSE_STATUS(iAclGroup)
			break;

		case DO_FIRMWARE_OP:
			VERBOSE_STATUS(iUpgradeFirmware)
			break;

		case DO_REBOOT_OP:
			VERBOSE_STATUS(iRebootSwitch)
			break;

		case DO_IPSET_OP:
			VERBOSE_STATUS(iAssignIp)
			break;

		/* TODO: this is a stub only; no real implementation */
		case DO_IPV6SET_OP:
			VERBOSE_STATUS2(iMeta2, "idle1", "idle2")
			break;

		/* TODO: these are stubs only; no real implementation */
		case DO_ACNTL_OP:
		case DO_PSEC_OP:
		case DO_PMIR_OP:
		case DO_VLANCR_OP:
		case DO_VLANC_OP:
		case DO_IPRAN_OP:
		case DO_PFILT_OP:
		case DO_PING_OP:
		case DO_TRACERT_OP:
		case DO_CABLE_OP:
		case DO_LOOPBK_OP:
			VERBOSE_STATUS1(iMeta1, "idle")
			break;

		case DO_NO_OP:
		default:
			printf("[%s] %s: there's no operation with OPCODE=%d, exiting\n", __FILE__, __func__, iOperation);
			break;
		
	}

	/* Delete entire list with URLs along with its compounds */
	DeleteUrlEx(&pUrlChain);

	/* Delete vocabuilary, et al*/
	DeleteXmlAuxEx(&pAuxiliary);

	/* Free the document */
	xmlFreeDoc(doc);

	/* Free the global variables that may have been allocated by the parser */
	xmlCleanupParser();

}





static int process_datafile(char * pcFilename)
{
FILE* fp = NULL;

	if(NULL == pcFilename)
	{
		HCOMMON("[%s] %s: ERROR: assign_host_file - empty filename\n", __FILE__, __func__ );

		return HTTP_BAD_FNAME;
	}

	/* Try to open file with commands  */
	if ( NULL == (fp = fopen (pcFilename, "r") ) )
	{
		HCOMMON("[%s] %s: can't open file <%s> \n", __FILE__, __func__ , pcFilename);

		return HTTP_BAD_FOPEN;
	}

	/* For each string of Raw Data file */
	while ( ! (feof (fp) ) ) 
	{
		/* Scan whole string into temp. buffer */
		if (NULL == fgets (cCmdDataBuf, HTTP_MAX_PATH, fp) )
		{
			/* End of file reached, most likely it's not an error */
			HCOMMON("[%s] %s: End of file reached\n", __FILE__, __func__);
		}
		else
		{
			/* As the scanned strings contains <CR> already we don't put our <\n> into format expresion */
			HCOMMON("[%s] %s: scanned:%s", __FILE__, __func__, cCmdDataBuf);

			/* Attach just scanned data */
			EnrollCmd(&pHttpCmdChain, cCmdDataBuf);
		}
	}


	/* Close file, and dispose pointer to Raw Data file */
	fclose(fp);

	//return 0;
	return HTTP_SUCCESS;
}


int m_Tocken;

static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size)
{
size_t i;
size_t c;
unsigned int width=0x10;

//TODO: explain such a choice 
char cBuf[8*512*8];

char * cpTockenPtr;
char * cp1, * cp2;

//printf(ptr);
//return 0;

	/* We already have <_tid_> */
	if (m_TockenFound) return;

	fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n", text, (long)size, (long)size);

	if ( NULL != ( cpTockenPtr = strstr(ptr, "var g_tid =") ) )
	{
		cp1 = strtok(cpTockenPtr, "\'");
		cp2 = strtok(NULL, "\'");		

		m_TockenFound = 1;

		printf("[%s] iTocken = <%s> SUCCESS. TERMINATING. <m_TockenFound=%d>\n", cpTockenPtr,  cp2 , m_TockenFound );

/*		strcpy((char*)&_tid_, cp2);

		printf("  _tid_ = <%s> \n", (char*)&_tid_);*/

		memcpy(&_tid_[0], cp2, strlen (cp2) + 1 );

		printf("  _tid_ = <%s> \n", &_tid_[0]);

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
		fprintf(stdout, "== Info: %s", data);
		default:
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


int ProcessHttpCmds(pCmdType pPointChainPar)
{
pCmdType pPointChain = pPointChainPar;

	/* Process each entry of chain */
	while (NULL != pPointChain)
	{	
//-------------------1
		if(NULL == ( curl = curl_easy_init() ) )
		{
			HCOMMON("[%s] %s: Can't initialize lib c-url \nERROR\n", __FILE__, __func__);

			return HTTP_BAD_LIBINIT;
		}

		if(curl)
		{
		int iRes;
		RespStruct RespStr;
		

			/* See comment below */
			iRes = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, m_trace);

			/* The DEBUGFUNCTION has no effect until we enable VERBOSE */
			iRes = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

			/* We tell libcurl to follow redirection */
			iRes = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");

			/* Send all data to this function */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RecvClbk);

			/* We pass our 'RespStr' struct to the callback function */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&RespStr);

			/* Clean the buffer before receiving a responce into it */
			memset (&RespStr, sizeof (struct _RespStruct) , 0);
//-------------------1
	
#if defined(_DBG)
			printf ("PRINT OUT<pPointChainPar=%p>:%s\n", pPointChain, pPointChain->pcCmd);
#endif /* 0 */
		
			ProcessSingleHttpCmd(pPointChain);

//-------------------2
			/* Close URL lib */
			curl_easy_cleanup(curl);
//-------------------2
		}

		/* Go to next record of chain */
		pPointChain = pPointChain->pNext;

	}

	return 0;//TODO: replace 

} /* int _ProcessHttpCmds */



/* Enter WEB-server with login/passwd <pcLogin/pcPasswd> and execute commands from <pcDataFile> on it */
int process_http_target(char * pcAddress, char * pcLogin, char * pcPasswd, char * pcDataFile)
{


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


	/* Fulfill records in <pHttpCmdChain> list */
	if (HTTP_SUCCESS != process_datafile(pcDataFile) )
	{
		HCOMMON("[%s] %s: Can't process datafile <%s> \nERROR\n", __FILE__, __func__, pcDataFile);

		return HTTP_BAD_DATA;
	}

#if (0)
//TODO: Should initialize every time we do a HTTP command

	if(NULL == ( curl = curl_easy_init() ) )
	{
		HCOMMON("[%s] %s: Can't initialize lib c-url \nERROR\n", __FILE__, __func__);

		return HTTP_BAD_LIBINIT;
	}

	if(curl)
	{
		/* See comment below */
		iRes = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, m_trace);

		/* The DEBUGFUNCTION has no effect until we enable VERBOSE */
		iRes = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		/* We tell libcurl to follow redirection */
		iRes = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");

		/* Send all data to this function */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RecvClbk);

		/* We pass our 'RespStr' struct to the callback function */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&RespStr);

		/* Clean the buffer before receiving a responce into it */
		memset (&RespStr, sizeof (struct _RespStruct) , 0);
#endif /* (0) */
		
		m_TockenFound = 0;//TODO: what's this?

		ProcessHttpCmds(pHttpCmdChain);

#if (0)
//TODO: Should uninitialize every time we do a HTTP command
		/* Close URL lib */
		curl_easy_cleanup(curl);


	}
#endif /* (0) */


	free(cPostMethodString);

	free(cPostMethodString2);


	return HTTP_SUCCESS;
}


