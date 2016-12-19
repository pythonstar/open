@ECHO OFF

REM Unregistering ActiveX/COM server

regsvr32 /u %windir%\system32\SWFToImage.dll

ECHO IMPORTANT: 
ECHO Now please delete SWFToImage.dll from \Windows\system32\ folder manually
ECHO 
pause