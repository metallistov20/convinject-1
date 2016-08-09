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

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

/* S.-b.-list,  */
typedef struct _DtaStructType
{
	char * pcType;
	char * pcName;
	char * pcAddress;
	char * pcLogin;
	char * pcPasswd;
	char * pcDatafile;
	char * pcProto;

} DtaStructType, *pDtaStructType;

/* S.-b.-list,  */
typedef struct _TgtStructType
{
	struct _DtaStructType * pDta;

	struct _TgtStructType * pNext;

} TgtStructType, *pTgtStructType;


void _AddTarget(pTgtStructType * ppbThisTarget, pDtaStructType pDta);

void _ProcessTargets(pTgtStructType pbThisTarget);

void _DeleteTarget(pTgtStructType pbThisTarget);

#endif /* _STRUCTS_H_ */
