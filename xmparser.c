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

/* List (SB) of parced XML entries */
pTgtStructType Root;

xmlNode *cur_node;

struct _DtaStructType cur_data;

void extract_xml_values(xmlNode * a_node)
{
		cur_node = a_node;

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
						printf("its content is <%s> [%d].\n", _ch_cur_node->content, strlen (_ch_cur_node->content) );
#if (1)
//TODO: here we use different pointers <cur_node> and <_ch_cur_node> as pair. It's wrong
// and can't be relevant unless extra check is done. Nevertheless..

						if ( 0 == strcmp ("Type", cur_node->name) )
						{
							cur_data.pcType = malloc (strlen (_ch_cur_node->content) + 1 ) ;
							strcpy (cur_data.pcType, _ch_cur_node->content);
						}

						if ( 0 == strcmp ("Name", cur_node->name) )
						{
							cur_data.pcName = malloc (strlen (_ch_cur_node->content) + 1 ) ;
							strcpy (cur_data.pcName, _ch_cur_node->content);
						}


						if ( 0 == strcmp ("Address", cur_node->name) )
						{
							cur_data.pcAddress = malloc (strlen (_ch_cur_node->content) + 1 ) ;
							strcpy (cur_data.pcAddress, _ch_cur_node->content);
						}


						if ( 0 == strcmp ("Login", cur_node->name) )
						{
							cur_data.pcLogin = malloc (strlen (_ch_cur_node->content) + 1 ) ;
							strcpy (cur_data.pcLogin, _ch_cur_node->content);
						}

						if ( 0 == strcmp ("Passwd", cur_node->name) )
						{
							cur_data.pcPasswd = malloc (strlen (_ch_cur_node->content) + 1 ) ;
							strcpy (cur_data.pcPasswd, _ch_cur_node->content);
						}

						if ( 0 == strcmp ("Datafile", cur_node->name) )
						{
							cur_data.pcDatafile = malloc (strlen (_ch_cur_node->content) + 1 ) ;
							strcpy (cur_data.pcDatafile, _ch_cur_node->content);
						}

						if ( 0 == strcmp ("Proto", cur_node->name) )
						{
							cur_data.pcProto = malloc (strlen (_ch_cur_node->content) + 1 ) ;
							strcpy (cur_data.pcProto, _ch_cur_node->content);

							_AddTarget(&Root, &cur_data);
						}



#endif /* (1) */
					} /* if ( XML_TEXT_NODE == _ch_cur_node->type) */

					/* Get out form this <for> */
					break;
				} /* for */

			} /* if( cur_node->name == Type, Name, Address, et all ) */

		}/* if (XML_ELEMENT_NODE == cur_node->type)*/


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

	_DeleteTarget (Root);

	return 0;
#endif /* (0) */
}

