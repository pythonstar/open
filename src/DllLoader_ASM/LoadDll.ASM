.386
.model flat,stdcall
option casemap:none

include windows.inc
include user32.inc
include kernel32.inc
include shell32.inc

includelib user32.lib
includelib kernel32.lib
includelib shell32.lib



		.data?
lpCmdLine       dd      ?
nArgs		dd	?

		.data
szCaption	db	'load dll--by sing',0
szText1		db	'OK!',0
szText2		db	'drag your dll on me!',0

		.code
start:
		invoke GetCommandLineW
		invoke CommandLineToArgvW,eax,addr nArgs
		.if nArgs>=2 
			mov edx,[eax+4]
			invoke LoadLibraryW,edx
			invoke MessageBox,0,addr szText1,addr szCaption,0
		.else
			invoke MessageBox,0,addr szText2,addr szCaption,0
		.endif
		invoke	ExitProcess,NULL
		end	start
