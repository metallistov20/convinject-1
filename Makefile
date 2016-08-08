#
# (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

CFLAGS :=

CC := gcc

MAJOR := 4
MIDDLE := 4
MINOR := 0

NAME:= ssh
NAME_A := ./access_srv
PWD=$(shell pwd)


##CFLAGS=-I/home/mkn/_libssh/libssh/include -L./shared                   -g   -I/usr/include/libxml2 -L/usr/local/lib  -lxml2
#CFLAGS=  -g   -I/usr/include/libxml2 -L/usr/local/lib  -L/usr/lib/i386-linux-gnu  -lxml2  
#CFLAGS+= -D_DBG

CFLAGS =  -c -g   -I/usr/include/libxml2  -D_DBG
LDFLAGS = -L/usr/local/lib 
LIBS =  	-lxml2

#CFLAGS =  -c -g   -I/usr/include/libxml2
#LDFLAGS = -L/usr/local/lib 

#LIBS = -lcurl 	-lxml2

VERSION := $(MAJOR).$(MIDDLE).$(MINOR)

all: $(NAME_A)

lib: ./shared/lib$(NAME).so.$(VERSION)                                    

##$(NAME_A):	./shared/lib$(NAME).so
#$(NAME_A):	./xmparser.c
#		$(CC)   $(CFLAGS)  ./xmparser.c    -o $@   \

#		 -Wl,--rpath-link $(PWD)/shared  -Wl,--rpath $(PWD)/shared

.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

$(NAME_A): ./xmparser.o
	$(CC)  -o $@ ./xmparser.o $(LDFLAGS) $(LIBS)


clean:
	$(RM) $(NAME_A) *.o *.so* *~
