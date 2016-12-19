REM copying files to the system folder

COPY SWFToImage.dll %windir%\system32

REM registering ActiveX/COM server
regsvr32 %windir%\system32\SWFToImage.dll