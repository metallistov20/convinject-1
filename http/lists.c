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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CURLOPT_URL */
#include <curl/curl.h>

#include "xmls.h"
#include "lists.h"
#include "http_main.h"
#include "auxiliary.h"
#include "verbose.h"

int AppendXmlAux(pXmlAuxType * ppXmlAux)
{
	if (NULL == *ppXmlAux)
	{
		/* only one chain, for beginning */
		*ppXmlAux = (pXmlAuxType) calloc ( 1, sizeof (XmlAuxType) );

		/* check if successful */
		if (NULL == *ppXmlAux)
		{
			HCOMMON("[%s] %s:   ERROR: can't allocate memory for aux. XML data\n", __FILE__, __func__);

			return HTTP_MEM_ERROR;
		}
	}

	return HTTP_SUCCESS;
}

void DeleteVocabularyEx(pCompoundType * ppThisVocabulary)
{
pCompoundType pChild, pThisVocabulary = *ppThisVocabulary;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisVocabulary)
	{
		/* if space to keep item's name is allocated */
		if (pThisVocabulary->pcData)
		
		    /* then release this space */
		    free(pThisVocabulary->pcData);

		/* preserve a pointer to next record */		    
		pChild = pThisVocabulary->pNext;
		
		/* free space occupied by current record */
		free (pThisVocabulary);
		
		/* Go to next record */
		pThisVocabulary = pChild;
	}

	*ppThisVocabulary = NULL;
}


void DeleteXmlAuxEx(pXmlAuxType * ppXmlAux)
{
pXmlAuxType pThisXmlAux = *ppXmlAux;

	/* Walk through entire list and delete each chain */
	if (NULL != pThisXmlAux)
	{
		/* if we have vocabulary */
		if (NULL != pThisXmlAux->pVocabulary)

		    /* then release it */
		    DeleteVocabularyEx( & pThisXmlAux->pVocabulary );

		/* free space occupied by current record */
		free(pThisXmlAux);
	}

	*ppXmlAux=NULL;
	
	/* done */
	return; 
}

int AppendUrl(pUrlChainType * ppThisUrlChain, char * pcData)
{
pUrlChainType pChild, pTempUrlChain;

	if (NULL == *ppThisUrlChain)
	{
		/* only one chain, for beginning */
		*ppThisUrlChain = (pUrlChainType) calloc ( 1, sizeof (UrlChainType) );

		/* check if successful */
		if (NULL == *ppThisUrlChain)
		{
			HCOMMON("[%s] %s:   ERROR: can't allocate memory for first chain, URL(%s)\n", __FILE__, __func__, pcData);

			return HTTP_MEM_ERROR;
		}
	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisUrlChain;

		pTempUrlChain = (pUrlChainType) calloc ( 1, sizeof (UrlChainType) );

		if (NULL == pTempUrlChain)
		{
			HCOMMON("[%s] %s:   ERROR: can't allocate memory for next chain, URL(%s)\n", __FILE__, __func__, pcData);

			return HTTP_MEM_ERROR;
		}

		/* Skip everything */
		while ( (NULL != pChild) && (NULL != pChild->pNextChain ) )
		{
			/* til the tail */
			pChild = pChild->pNextChain;
		}

		/* before while we've already ensured that next chunk was created allright */

		/* attach a new chain entry to the end of existing chain */
		pChild->pNextChain = pTempUrlChain;

	}
	return HTTP_SUCCESS;
}

