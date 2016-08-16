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
	LIBS  += -lcurl

	CFLAGS += -I/home/mkn/_libssh/libssh/include -L./ssh/shared 

	VERSION := 4.4.0

	EXTRA=

SSH_DIR=ssh
HTTP_DIR=http

EXEC := ./convinject
PWD=$(shell pwd)

CFLAGS 	+= -Dname=process_ssh_target -Dname1=process_http_target
LDFLAGS +=
LIBS 	+= ./ssh/shared/libssh.so.$(VERSION)

all: $(EXEC)

SSH_OBJS	= ./$(SSH_DIR)/cmds.o ./$(SSH_DIR)/authentication.o ./$(SSH_DIR)/connect_ssh.o ./$(SSH_DIR)/knownhosts.o ./$(SSH_DIR)/pipes.o
HTTP_OBJS	= ./$(HTTP_DIR)/auxiliary.o   ./$(HTTP_DIR)/funcs.o  ./$(HTTP_DIR)/inject.o  ./$(HTTP_DIR)/lists.o  ./$(HTTP_DIR)/xmls.o  $(HTTP_DIR)/voc.obj 
SSH_OBJS_R	=$(SSH_OBJS)
HTTP_OBJS_R	=$(HTTP_OBJS)


ssh:	$(SSH_OBJS) 
	cd ./$(SSH_DIR) && $(MAKE) 
	@echo "SSH created:" && ls ./ssh/*.o
#TODO: obsolete	$(MAKE) "SSH_OBJS_R=$(SSH_OBJS)" all

HTTP_OBJS_R=$(HTTP_OBJS)

#http
$(HTTP_OBJS_R):	./$(HTTP_DIR)/Makefile 
	cd ./$(HTTP_DIR) && $(MAKE) build
	@echo "HTTP created:" && ls ./http/*.o


.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)  

$(EXEC): $(SSH_OBJS_R) $(HTTP_OBJS_R)       $(OBJS) $(LIBS)	
	$(CC)  -o $@ $(OBJS)  $(SSH_OBJS_R) $(HTTP_OBJS_R)   $(LIBS)  $(LDFLAGS)    -L/usr/local/lib  \
	    -Wl,--rpath /usr/lib/i386-linux-gnu   -Wl,--rpath $(PWD)/ssh/shared

#-Wl,--rpath-link $(PWD)/ssh/shared
#  -L./ssh/shared
# -Wl,--rpath $(PWD)/ssh/shared
# -Wl,--rpath $(PWD)/ssh/shared


clean:
	$(RM) -fv $(EXEC) *.o *.so* *~ core  *.c~
	cd ./$(SSH_DIR) && $(MAKE)  clean
	cd ./$(HTTP_DIR) && $(MAKE)  clean
