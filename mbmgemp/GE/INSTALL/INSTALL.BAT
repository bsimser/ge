@echo off
cls
Echo º Galactic Empire 3.0 Installation Utility
echo º (c) Copyright 1993 M.B. Murdock & Assoc.
echo º

if "#%1#" == "##" goto noopt1
if "#%2#" == "##" goto noopt2
copy %1mbmge*.zip %2 >nul
%2\
cd %2

if exist mbmgemp.dll goto notstrt
if exist mbmgemsg.msg goto notstrt
if exist mbmgehlp.msg goto notstrt
if exist mbmgemp.doc goto notstrt
if exist mbmgemp.mdf goto notstrt
if exist mbmgemnu.txt goto notstrt

if exist mbmgeshp.new goto notstrt
if exist mbmgeusr.new goto notstrt
if exist mbmgeplt.new goto notstrt
if exist mbmgemal.new goto notstrt

if exist mbmgeshp.dat goto notstrt
if exist mbmgeusr.dat goto notstrt
if exist mbmgeplt.dat goto notstrt
if exist mbmgemal.dat goto notstrt

%1pkunzip %2\mbmge*.zip
del %2\mbmge*.mcv >nul

if not exist mbmgemp.dll goto notgood
if not exist mbmgemsg.msg goto notgood
if not exist mbmgehlp.msg goto notgood
if not exist mbmgemp.doc goto notgood
if not exist mbmgemp.mdf goto notgood
if not exist mbmgemnu.txt goto notgood

if not exist geinst.doc goto notgood
if not exist gereadme.doc goto notgood
if not exist order.frm goto notgood

if not exist mbmgeshp.new goto notgood
if not exist mbmgeusr.new goto notgood
if not exist mbmgeplt.new goto notgood
if not exist mbmgemal.new goto notgood

if not exist mbmgeshp.dat goto notgood
if not exist mbmgeusr.dat goto notgood
if not exist mbmgeplt.dat goto notgood
if not exist mbmgemal.dat goto notgood

echo º Installation Complete! Please install Galactic Empire on your menu
echo º according to the procedures in the MAJORBBS OPERATIONS GUIDE.
goto end

:notstrt
echo º *Error* Files already exist. If you are performing a complete install
echo º         and wish to reset the databases type "DEL MBMGE*.*" and press
echo º         return. Then repeat the installation procedure. If you do not
echo º         wish to install virgin (empty) databases please refer to the
echo º         Update procedure in the Galactic Empire Operations Guide.
goto end

:notgood
echo º         Installation Error. Please check! Call (813) 545-8050
echo º         for customer help if problem cannot be resolved.
goto end


:notgood
echo º *Error* File Installation Error. Please check! Call (813) 545-8050
echo º         for customer help if problem cannot be resolved.
goto end


:noopt1
echo º
Echo º *Error* No source path specified.
echo º
goto thisway
:noopt2
echo º
Echo º *Error* No destination path specified.
echo º
goto thisway

:thisway
echo º
echo º Usage: Install [source drive] [dest path]
Echo º
echo º Example:  Install A: C:\BBSV6
echo º
goto end
:end
