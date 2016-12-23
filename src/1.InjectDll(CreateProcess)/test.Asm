.586
.model flat,stdcall
option casemap:none

include user32.inc
includelib user32.lib

.code

start:
	invoke MessageBox,0,0,0,0
	ret
end start
