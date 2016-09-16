##PROJECT NAME:		convinject v.1.0

#### PROJECT PURPOSE:	SSH and HTTPS traffic injections to automate remote setting of L2/L3 network hardware.

## Contents of this folder:


**prereqs.linux**   prerequisites download & build for Linux

**prereqs.cygwin**   prerequisites download & build for Cygwin x86_x32

**prereqs.cygwin.x86_64**   prerequisites download & build for Cygwin x86_x64

**Makefile.linux**   makefile for Linux

**Makefile.cygwin**   makefile for CygWIN



**main.c**   main routine of prj.

**main.h**   headers of prj's main routine

**structs.c**   routines to process abstract structures

**structs.h**   type definition of these and routines' headers

**xmparser.c**   routines to parse test formated as XML

**xmparser.h**   declarations of above routines



**ssh/Makefile**   makefile to process contyents of folder <ssh> 

**ssh/authentication.c**   

**ssh/cmds.c**   

**ssh/cmds.h**   

**ssh/config.h**   

**ssh/connect_ssh.c**   

**ssh/knownhosts.c**   

**ssh/ssh_common.h**   

**ssh/ssh_main.c**   



**http/Makefile**   makefile to process contyents of folder <http> 

**http/http_main.c**   

**http/http_main.h**   

**http/lists.c**   

**http/lists.h**   

**http/funcs.c**   

**http/funcs.h**   

**http/opcodes.h**   

**http/verbose.h**   

**http/auxiliary.c**   

**http/auxiliary.h**   

**http/compute_tabs.c**   

**http/constants.h**   

**http/gen_vocabulary.c**   

**http/gen_voHabulary.c**   

**http/xmls.c**   

**http/xmls.h**   


**targets.xml**   top-level scenario file needed to lanch the porject from command line, e.g. <./convinject ./targets.xml> 

**http_commands.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <http> proto

**description_change.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <ssh> proto

**switch_192_168_0_1.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <ssh> proto

**switch_192_168_0_3.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <ssh> proto

**basic_commands.txt**   another instance of bottom-level scenario file

**ip_change.txt**   another instance of bottom-level scenario file

**local_data.txt**   another instance of bottom-level scenario file



**cast.2218.txt.voc**   auxiliary templat file

**cast.5428E.txt.voc   auxiliary templat file

**cast.5428E.xml**   cast file for HTTPS-conversation with 5428E target

**README.md**   this file


