CC := gcc

CFLAGS = -c -g -I/usr/include/libxml2      -I./include  
	
EXEC := ./convinject
PWD=$(shell pwd)

CFLAGS 	+= -Dname=process_ssh_target -Dname1=process_http_target   -mpreferred-stack-boundary=4 -mstackrealign

LIBS 	+= ./shared/libssh.so
LIBS 	+= ./shared/libcurl.so
LIBS 	+= ./shared/libxml2.so

all: $(EXEC)

OBJS		= main.o xmparser.o structs.o 
SSH_OBJS	= ssh/cmds.o ssh/authentication.o ssh/connect_ssh.o ssh/knownhosts.o ssh/ssh_main.o
HTTP_OBJS	= http/voc.obj http/auxiliary.o   http/funcs.o  http/http_main.o  http/lists.o  http/xmls.o  

.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)  

http/gen_vocabulary: http/gen_vocabulary.c
	$(CC) http/gen_vocabulary.c -o http/gen_vocabulary

http/gen_voHabulary: http/gen_voHabulary.c
	$(CC) http/gen_voHabulary.c -o http/gen_voHabulary    

http/voc.c:	http/gen_vocabulary
	http/gen_vocabulary >http/voc.c

http/voc.h:	http/gen_voHabulary
	http/gen_voHabulary >http/voc.h

http/voc.obj: http/voc.c http/voc.h  
	$(CC) -o http/voc.obj -c http/voc.c  $(CFLAGS) $(LDFLAGS)

http/compute_tabs: http/compute_tabs.c
	$(CC) http/compute_tabs.c -o http/compute_tabs



$(EXEC): $(SSH_OBJS)  $(HTTP_OBJS)  $(OBJS)        $(LIBS)		
	$(CC)  -o $@   $(SSH_OBJS) $(HTTP_OBJS) $(OBJS)   $(LIBS)      \
 -Wl,--rpath-link $(PWD)/shared  -Wl,--rpath $(PWD)/shared 


clean:
	$(RM) -fv $(EXEC) *.o *.so* *~ core  *.c~
	cd ./ssh/ && $(MAKE)  clean
	cd ./http/ && $(MAKE)  clean
