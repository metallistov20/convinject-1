/*
 (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "xmparser.h"
#include "structs.h"


/* Ptr to XML data for in memory */
xmlNode *root_element = NULL;

/* Ptr to XML data document */
xmlDoc *doc = NULL;

/* Static array in scope of curr. file. We store aux strings StrTokIdx() inside  */
static char * aLastToken[3];

/* Treansform string to tocken; designed for serial recursive calling */
static char * strTokIdx(char *s1, const char *delimit, int iIdx)
{
char ** lastToken;

char *tmp;

	lastToken =  &(aLastToken[iIdx]) ;

	/* TIP: can't initialize (*lastToken = NULL;) here */

	/* Skip leading delimiters if new string. */
	if ( s1 == NULL )
	{
		s1 = *lastToken;

		if (s1 == NULL)

			return NULL;
	}
	else
		s1 += strspn(s1, delimit);


	/* Find end of segment */
	tmp = strpbrk(s1, delimit);

	if (tmp)
	{
		/* Found another delimiter, split string and save state. */
		*tmp = '\0';

		*lastToken = tmp + 1;
	}
	else
	{
		/* Last segment, remember that. */
		*lastToken = NULL;
	}

	return s1;
}

/* Function removes all <tab> withing single string of XML block; Once removed, extracts
   value out of "URL=value" string. Then passes thins string to <_unat>.
   TODO: add processing of 0-byte long tokens, which is needed to process URLs like "http://xxx.xxx.xxx.xxx",
   i.e. those without any payload and backslash on the end. */
static void _untab(char * tkn)
{
char *_localCopy;

char *_localToken;

char *cParcedOut;

	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strTokIdx(_localCopy, "\t", 1);

	while( _localToken != NULL )
	{
/*		if (strlen (_localToken) <= ( strlen("URL=\"") + strlen("\"") ) )

			break;
		else*/
		{
			cParcedOut = strndup(_localToken/*+ strlen("URL=\"")*/, strlen(_localToken) /*-strlen("URL=\"")*/ );

			cParcedOut[strlen(cParcedOut) -1] = 0;
#if 0
			if (INJ_SUCCESS != AppendUrl(&pUrlChain, "(aux;dta;)") )
			{
				DXMLAUX("%s: FAILURE: memory error on appenging new URL\n", "");

				_localToken = NULL;

				free (cParcedOut);

				/* exiting from while-loop */
				break;

			}

			_unat(cParcedOut);

#else
	printf("[%s]: .=%s \n", "caller"/*TODO*/, cParcedOut );
#endif /* (0) */



			_localToken = strTokIdx(NULL, "\t", 1);

			free (cParcedOut);
		}
	}

	free(_localCopy);
}

/* Function removes all <newline> withing entire XML block; once removed - passes string for furtherprocessing to <_untab> */
static void _unret(char * tkn)
{
char *_localCopy;

char *_localToken;

	_localCopy=strndup(tkn, strlen(tkn));

	_localToken=strTokIdx(_localCopy, "\n", 0);

	while( _localToken != NULL )
	{
		_untab(_localToken);

		_localToken = strTokIdx(NULL, "\n", 0);
	}


	free(_localCopy);
}

void extract_xml_values(xmlNode * a_node)
{
xmlNode *cur_node = a_node;

	if (XML_ELEMENT_NODE == cur_node->type)
	{
		/* Element has been found by template?  */
		if (        					 ( 0 == strcmp ("Type", cur_node->name) )  ||
		 ( 0 == strcmp ("Name", cur_node->name) )  ||  ( 0 == strcmp ("Address", cur_node->name) )  || 
		 ( 0 == strcmp ("Login", cur_node->name) )  ||   ( 0 == strcmp ("Passwd", cur_node->name) )  || 
		 ( 0 == strcmp ("Datafile", cur_node->name) )  ||  ( 0 == strcmp ("Proto", cur_node->name) )  	)
		{
			xmlNode *_ch_cur_node = NULL;

			printf("field <%s> found, ", cur_node->name);

			/* Go and parce it */
			for (_ch_cur_node = cur_node->children; _ch_cur_node; _ch_cur_node = _ch_cur_node->next)
			{
				if ( XML_TEXT_NODE == _ch_cur_node->type)
				{
					printf("its content is <%s>.\n", _ch_cur_node->content);
				}

				/* Get out form this <for> */
				break;
			}


		}
	}
}

/* Parse XML structure */
void _parse_xml(/* const char * caller, */xmlNode * a_node, const char * template)
{
xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if (XML_ELEMENT_NODE == cur_node->type)
		{
			/* Element has been found by template?  */
			if ( 0 == strcmp (template, cur_node->name) )
			{
				xmlNode *_ch_cur_node = NULL;

				DXMLAUX("<%s> found \n", cur_node->name);

				/* Go and parce it, store results of paring into URL&CMPND structures  */
				for (_ch_cur_node = cur_node->children; _ch_cur_node; _ch_cur_node = _ch_cur_node->next)
				{
					//.DXMLAUX("<%s, %s> \n", _ch_cur_node->name , _ch_cur_node->content);

					extract_xml_values(_ch_cur_node);

				} /* for() */
printf("\n");//TODO
			}		
		}
		_parse_xml(cur_node->children, template);
	}
}

char cXmlName[255];//TODO


int main (int argc, char **argv)
{
	/* Assign datafile name*/
	strcpy (cXmlName, argv[1]);

printf("main: %s\n", cXmlName);

	doc = xmlReadFile(cXmlName, NULL, 0);

	if (NULL == doc)
	{
		DCOMMON("ERROR: could not parse file %s\n", cXmlName);

		return INJ_XML_ERROR;
	}

	/* Get the root node of the XML data stored in the <doc> */
	root_element = xmlDocGetRootElement(doc);


	/* Put XML section <TL-SL5428E> into structure <pUrlChain> */
	_parse_xml(root_element, "Target");

#if 0
	/* Glue particles of <pUrlChain> into full-blown URLs */
	GlueUrl(pUrlChain);

#if (DEBUG_URL)
	DisplayUrl(pUrlChain);
#endif /* (DEBUG_URL) */

	/* Put URLs into wire */
	return DeployUrlEx(pUrlChain, 1);
#else
	return 0;
#endif /* (0) */
}

