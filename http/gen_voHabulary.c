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

#include <sys/stat.h>

/* TODO: should come as command line argument */
//#define FNAME "cast.5428E.txt.voc"
#define FNAME "cast.2218.txt.voc"

int main()
{             
FILE *fp;

unsigned long ulo;

struct stat fileStats;

	if (stat(FNAME, &fileStats) != 0)
	{

		printf("/* Can't stat file %s*/ \n", FNAME);

		return -2;

	}
	else
	{
		fp=fopen(FNAME,"r");

		if(NULL == fp)
		{
			printf("/* Some problem on FIRST opening the file %s*/ ", FNAME );

			return -1;
		} 
		else
		{ 
		int i=0;
		char c[512];

			printf("/* This is AUTOGENERATED file. Do NOT edit it. */\n");

			printf("\n#include \"constants.h\"\n\n");

			while ( !feof(fp) )
			{
				i++;

				fscanf(fp, "%s\n", &c[0]);

				printf("/* Decl. of variable for %d%s keyword found in <%s> file */\n", i, (i==1)?"st":((i==2)?"nd":((i==3)?"rd":"th")), FNAME );

				printf("extern char %s [MAX_STR_SIZE];\n\n", &c[0]);
			}
			fclose(fp);

			printf("/* End of AUTOGENERATed file */");
		}
	}

	return 0;
}
