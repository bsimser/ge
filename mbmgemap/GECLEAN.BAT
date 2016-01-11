echo off
btrieve /p:2048 /e >nul

rem -------------------------------------------------------------------------
rem YOU MUST CHANGE THE NEXT 8 ZEROS FOR YOUR GALACTIC EMPIRE ACTIVATION CODE
rem -------------------------------------------------------------------------

mbmgemap -n00000000 
del mbmgeplt.zip
pkzip mbmgeplt mbmgeplt.map

rem -------------------------------------------------------------------------
rem CHANGE THE NAME GEMAPS TO WHATEVER FILE LIBRARY DIRECTORY YOU WISH TO 
rem LOAD THE ZIP'ED STARMAP FILE INTO FOR YOUR USERS TO DOWNLOAD
rem -------------------------------------------------------------------------

copy mbmgeplt.zip GEMAPS

