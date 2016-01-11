###############################################################################
##### This Makefile generates the "MAJORBBS.EXE" and all standard DLL's   #####
###############################################################################

.autodepend
.silent
.path.c=\bbsv6\src
.path.h=\bbsv6\src
.path.dll=\bbsv6
.path.msg=\bbsv6
.path.obj=\bbsv6\phobj
.path.exe=\bbsv6


###############################################################################
#####                           Build DLL's                               #####
###############################################################################
mbmgemp.dll: \
     mbmgemsg.h    mbmgehlp.h                                         \
     gemain.obj     gefuncs.obj   gecmds.obj    gecybs.obj   geplanet.obj \
     gelib.obj \
     \bbsv6\dlib\galimp.lib        \bbsv6\dlib\gsblimp.lib                 \
     \run286\bc3\lib\phapi.lib     \bbsv6\dlib\nodef.def
     ltdll mbmgemp


###############################################################################
#####                 Build Objects For FASTPACE DLL                      #####
###############################################################################

gemain.obj: gemain.c
     ctdll gemain
gefuncs.obj: gefuncs.c
     ctdll gefuncs
gecmds.obj: gecmds.c
     ctdll gecmds
gecybs.obj: gecybs.c
     ctdll gecybs
geplanet.obj: geplanet.c
     ctdll geplanet
gelib.obj: gelib.c
     ctdll gelib

###############################################################################
#####               Generate Headers for Compilation                      #####
###############################################################################

mbmgemsg.h: mbmgemsg.msg
     cd \bbsv6
     bbsmsx mbmgemsg -osrc
     cd \bbsv6\src
mbmgehlp.h: mbmgehlp.msg
     cd \bbsv6
     bbsmsx mbmgehlp -osrc
     cd \bbsv6\src



