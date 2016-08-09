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


ifeq ($(strip $(proto)),)
$(error "Define protocols to support, such as: 'proto=ssh', 'proto=http' ")
endif

ifeq ($(strip $(proto)),ssh)
	MAJOR := 4
	MIDDLE:= 4
	MINOR:= 0

	OBJS	= ./main.o ./xmparser.o ./structs.o  ./ssh/authentication.o ./ssh/cmds.o ./ssh/connect_ssh.o ./ssh/knownhosts.o ./ssh/pipes.o 

	CFLAGS = -c -g -I/usr/include/libxml2
	LDFLAGS= -L/usr/local/lib 
	LIBS   = -lxml2

	CFLAGS += -I/home/mkn/_libssh/libssh/include -L./ssh/shared 

	VERSION := $(MAJOR).$(MIDDLE).$(MINOR)


	EXTRA=
else
	ifeq ($(strip $(proto)),http)
		CFLAGS=

		OBJS= 
		GRBG=*.o *~ m
	endif
endif



EXEC := ./convinject
PWD=$(shell pwd)


CFLAGS 	+=
LDFLAGS +=
LIBS 	+= ./ssh/shared/libssh.so.$(VERSION)

all: $(EXEC)

.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)  -L./ssh/shared -Wl,--rpath-link $(PWD)/ssh/shared  -Wl,--rpath $(PWD)/ssh/shared

$(EXEC): $(OBJS) $(LIBS)
	$(CC)  -o $@ $(OBJS) $(LIBS)   $(LDFLAGS)   -L./ssh/shared -Wl,--rpath-link $(PWD)/ssh/shared  -Wl,--rpath $(PWD)/ssh/shared


clean:
	$(RM) $(EXEC) *.o *.so* *~ core ./ssh/*.o
