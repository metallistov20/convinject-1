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

#include "structs.h"

/* creates Target and initializes its initial element */
pTgtStructType _CrTarget(pDtaStructType pDta, pTgtStructType pbNewTarget)
{
	/* only one chain, for breginning */
	pbNewTarget = (pTgtStructType) malloc ( sizeof (TgtStructType) );

	/* check if successful */
	if (!pbNewTarget)
		/* NULL identifies "failure on creation" error */
		return  NULL;

	if (NULL != pDta)
	{
		pbNewTarget->pDta = (pDtaStructType) malloc( sizeof (struct _DtaStructType) );

		if (NULL != pbNewTarget->pDta)
		{
			pbNewTarget->pDta->pcType = (char*) malloc (strlen (pDta->pcType) +1 );
			strcpy(pbNewTarget->pDta->pcType, pDta->pcType);

			pbNewTarget->pDta->pcName = (char*) malloc (strlen (pDta->pcName) +1 );
			strcpy(pbNewTarget->pDta->pcName, pDta->pcName);

			pbNewTarget->pDta->pcAddress = (char*) malloc (strlen (pDta->pcAddress) +1 );
			strcpy(pbNewTarget->pDta->pcAddress, pDta->pcAddress);

			pbNewTarget->pDta->pcLogin = (char*) malloc (strlen (pDta->pcLogin) +1 );
			strcpy(pbNewTarget->pDta->pcLogin, pDta->pcLogin);

			pbNewTarget->pDta->pcPasswd = (char*) malloc (strlen (pDta->pcPasswd) +1 );
			strcpy(pbNewTarget->pDta->pcPasswd, pDta->pcPasswd);

			pbNewTarget->pDta->pcDatafile = (char*) malloc (strlen (pDta->pcDatafile) +1 );
			strcpy(pbNewTarget->pDta->pcDatafile, pDta->pcDatafile);

			pbNewTarget->pDta->pcProto = (char*) malloc (strlen (pDta->pcProto) +1 );
			strcpy(pbNewTarget->pDta->pcProto, pDta->pcProto);
		}
		else
			printf("ERROR: can't allocate mem. while copying initialization data\n");

	}
	else
		printf("ERROR: bad initialization data\n");



	/* a lock-up  */
	pbNewTarget->pNext = NULL;
	
	/* NULL identifies "failure on creation" error */
	return pbNewTarget;
}

/* registers an ditem '_NameOfItem' underf address '_AddressOfItem' */
void _AppendTarget(pTgtStructType pbThisTarget, pDtaStructType pDta)
{
/* introduce two temporary variables of type 'pTgtStructType' */
pTgtStructType pbChild, pbTempTgtStructType;

	/* point with first temporary element to head of Target */
	pbChild = pbThisTarget;

	/* allocate a space for new record in Behorde */
	pbTempTgtStructType = (pTgtStructType) malloc ( sizeof (TgtStructType));

	/* Skip everything */
	while (NULL != pbChild->pNext )

		/* til the tail */
		pbChild = pbChild->pNext;
		
	/* if previous memory allocation was successful */
	if(pbTempTgtStructType != NULL)
	{
		if (NULL != pDta)
		{
			pbTempTgtStructType->pDta = (pDtaStructType) malloc( sizeof (struct _DtaStructType) );

			if (NULL != pbTempTgtStructType->pDta)
			{
				pbTempTgtStructType->pDta->pcType = (char*) malloc (strlen (pDta->pcType) +1 );
				strcpy(pbTempTgtStructType->pDta->pcType, pDta->pcType);

				pbTempTgtStructType->pDta->pcName = (char*) malloc (strlen (pDta->pcName) +1 );
				strcpy(pbTempTgtStructType->pDta->pcName, pDta->pcName);

				pbTempTgtStructType->pDta->pcAddress = (char*) malloc (strlen (pDta->pcAddress) +1 );
				strcpy(pbTempTgtStructType->pDta->pcAddress, pDta->pcAddress);

				pbTempTgtStructType->pDta->pcLogin = (char*) malloc (strlen (pDta->pcLogin) +1 );
				strcpy(pbTempTgtStructType->pDta->pcLogin, pDta->pcLogin);

				pbTempTgtStructType->pDta->pcPasswd = (char*) malloc (strlen (pDta->pcPasswd) +1 );
				strcpy(pbTempTgtStructType->pDta->pcPasswd, pDta->pcPasswd);

				pbTempTgtStructType->pDta->pcDatafile = (char*) malloc (strlen (pDta->pcDatafile) +1 );
				strcpy(pbTempTgtStructType->pDta->pcDatafile, pDta->pcDatafile);

				pbTempTgtStructType->pDta->pcProto = (char*) malloc (strlen (pDta->pcProto) +1 );
				strcpy(pbTempTgtStructType->pDta->pcProto, pDta->pcProto);

			}
			else
				printf("ERROR: can't allocate mem. while copying initialization data\n");

		}
		else
			printf("ERROR: bad initialization data\n");
		
		/* set a look-up */
		pbTempTgtStructType->pNext = NULL;		

		/* append a new Target entry to the end of existing Target */
		pbChild->pNext = pbTempTgtStructType;
	}
	else
		/* memory for new recored hasn't been allocated */
		return; 
	
}

/* deploy information about all current Targets */
void _ProcessTargets(/*pHandle phThisHandle, */pTgtStructType pbThisTarget)
{
/* a storage for number under which the item is registerd in Target */
unsigned char _the_number;
    
    /* process each Target's entry */
    while (pbThisTarget != NULL )
    {

	// TODO do effective processing here 	
	
	/* Go to next record of Target */
	pbThisTarget =  pbThisTarget->pNext;
    };	
    
}


/* erase an entire Target */
void _DeleteTarget(pTgtStructType pbThisTarget)
{
/* a tomparary variable of type 'pTgtStructType' */
pTgtStructType pbChild;

	/* Walk through entire list and delete each chain */
	while (NULL != pbThisTarget)
	{
		if (NULL != pbThisTarget->pDta) 
		{
			free(pbThisTarget->pDta->pcType);

			free(pbThisTarget->pDta->pcName);

			free(pbThisTarget->pDta->pcAddress);

			free(pbThisTarget->pDta->pcLogin);

			free(pbThisTarget->pDta->pcPasswd);

			free(pbThisTarget->pDta->pcDatafile);

			free(pbThisTarget->pDta->pcProto);

			free (pbThisTarget->pDta);
		}
		else
			printf("ERROR: nothing to release\n");
		    
		/* preserve a pointer to next record */		    
		pbChild = pbThisTarget->pNext;
		
		/* free space occupied by current record */
		free (pbThisTarget);
		
		/* Go to next record */
		pbThisTarget = pbChild;
	}
	
	/* Done */
	return; 
}
