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

OBJS	= ./authentication.o ./cmds.o ./connect_ssh.o ./knownhosts.o ./pipes.o 

CFLAGS = -c -D_DBG                                                                -g 

CFLAGS += -I/home/mkn/_libssh/libssh/include -L./ssh/shared 

all: $(OBJS)

.o: .c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)  

clean:
	$(RM) -fv  *.o *.so* *~ core  *.c~