int AppendCompound(pCompoundType * ppThisCompound, char * pcData, void * pThisVoid)
{
pCompoundType pChild, pTempCompound;

	if (NULL == *ppThisCompound)
	{
		/* only one chain, for beginning */
		*ppThisCompound = (pCompoundType) calloc ( 1, sizeof (CompoundType) );

		/* check if successful */
		if (NULL == *ppThisCompound)
		{
			HCOMMON("[%s] %s:   ERROR: can't allocate memory for first compound, URL(%s)\n", __FILE__, __func__, pcData);

			return HTTP_MEM_ERROR;
		}

		(*ppThisCompound)->pcData = calloc (1, strlen (pcData) +1 );
	
		strcpy ((*ppThisCompound)->pcData, pcData );

		/* Serve case of vocabulary */
		if (NULL != pThisVoid)

			(*ppThisCompound)->pVar = pThisVoid;
	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisCompound;

		pTempCompound = (pCompoundType) calloc ( 1, sizeof (CompoundType) );

		if (NULL == pTempCompound)
		{

			HCOMMON("[%s] %s:   ERROR: can't allocate memory for next compound, URL(%s)\n", __FILE__, __func__, pcData);

			return HTTP_MEM_ERROR;
		}

		/* Skip everything till last one */
		while ( (NULL != pChild) && (NULL != pChild->pNext ) )
		{
			/* til the tail */
			pChild = pChild->pNext;
		}

		/* before while we've already ensured that next chunk was created allright */

		/* allocate a space needed for item's name */
		pTempCompound->pcData = (char *) calloc ( 1, strlen(pcData) + 1 );
	
		/* do copy item's name */
		strcpy(pTempCompound->pcData, pcData);

		/* Serve case of vocabulary */
		if (NULL != pThisVoid)

			pTempCompound->pVar = pThisVoid;
	
		/* attach a new chain entry to the end of existing chain */
		pChild->pNext = pTempCompound;
	}
	return HTTP_SUCCESS;
}

void DeleteCompoundEx(pCompoundType * ppThisCompound)
{
pCompoundType pChild, pThisCompound = *ppThisCompound;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisCompound)
	{
		/* if space to keep item's name is allocated */
		if (pThisCompound->pcData)
		
		    /* then release this space */
		    free(pThisCompound->pcData);

		/* preserve a pointer to next record */		    
		pChild = pThisCompound->pNext;
		
		/* free space occupied by current record */
		free (pThisCompound);
		
		/* Go to next record */
		pThisCompound = pChild;
	}

	*ppThisCompound = NULL;
}

void DeleteUrlEx(pUrlChainType * ppThisUrlChain)
{
pUrlChainType pChild, pThisUrlChain = *ppThisUrlChain;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisUrlChain)
	{
		/* if we have compound data (i.e. binded-list insted of single string) release  */
		if ( pThisUrlChain->pCompound)

		    /* then release this space */
		    DeleteCompoundEx( & pThisUrlChain->pCompound );


		/* if we've allocated area for cumulative URL */
		if ( pThisUrlChain->pcSumm)

		    /* then release this space */
		    free( pThisUrlChain->pcSumm);

		/* preserve a pointer to next record */		    
		pChild = pThisUrlChain->pNextChain;
		
		/* free space occupied by current record */
		free(pThisUrlChain);
		
		/* Go to next record  */
		pThisUrlChain = pChild;
	}

	*ppThisUrlChain=NULL;
	
	/* done */
	return; 
}

void DisplayString(char * pcDataPar)
{    
	if (NULL != pcDataPar)
		DXMLAUX("[%s] %s:   STRING(%s)\n", __FILE__, __func__, pcDataPar);
	else
		DXMLAUX("[%s] %s:   EMPTY_STR_AT(%p)\n", __FILE__, __func__, &pcDataPar);
}

void DisplayCompound(pCompoundType pCompoundPar)
{    
pCompoundType pCompound = pCompoundPar;

    /* process each  entry of chain */
    while (NULL != pCompound)
    {
	/* issue item's name */	
	if (NULL != pCompound->pcData)

		DisplayString((char *)(pCompound->pcData));

	/* issue item's name */	
	if (NULL != pCompound->pVar)

		DisplayString((char *)(pCompound->pVar));
	
	/* Go to next record of Chainwork */
	pCompound =  pCompound->pNext;
    }
}

void DisplayXmlAux(pXmlAuxType pXmlAuxPar)
{    
pXmlAuxType pThisXmlAux = pXmlAuxPar;

    /* process each  entry of chain */
    if (NULL != pThisXmlAux)
    {
	if (NULL != pThisXmlAux->pVocabulary)
	{
		DXMLAUX("\t[%s] %s:   pCompund is not NULL, its contents are:\n", __FILE__, __func__);

		DisplayCompound(pThisXmlAux->pVocabulary);
	}	
    }
}

void DisplayUrl( pUrlChainType pThisUrlChainPar)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {

	if (NULL != pThisUrlChain->pCompound)
	{
		DXMLAUX("\t[%s] %s:   pCompund is not NULL, its contents are:\n", __FILE__, __func__);

		DisplayCompound(pThisUrlChain->pCompound);
	}

	if (NULL != pThisUrlChain->pCompound)
	{

		DXMLAUX("\t[%s] %s:   Cumulative resulting URL is :\n", __FILE__, __func__);

		DisplayString(pThisUrlChain->pcSumm);

	}
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };	    
}

