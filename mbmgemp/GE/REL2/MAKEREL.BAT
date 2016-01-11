del mbm*.*
del ge*.*
del upd*.*
del *.bak

copy \bbsv6\mbmg2.dll
copy \bbsv6\mbmg2msg.msg
copy \bbsv6\mbmg2hlp.msg
copy \bbsv6\mbmg2.mdf

copy \bbsv6\mbmg2mnu.txt

copy ..\vir60\mbm*.dat *.new
copy ..\vir60\mbm*.dat 

ren mbmge*.dat mbmg2*.dat
ren mbmge*.new mbmg2*.new

touch *.*

Pkzip mbmg2dll.zip *.*
pkzip -d mbmg2dll.zip makerel.bat

copy mbmg2dll.zip updg2dll.zip
pkzip -d updg2dll.zip *.dat

ren mbmg2dll.zip mbmg2%1.zip
ren updg2dll.zip updg2%1.zip


