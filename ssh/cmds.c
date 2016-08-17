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

/* stdout */
#include <stdio.h>

/* strlen() */
#include <string.h>

/* calloc() */
#include <stdlib.h>

/* Data structure type definition */
#include "cmds.h"


/* How much seconds to wait between commands */
#define BETW_CMD_TMO		2

/* Allocation/deallocation was succesful */
#define SUCCESS_MEM		0

/* Alocation/deallocation failed */
#define ERROR_MEM		(-4)

/* File desciptors of pipe to write STDIN-data to */
extern int input_pipe[2];

/* Enroll single command into struct. Allocates memory for <*ppThisCmdChain> once not allocated, yet */
int _EnrollCmd(const char * caller, pCmdType * ppThisCmdChain, char * pcCmd)
{
pCmdType pChild, pTempCmdChain;

	if (NULL == *ppThisCmdChain)
	{
		/* only one chain, for beginning */
		*ppThisCmdChain = (pCmdType) calloc ( 1, sizeof (CmdType) );

		/* check if successful */
		if (NULL == *ppThisCmdChain)
		{
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. \n",
			__FILE__, caller, __func__);

			return ERROR_MEM;
		}

		/* fulfill data */
		(*ppThisCmdChain)->pcCmd = calloc (1, strlen (pcCmd) +1 );

		strcpy( (*ppThisCmdChain)->pcCmd, pcCmd);
	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisCmdChain;

		pTempCmdChain = (pCmdType) calloc (1, sizeof (CmdType) );

		if (NULL == pTempCmdChain)
		{

			printf("[%s] %s:%s : ERROR: can't allocate memory for next element. \n", 
			__FILE__, caller, __func__);

			return ERROR_MEM;
		}

		/* fulfill data */
		pTempCmdChain->pcCmd = calloc (1, strlen (pcCmd) +1 );

		strcpy( pTempCmdChain->pcCmd, pcCmd);

		/* Skip everything, except last entry */
		while ( (NULL != pChild) && (NULL != pChild->pNext ) )
		{
			/* . . */
			pChild = pChild->pNext;
		}

		/* Next chunk was created allright (we know it at this moment), so we attach a new chain entry to the end of existing chain */
		pChild->pNext = pTempCmdChain;
	}

	return SUCCESS_MEM;

}


extern int output_pipe[2];


/* Process data stored in dynamic structure pointed by 'pPointChainPar' */
static int ProcessSingleCmd(/* const char * caller, */pCmdType pPointChainPar)
{
char cResponceData[0x400];

int iNumRead;

	/* Wait between commands */
	sleep (BETW_CMD_TMO);

	/* Push next command from tray into second endpoint of pipe */
	write(input_pipe[1], pPointChainPar->pcCmd, strlen (pPointChainPar->pcCmd) +  1);

#if defined(OUT_PIPE)
	iNumRead = read(output_pipe[1], cResponceData, 0x400);
#endif /* OUT_PIPE */

#if defined(_DBG)
	if (-1 == iNumRead)
		printf("[read ERR]\n");
	else
		if (0 == iNumRead)
			printf("[eof]\n");
		else
			printf(">>>>RSPNC>>>> %s <<<<<\n", cResponceData );
#endif /* 0 */

}

/* VERBOSE_STATUS */
//2del #include "../http/verbose.h"