static int GlueCompound(pCompoundType pCompoundPar, pUrlChainType pUrlChain)
{    
pCompoundType pCompound = pCompoundPar;

int iCnt=0;

	/* process each  entry of chain */
	while (NULL != pCompound)
	{
		/* issue item's name */	
		if (NULL != pCompound->pcData)
		{
			/* glue all burt first */
			if ( 0 < iCnt++ )

				/* standatrd delimited for parameters on URL requets string */
				strcat( (char *)(pUrlChain->pcSumm),  "&" );

			strcat( (char *)(pUrlChain->pcSumm),  (char *)(pCompound->pcData) );
		}

		/* Go to next record of Chainwork */
		pCompound =  pCompound->pNext;
	}

	return HTTP_SUCCESS;
}

int GlueUrl(pUrlChainType pThisUrlChainPar)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {

	pThisUrlChain->pcSumm = (char *) malloc ( MAX_URL_SIZE );

	if ( NULL == pThisUrlChain->pcSumm ) 
	{
		DXML("\t[%s] %s:   can't allocate %d bytes for URL data\n", __FILE__, __func__, MAX_URL_SIZE );

		return HTTP_MEM_ERROR;
	}

	strcpy (pThisUrlChain->pcSumm, "http://");

	if ( NULL != cIpAddr ) 
	{
		strcat (pThisUrlChain->pcSumm, cIpAddr);
	}

	if (NULL != pThisUrlChain->pCompound)
	{
		DXMLAUX("\t[%s] %s:   URL is going to be composed of its particles\n", __FILE__, __func__);

		GlueCompound(pThisUrlChain->pCompound, pThisUrlChain);
	}
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };

    return HTTP_SUCCESS;
}

int DeployUrl(pUrlChainType pThisUrlChainPar)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

int iRes;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {
	if ( NULL == pThisUrlChain->pcSumm ) 
	{
		DXML("\t[%s] %s:   can't allocate %d bytes for URL data:\n", __FILE__, __func__, MAX_URL_SIZE );

		return HTTP_MEM_ERROR;
	}

	DURL("[%s] %s:   summURL", __FILE__, __func__);
	DXMLAUX("(at:<%p>;<%p>;<%p>)", pThisUrlChain, pThisUrlChain->pcSumm, pThisUrlChain->pNextChain);
	DURL(" = %s\n", pThisUrlChain->pcSumm);

	if ( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_URL, pThisUrlChain->pcSumm ) ) )
	{
#if (IDLE_RUN)
		DURL("[%s] %s:   idle run mode: the HTTP injection was not physically sent\n", __FILE__, __func__);
#else
		/* here we produce 'live' HTTP traffic in wire */
		iRes = curl_easy_perform(curl);
#endif /* (IDLE_RUN) */
	}
	else
	{
		DURL("[%s] %s:   cURL call setopt(CURLOPT_URL) failed with ERR_CODE(%d)\n", __FILE__, __func__, iRes); 

		/* don't process other chaing, return immediately */
		return iRes;
	}
	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };


    if ( CURLE_OK == iRes)
    {
	/* go out quietly if all correct */
	return  HTTP_SUCCESS;
    }
    else	
    {
	/* verbosing CURLcode returned as <iRes> if error occured */
	DURL("[%s] %s:   recent cURL call failed with ERR_CODE(%d)\n", __FILE__, __func__, iRes);

	return   HTTP_CURL_ERROR;
    }
}

