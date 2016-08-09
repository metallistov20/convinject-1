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

/* MAX_PATH definition */
#include <linux/limits.h>

/* xmlDoc, xmlNode definition */
#include <libxml/tree.h>

/* _parse_xml(), ... */
#include "xmparser.h"

/* pTgtStructType type definition */
#include "structs.h"

/* Ptr to XML data for in memory */
xmlNode *root_element = NULL;

/* Ptr to XML data document */
xmlDoc *doc = NULL;

/* Once upon a life we can afford 4096 characters long array */
char cXmlName[PATH_MAX];

/* List (SB) of parced XML entries */
pTgtStructType Root;


int main (int argc, char **argv)
{
	/* Assign datafile name*/
	strcpy (cXmlName, argv[1]);

	doc = xmlReadFile(cXmlName, NULL, 0);

	if (NULL == doc)
	{
//.		DGENERAL("ERROR: could not parse file %s\n", cXmlName);

		return (-2);
	}

	/* Get the root node of the XML data stored in the <doc> */
	root_element = xmlDocGetRootElement(doc);


	/* Put all XML sections <Target> into list pointed <Root> */
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

	_DeleteTarget (Root);

	return 0;
#endif /* (0) */
}

