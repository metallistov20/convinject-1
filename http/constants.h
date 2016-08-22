/*
 Tested switches: TL-SL2428, TL-SL2218, TL-SL5428E

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

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

/* Successfull operation */
#define  HTTP_SUCCESS		0

/* Bad paramaters */
#define HTTP_PAR_ERROR		(-1)

/* Memory was not allocated, or bad mem. address being referred */
#define HTTP_MEM_ERROR		(-3)

/* call to fn. of library cURL was failed */
#define HTTP_CURL_ERROR		(-4)

/* Operation is not yet impelmented */
#define HTTP_NOT_IMPL		(-8)

/* Empty string instead of XML node name */
#define HTTP_XML_NULL		(-9)

#define  HTTP_BAD_VOC		(-10)


/* */
#define  HTTP_WRONG_NAME	(-11)

/* */
#define  HTTP_BAD_XML_FORMAT	(-12)

/* */
#define  HTTP_BAD_DATAFILE		(-13)

/* Bad file name */
#define  HTTP_BAD_FNAME		(-14)

/* File does not exist, or requested permission is not granted */
#define  HTTP_BAD_FOPEN		(-15)

#define  HTTP_BAD_DATA		(-17)

#define  HTTP_BAD_LIBINIT	(-18)






/* Assuming that 1K shoudl be enought for Barracuda */
#define MAX_URL_SIZE 	1024

/* 12 digits + 3 dots */
#define MAX_IP_SIZE 	15

/* Depends on switch SW, Assuming that 110 is enough */
#define MAX_SNMP_SIZE 	110

/* Depends on switch SW, Assuming that 64 is enough */
#define MAX_TID_SIZE 	64

/* Auxiliary strings, those not library- or Barracuda-dependant */
#define MAX_STR_SIZE 	64

/* On linux systems should (but not must) be same as MAX_PATH */
#define HTTP_MAX_PATH 	260



/* Dont sent URLs physically, only show them */
#define IDLE_RUN	1


#endif /* _CONSTANTS_H_ */
