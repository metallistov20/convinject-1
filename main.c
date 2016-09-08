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



/* strcpy() definition */
#include <string.h>

#if (0)
/* MAX_PATH definition on Linux */
#include <linux/limits.h>
#else
/* MAX_PATH definition for rest cases */
#define MAX_PATH (4*260)
#endif /* (0) */

/* xmlDoc, xmlNode definition */
#include <libxml/tree.h>

/* parse_xml(), ... */
#include "xmparser.h"

/* pTgtStructType type definition */
#include "structs.h"

/* DGENERAL() */
#include "main.h"

/* Such constants as <HTTP_WRONG_NAME>, <HTTP_BAD_XML_FORMAT> */
#include "./http/constants.h"

/* Such macros as <HCOMMON>, etc */
#include "./http/verbose.h"

/* Ptr to XML data for in memory */
xmlNode *xmlRootElement = NULL;

/* Ptr to XML data document */
xmlDoc *xmlDocument = NULL;

/* Once upon a life we can afford 4096 characters long array */
char cXmlName[PATH_MAX];

/* S.B.-List of parced XML entries */
pTgtStructType Root;


int main (int argc, char **argv)
{
	if (NULL != argv[1])

		/* Assign datafile name*/
		strcpy (cXmlName, argv[1]);

	else
	{
		HCOMMON("[%s] %s:   Put XML file name on command line, e.g. <%s targets.xml>. \nERROR\n", __FILE__, __func__, argv[0]);

		return CMN_WRONG_NAME;
	}

	/* Check potential ABI mismatches between the version it was compiled for and the actual shared library used */
	LIBXML_TEST_VERSION

	xmlDocument = xmlReadFile(cXmlName, NULL, 0);

	if (NULL == xmlDocument)
	{
		DGENERAL("[%s] %s:   ERROR: could not parse file %s\n", __FILE__, __func__, cXmlName);

		return CMN_BAD_DATAFILE;
	}

	/* Get the root node of the XML data stored in the <doc> */
	if ( NULL == ( xmlRootElement = xmlDocGetRootElement(xmlDocument) ) )
	{
		HCOMMON("[%s] %s: badly organized XML doc in file %s\n", __FILE__, __func__, cXmlName);

		return CMN_BAD_XML_FORMAT;
	}


	/* Put all XML sections <Target> into list pointed <Root> */
	parse_xml(xmlRootElement, "Target");


	/* Put XML data on air */
	_ProcessTargets(Root);

	/* Free dynamic memory occupied S.B.-List <Root> */
	_DeleteTarget (Root);



	/* Free the document */
	xmlFreeDoc(xmlDocument);

	/* Free the global variables that may have been allocated by the parser */
	xmlCleanupParser();

	/* In <main> has same effect as <return> */
 	exit (0);
}

