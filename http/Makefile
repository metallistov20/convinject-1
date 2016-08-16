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

#gen_vocabulary.o  gen_voHabulary.o  
OBJS	= auxiliary.o   funcs.o    inject.o  lists.o  xmls.o  

CFLAGS = -c       -I/usr/include/libxml2                                                         -g 

CFLAGS += 

LDFLAGS = -L/usr/local/lib 

LIBS = -lcurl 	-lxml2




all: build

#TODO: not sure that <../ssh/cmds.o> should be deployed this way, but for now we need it anyway
build:	compute_tabs gen_vocabulary gen_voHabulary voc.c voc.h voc.obj    ../ssh/cmds.o   $(OBJS) 


gen_vocabulary: gen_vocabulary.c
	$(CC) gen_vocabulary.c -o gen_vocabulary

gen_voHabulary: gen_voHabulary.c
	$(CC) gen_voHabulary.c -o gen_voHabulary    

voc.c:	gen_vocabulary
	./gen_vocabulary >voc.c

voc.h:	gen_voHabulary
	./gen_voHabulary >voc.h

voc.obj: voc.c 
	$(CC) -o voc.obj -c voc.c  $(CFLAGS) $(LDFLAGS)

compute_tabs: compute_tabs.c
	$(CC) compute_tabs.c -o compute_tabs

inject: inject.o funcs.o lists.o xmls.o auxiliary.o voc.obj 
	$(CC)  -o inject inject.o funcs.o lists.o xmls.o auxiliary.o voc.obj  ../ssh/cmds.o $(LDFLAGS) $(LIBS)
#TODO: not sure that <../ssh/cmds.o> should be deployed this way, but for now we need it anyway



.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)  


clean:
	rm -fv voc.c voc.h inject gen_vocabulary gen_voHabulary compute_tabs config.log config.status configure  *.in~ *.sh~ *.c~ *.h~ *.xml~ *.o *.log *.obj *~
	rm -rfv autom4te.cache


