del mbm*.*
del ge*.*
del upd*.*
del *.bak
del *.zip

copy \bbsv6\mbmgemp.dll
copy \bbsv6\mbmgemsg.msg
copy \bbsv6\mbmgehlp.msg
copy \bbsv6\mbmgeshp.msg
copy \bbsv6\mbmgemp.mdf
copy \bbsv6\mbmgeshp.msg

copy \bbsv6\mbmgegrf.gif
copy \bbsv6\mbmgegr1.gif

copy \bbsv6\mbmgemnu.txt
rem copy \bbsv6\mbmgecv.exe
rem copy \bbsv6\mbmgecvt.bat

copy ..\vir60\mbm*.dat *.new
copy ..\vir60\mbm*.dat 

copy ..\docs\mbmgemp.doc
copy ..\docs\geinst.doc
copy ..\docs\gereadme.doc
copy ..\docs\order.frm


touch *.*

Pkzip mbmgedll.zip *.*
pkzip -d mbmgedll.zip makerel.bat

copy mbmgedll.zip updgedll.zip
pkzip -d updgedll.zip *.dat

pkzip patgedll.zip mbmgemp.dll gereadme.doc mbmgemnu.txt mbmgemsg.msg

ren mbmgedll.zip mbmge%1.zip
ren updgedll.zip updge%1.zip
ren patgedll.zip patge%1.zip



