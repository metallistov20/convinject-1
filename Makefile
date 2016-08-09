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


CC := gcc

	OBJS	= ./main.o ./xmparser.o ./structs.o 

	CFLAGS = -c -g -I/usr/include/libxml2
	LDFLAGS= -L/usr/local/lib 
	LIBS   = -lxml2

	CFLAGS += -I/home/mkn/_libssh/libssh/include -L./ssh/shared 

	VERSION := 4.4.0

	EXTRA=

SSH_DIR=ssh


EXEC := ./convinject
PWD=$(shell pwd)


CFLAGS 	+=
LDFLAGS +=
LIBS 	+= ./ssh/shared/libssh.so.$(VERSION)

all: $(EXEC)

#TODO: remork - don't enumerate them plainly
SSH_OBJS	= ./ssh/cmds.o ./ssh/authentication.o  ./ssh/connect_ssh.o  ./ssh/knownhosts.o  ./ssh/pipes.o

#TODO: remork - the second case - http - drops out from this contruction, which is surely not a purpose
ssh:	$(SSH_OBJS)
	cd ./$(SSH_DIR) && $(MAKE) 
	@echo "SSH created:" && ls ./ssh/*.o
	$(MAKE) "SSH_OBJS_R=$(SSH_OBJS)" all 

.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)  

$(EXEC): $(OBJS) $(SSH_OBJS_R) $(LIBS)
	$(CC)  -o $@ $(OBJS) $(SSH_OBJS_R)  $(LIBS)   $(LDFLAGS)   -L./ssh/shared -Wl,--rpath-link $(PWD)/ssh/shared  -Wl,--rpath $(PWD)/ssh/shared


clean:
	$(RM) $(EXEC) *.o *.so* *~ core  *.c~
	cd ./$(SSH_DIR) && $(MAKE)  clean
