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
MIDDLE:= 4
MINOR:= 0

OBJS	= ./authentication.o ./cmds.o ./connect_ssh.o ./knownhosts.o ./pipes.o 

CFLAGS = -c -g -I/usr/include/libxml2             -D_DBG
LDFLAGS= -L/usr/local/lib 
LIBS   = -lxml2

CFLAGS += -I/home/mkn/_libssh/libssh/include -L./ssh/shared 

VERSION := $(MAJOR).$(MIDDLE).$(MINOR)


PWD=$(shell pwd)


CFLAGS 	+=
LDFLAGS +=


all: $(OBJS)

.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)  
#-L./ssh/shared -Wl,--rpath-link $(PWD)/ssh/shared  -Wl,--rpath $(PWD)/ssh/shared

clean:
	$(RM)  *.o *.so* *~ core *.o *.c~
