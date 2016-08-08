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

#ifndef _XMPARSER_H_
#define _XMPARSER_H_

#define INJ_XML_ERROR	(-1)

#if (0)
/* S.-b.-list, contains volabulary, which is s.b.-list, either */
typedef struct _XmlAuxType
{
	/* Vocabulary of keywords relevant to this XML file */
	struct _CompoundType * pVocabulary;	

} XmlAuxType, *pXmlAuxType;


/* Pointer to auxiliary data for current XML file */
extern pXmlAuxType pAuxiliary;

#endif /* (0) */

	#define DCOMMON(format, ...) fprintf (stdout, format, __VA_ARGS__)

	#define DURL(format, ...) fprintf (stdout, format, __VA_ARGS__)

	#define DURLAUX(format, ...) fprintf (stdout, format, __VA_ARGS__)

	#define DXML(format, ...) fprintf (stdout, format, __VA_ARGS__)

	#define DXMLAUX(format, ...) fprintf (stdout, format, __VA_ARGS__)

	#define DVOC(format, ...) fprintf (stdout, format, __VA_ARGS__)

	#define DFUNC(format, ...) fprintf (stdout, format, __VA_ARGS__)


#endif /* _XMPARSER_H_ */
