##PROJECT NAME:		convinject v.1.0

#### PROJECT PURPOSE:	SSH and HTTPS traffic injections to automate remote setting of L2/L3 network hardware.

# Contents of this folder:


> ##Build Files.  Current Folder ##

**prereqs.linux**   prerequisites download & build for Linux

**prereqs.cygwin**   prerequisites download & build for Cygwin x86_x32

**prereqs.cygwin.x86_64**   prerequisites download & build for Cygwin x86_x64

**Makefile.linux**   makefile for Linux

**Makefile.cygwin**   makefile for CygWIN

> ##Program Files.  Current Folder ##

**main.c**   main routine of prj.

**main.h**   headers of prj's main routine

**structs.c**   routines to process abstract structures

**structs.h**   type definition of these and routines' headers

**xmparser.c**   routines to parse text formated as XML

**xmparser.h**   declarations of above routines


> ##Program Files. Folder **./ssh** ##

**ssh/Makefile**   makefile to process contyents of folder **ssh**

**ssh/authentication.c**   authentication routine of **ssh** module

**ssh/cmds.c**   commands as strcutures

**ssh/cmds.h**   .. and their definitions, declarations

**ssh/config.h**   necessary declarations

**ssh/connect_ssh.c**   routine to establish and maintain **ssh** connection

**ssh/knownhosts.c**   routines to pass authentication according to **ssh** (v2)

**ssh/ssh_common.h**   basica definition, declarationn of **ssh** module

**ssh/ssh_main.c**   main routine of **ssh** module

> ##Program Files. Folder **./http** ##

**http/Makefile**   makefile to process contyents of folder <./http> 

**http/http_main.c**   main routine of http module

**http/http_main.h**   .. its headers, definitions, macros

**http/lists.c**   injections of  **http** traffic as reconrd of corresponding structures

**http/lists.h**   .. and the haders of then (btw, definitions - too)

**http/funcs.c**   functions and metafunctions - implementattion

**http/funcs.h**   .. their declarations

**http/opcodes.h**   opcades for commands

**http/verbose.h**   switches to toggle between levels of verbosity across **ssh** module (not entire project)

**http/auxiliary.c**   auxiliary functions

**http/auxiliary.h**   .. their declarations

**http/compute_tabs.c**   a small routine to compute tabs in XML-formatted text, needed for generation of CAST files (such as **cast.5428E.xml**, see below)

**http/constants.h**   few basic definitions of **ssh** module 

**http/gen_vocabulary.c**   autogenrerator of **voc.c** file

**http/gen_voHabulary.c**   autogenrerator of **voc.h** file

**http/xmls.c**   routine to process XML-formatted text

**http/xmls.h**   .. and its declarations, definitions

> ##Scenario Files.  Current Folder ##

**targets.xml**   top-level scenario file needed to lanch the porject from command line, e.g. <./convinject ./targets.xml> 

**http_commands.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <http> proto

**description_change.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <ssh> proto

**switch_192_168_0_1.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <ssh> proto

**switch_192_168_0_3.txt**   bottom-level scenario file referred by file <./targets.xml> within section with <ssh> proto

**basic_commands.txt**   another instance of bottom-level scenario file

**ip_change.txt**   another instance of bottom-level scenario file

**local_data.txt**   another instance of bottom-level scenario file

> ##Auxiliary Files.  Current Folder ##

**cast.2218.txt.voc**   auxiliary templat file

**cast.5428E.txt.voc**   auxiliary templat file

**cast.5428E.xml**   cast file for HTTPS-conversation with 5428E target

**README.md**   this file