int DeployUrlEx(pUrlChainType pThisUrlChainPar, int iExtra)
{    
pUrlChainType pThisUrlChain = pThisUrlChainPar;

int iRes;

/* Index of 'extras' to be processed */
int iExtras = 0;

    /* process each  entry of chain */
    while (NULL != pThisUrlChain)
    {
	if ( NULL == pThisUrlChain->pcSumm ) 
	{
		DXML("\t[%s] %s:   can't allocate %d bytes for URL data:\n", __FILE__, __func__, MAX_URL_SIZE );

		return HTTP_MEM_ERROR;
	}

	DURL("[%s] %s:   summURL", __FILE__, __func__);
	DXMLAUX("(at:<%p>;<%p>;<%p>)", pThisUrlChain, pThisUrlChain->pcSumm, pThisUrlChain->pNextChain);
	DURL(" = %s\n", pThisUrlChain->pcSumm);

	if ( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_URL, pThisUrlChain->pcSumm ) ) )
	{
		if ( iExtra )
		{
			switch (iExtras++)
			{
				case 0:
				{
					if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *pcPtr2Extra1) ))
					{
						if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 
							strlen(*pcPtr2Extra1))) )
						{
							break;
						}
					}

					DURL("[%s] %s:   cURL call setopt(CURLOPT_POSTFIELDS) failed with ERR_CODE(%d)\n", __FILE__, __func__, iRes); 

					return   HTTP_CURL_ERROR;

				}
				case 1:
					if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *pcPtr2Extra2) ))
					{
						if( CURLE_OK == ( iRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 
							strlen(*pcPtr2Extra2))) )
						{
							break;
						}
					}

					DURL("[%s] %s:   cURL call setopt(CURLOPT_POSTFIELDS) failed with ERR_CODE(%d)\n", __FILE__, __func__, iRes); 

					return  HTTP_CURL_ERROR;

				default:
					break;
			}


		}

#if (IDLE_RUN)
		DURL("[%s] %s:   idle run mode: the HTTP injection was not physically sent\n", __FILE__, __func__);
#else
		/* here we produce 'live' HTTP traffic in wire */
		iRes = curl_easy_perform(curl);
#endif /* (IDLE_RUN) */
	}
	else
	{
		DURL("[%s] %s:   cURL call setopt(CURLOPT_URL) failed with ERR_CODE(%d)\n", __FILE__, __func__, iRes); 

		/* don't process other chaing, return immediately */
		return iRes;
	}

	
	/* Go to next record of Chainwork */
	pThisUrlChain =  pThisUrlChain->pNextChain;
    };


    if ( CURLE_OK == iRes)
	/* go out quietly if all correct */
	return  HTTP_SUCCESS;
    else
    {
	/* verbosing CURLcode returned as <iRes> if error occured */
	DURL("[%s] %s:   recent cURL call failed with ERR_CODE(%d)\n", __FILE__, __func__, iRes);

	return   HTTP_CURL_ERROR;
    }
}


void CleanQuine()
{
int i;

	/* Everything bout <_tid_> to be cleaned. TODO: sooner or later do the same with <target> */
	for (i = 1; NULL != QuineArray[i].vpQuineVar; i++)
	{
		DVOC("[%s] %s:   before clean: keyword vpQuineVar<%s>   pcQuineVarValue<%s> \n", __FILE__, __func__, 
			QuineArray[i].vpQuineVar, QuineArray[i].pcQuineVarValue);

		/* Delete string stored at address <QuineArray[i].vpQuineVar> */
		* (char**)( QuineArray[i].vpQuineVar) = NULL;

		DVOC("[%s] %s:   after clean: here's the keyword vpQuineVar<%s>   pcQuineVarValue<%s>\n", __FILE__, __func__, 
			QuineArray[i].vpQuineVar, QuineArray[i].pcQuineVarValue);
	}
}

int XmlAuxCreateEx()
{
char cStr[MAX_URL_SIZE], cGarb[MAX_URL_SIZE];

void * pVoid;

	if (HTTP_SUCCESS == AppendXmlAux( &pAuxiliary ) )
	{
	int i;
		/* Tip: here we presume that element QuineArray[0] exists. TODO; re-work, to avoid relying on this presumption */
		for (i = 0; ( NULL != QuineArray[i].pcQuineVarValue ) &&
			(NULL != QuineArray[i].vpQuineVar); i++ )
		{

			DVOC("[%s] %s:   adding keyword (%s=) to vocabulary; at addr<%p> \n", __FILE__, __func__, 
				QuineArray[i].pcQuineVarValue, QuineArray[i].vpQuineVar);

			AppendCompound(&pAuxiliary->pVocabulary, (char*)QuineArray[i].pcQuineVarValue, QuineArray[i].vpQuineVar);
		}

#if (DEBUG_VOC)
		DisplayXmlAux(pAuxiliary);
#endif /* (DEBUG_VOC) */
	}
	else
	{
		HCOMMON("[%s] %s:   ERROR: auxilary data structure was not created\n", __FILE__, __func__);

		return HTTP_MEM_ERROR;
	}

	return HTTP_SUCCESS;
}
