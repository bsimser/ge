@echo off
cls
Echo º Galactic Empire 3.0 Make Disk Utility
echo º (c) Copyright 1993 M.B. Murdock & Assoc.
echo º

if "#%1#" == "##" goto noopt1

copy c:\pk\pkunzip.exe %1 >nul
copy ..\rel\mbmge*.zip %1 >nul
copy ..\rel\updge*.zip %1 >nul

copy install.bat %1 >nul
copy update.bat %1 >nul

if not exist %1updge*.zip goto notgood
if not exist %1mbmge*.zip goto notgood
if not exist %1pkunzip.exe goto notgood
if not exist %1install.bat goto notgood
if not exist %1update.bat goto notgood

echo º Disk Creation Complete!

goto end

:noopt1
echo º
Echo º *Error* No source path specified.
echo º
goto thisway

:notgood
echo º         Missing Files Error. Please check! 
goto end

:thisway
echo º
echo º Usage: Makedisk [drive]
Echo º
echo º Example:  makedisk A:
echo º
goto end
:end
