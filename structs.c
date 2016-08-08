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

#include <stdlib.h>

#include "structs.h"

/* creates Target and initializes its initial element */
pTgtStructType _CrTarget(unsigned long uloNewTargetAddress, pTgtStructType pbNewTarget)
{
	/* only one chain, for breginning */
	pbNewTarget = (pTgtStructType) malloc ( sizeof (TgtStructType) );

	/* check if successful */
	if (!pbNewTarget)
		/* NULL identifies "failure on creation" error */
		return  NULL;

#if (0)
	/* allocate a memory for name string */
	pbNewTarget->pNextuchName = (unsigned char *) malloc (strlen("(start)"));	

	/* assing this string a value */
	strcpy(pbNewTarget->pNextuchName, "(start)");
#else
	// TODO. 

#endif /* (0) */

	/* a lock-up  */
	pbNewTarget->pNext = NULL;
	
	/* NULL identifies "failure on creation" error */
	return pbNewTarget;
}

/* registers an ditem '_NameOfItem' underf address '_AddressOfItem' */
void _AppendTarget(pTgtStructType pbThisTarget, unsigned char * _NameOfItem, unsigned long _AddressOfItem)
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
#if (0)
		/* allocate a space needed for item's name */
		pbTempTgtStructType->pNextuchName = (unsigned char *) malloc (strlen(_NameOfItem));
		
		/* do copy item's name */
		strcpy(pbTempTgtStructType->pNextuchName, _NameOfItem);
#else
		// TODO. enroll 'em all
#endif /* (0) */
		
		/* set a look-up */
		pbTempTgtStructType->pNext = NULL;		

		/* append a new Target entry to the end of existing Target */
		pbChild->pNext = pbTempTgtStructType;
	}
	else
		/* memory for new recored hasn't been allocated */
		return; 
	
}

/* send information about all current Targets entries through serial line */
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


/* erase an entire Target, useless but kept for symmetrisity with '_CrTarget()' */
void _DeleteTarget(pTgtStructType pbThisTarget)
{
/* a tomparary variable of type 'pTgtStructType' */
pTgtStructType pbChild;

	/* Walk through entire list and delete each chain */
	while (NULL != pbThisTarget)
	{
#if (0)
		/* if space to keep item's name is allocated */
		if (pbThisTarget->pNextuchName)
		
		    /* then release this space */
		    free(pbThisTarget->pNextuchName);
#else
#endif /* (0) */
		    
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