static int ProcessSingleHttpCmd(/* const char * caller, */pCmdType pPointChainPar)
{

/* Index of desired operation (can be 'create', 'save', 'ACL', 'firmware' )*/
int iOperation;

	/* Wait between commands */
	sleep (BETW_CMD_TMO);

	//write(input_pipe[1], pPointChainPar->pcCmd, strlen (pPointChainPar->pcCmd) +  1);
//	VERBOSE_STATUS(NULL/*pPointChainPar->pcCmd*/);

#if (0)
		/* Avoid dafault 0 value */
		iOperation=DO_NO_OP;

		/* Parce each parameter */
		switch (iOption)
		{

			/* Single: open site */
			case 'o':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_OPEN_OP;
				break;

			/* Single: close site */
			case 'x':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_CLOSE_OP;
				break;

			/* Single: create SNMP group */
			case 'c':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_CREATE_OP;
				break;

			/* Single: save changes on site  */
			case 's':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_SAVE_OP;
				break;

			/* Single: perform ACL setting */
			case 'a':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_ACL_OP;
				break;

			/* Single: firmware upload and upgrade */
			case 'r':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_FIRMWARE_OP;
				break;

			/* Single: switch reboot */
			case 'b':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_REBOOT_OP;
				break;

			/* Single: ipset */
			case 'g':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_IPSET_OP;
				break;


			/* Single: ip v6 set */
			case 'e':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_IPV6SET_OP;
				break;
			/* Single: access */
			case 'k':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_ACNTL_OP;
				break;
			/* Single: port security */
			case 'h':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PSEC_OP;
				break;
			/* Single: port mirroring */
			case 'j':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PMIR_OP;
				break;
			/* Single: vlan conf. create */
			case 'm':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_VLANCR_OP;
				break;
			/* Single: vlan configure */
			case 'n':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_VLANC_OP;
				break;
			/* Single: ip range */
			case 'q':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_IPRAN_OP;
				break;
			/* Single: port filter */
			case 'p':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PFILT_OP;
				break;
			/* Single: ping */
			case 'y':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_PING_OP;
				break;
			/* Single: tracert */
			case 'z':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_TRACERT_OP;
				break;
			/* Single: cable test */
			case 'v':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_CABLE_OP;
				break;
			/* Single: loopback */
			case 'w':
				DCOMMON("%s: option -%c\n", cArg0, iOption);
				iOperation = DO_LOOPBK_OP;
				break;



			/* Couple: IP addr of target switch */
			case 't':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(cIpAddr, optarg);
				break;

			/* Couple: tID of the session */
			case 'i':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(_tid_, optarg);
				break;

			/* Couple: SNMP community name to be created on target switch*/
			case 'u':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(txt_comname, optarg);
				break;

			/* Couple: Firmware name to be uploaded and upgraded on switch*/
			case 'f':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(cFwName, optarg);
				break;

			/* Couple: Filename of XML file with data */
			case 'd':
				DCOMMON("%s: option -%c with value `%s'\n", cArg0, iOption, optarg);
				strcpy(cXmlName, optarg);
				break;

			/* Couple: Assign ACL setings */
			case 'l':
				DCOMMON("%s: option -%c (--acl-data) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(aclId, optarg);
				break;

			/* Couple: ip address */
			case '0':
				DCOMMON("%s: option -%c (--ip-addr) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(ip_address, optarg);
				break;

			/* Couple: ip network mask */
			case '1':
				DCOMMON("%s: option -%c (--ip-mask) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(ip_mask, optarg);
				break;

			/* Couple:  */
			case '2':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(t_mode, optarg);
				break;
			/* Couple:  */
			case '3':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(t_key, optarg);
				break;
			/* Couple:  */
			case '4':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(t_stat, optarg);
				break;
			/* Couple:  */
			case '5':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(chk_, optarg);
				break;
			/* Couple:  */
			case '6':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(cb_, optarg);
				break;
			/* Couple:  */
			case '7':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(list_, optarg);
				break;
			/* Couple:  */
			case '8':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(member_, optarg);
				break;
			/* Couple:  */
			case '9':
				DCOMMON("%s: option -%c (...) with value `%s'\n", cArg0, iOption, optarg);
				strcpy(proof, optarg);
				break;


			case '?':
				/* getopt_long prints an error message, so we don't */
				break;

			default:
				DCOMMON("%s: bad usage, exiting", cArg0);
				abort ();
		}
//#endif /* (0) */

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
				DCOMMON("%s: there's no operation with OPCODE=%d, exiting\n", cArg0, iOperation);
				break;
			
		}
#endif /* (0) */


}

/* Process data stored in dynamic structure pointed by 'pPointChainPar' */
int _ProcessCmds(const char * caller, pCmdType pPointChainPar)
{
pCmdType pPointChain = pPointChainPar;

	/* Process each entry of chain */
	while (NULL != pPointChain)
	{		
#if defined(_DBG)
		printf ("PRINT OUT<pPointChainPar=%p>:%s\n", pPointChain, pPointChain->pcCmd);
#endif /* 0 */

		/* Realtime and relative-time values */
		ProcessSingleCmd(pPointChain);

		/* Go to next record of chain */
		pPointChain = pPointChain->pNext;
	}

	return SUCCESS_MEM;

}


int _ProcessHttpCmds(const char * caller, pCmdType pPointChainPar)
{
pCmdType pPointChain = pPointChainPar;

	/* Process each entry of chain */
	while (NULL != pPointChain)
	{		
#if defined(_DBG)
		printf ("PRINT OUT<pPointChainPar=%p>:%s\n", pPointChain, pPointChain->pcCmd);
#endif /* 0 */

		/* Realtime and relative-time values */
		ProcessSingleHttpCmd(pPointChain);

		/* Go to next record of chain */
		pPointChain = pPointChain->pNext;
	}

	return SUCCESS_MEM;

}

/* Free memory occupied by '*ppThisCmdChain' */
void _DeleteCmds(const char * caller, pCmdType * ppThisCmdChain)
{
pCmdType pChild, pThisCmdChain = *ppThisCmdChain;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisCmdChain)
	{
		/* if space to keep item's name is allocated */
		if (pThisCmdChain->pcCmd)
		
		    /* then release this space */
		    free(pThisCmdChain->pcCmd);

		/* preserve a pointer to next record */		    
		pChild = pThisCmdChain->pNext;
		
		/* free space occupied by current record */
		free (pThisCmdChain);
		
		/* Go to next record */
		pThisCmdChain = pChild;
	}

	/* Dispose first element of chain */
	*ppThisCmdChain = NULL;

}
