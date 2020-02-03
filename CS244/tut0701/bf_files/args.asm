; asmsyntax=nasm

%include "constants.inc"

;-------------------------------------------------------------------------------
section .data
;-------------------------------------------------------------------------------
fopenMsg 	db 	"save: could not open "
fopenMsgLen equ $-fopenMsg
usageMsg 	db 	"usage: save <filename> [words...]", LF
usageMsgLen equ $-usageMsg
lineFeed 	db 	LF

;-------------------------------------------------------------------------------
section .text
;-------------------------------------------------------------------------------
global _start ; "export" the _start symbol for the linker

;-------------------------------------------------------------------------------
%define argv 		[ebp + 4*ecx]
%define fname 		[ebp + 12]
%define progname 	[ebp + 8]
%define argc 		[ebp + 4]
_start:
  push 	ebp 		; preserve base pointer
  mov 	ebp, esp 	; set base pointer to stack top

; check the number of command-line arguments
  cmp 	dword argc, 1
  je 	.usage


; open file for writing
  mov 	eax, SYS_OPEN
  mov 	ebx, fname
  mov 	ecx, O_CREAT | O_TRUNC | O_WRONLY
  mov 	edx, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
  int 	80h

; check that file was opened successfully
  cmp 	eax, 0
  jl 	.fopenerr
  mov 	ebx, eax

; loop over the command-line arguments
  mov 	ecx, 4

.next_arg:
  mov 	edi, dword argv
  test 	edi, edi  		; test for a NULL pointer; if it is
  jz 	.exit 			; 	then we are at end of argv arr

  call 	strlen 	; gets str address via edi, returns length via edx
  call 	fwrite 	; gets file descriptor via ebx, argv counter via ecx.
  				; and string length via edx
  inc 	ecx
  jmp  	.next_arg

; report error while attempting to open file, and exit
.fopenerr:
  mov 	eax, SYS_WRITE
  mov 	ebx, STDERR
  mov 	ecx, fopenMsg
  mov 	edx, fopenMsgLen
  int 	80h
  
  mov 	edi, fname
  call 	strlen
  mov 	eax, SYS_WRITE
  mov 	ebx, STDERR
  mov 	ecx, fname
  int 	80h
 
  mov 	eax, SYS_WRITE
  mov 	ebx, STDERR
  mov 	ecx, lineFeed
  mov 	edx, 1
  int 	80h
  
  jmp 	.errexit
  
; print usage message, and exit
.usage:
  mov 	eax, SYS_WRITE
  mov 	ebx, STDERR
  mov 	ecx, usageMsg
  mov 	edx, usageMsgLen
  int 	80h

; exit with non-zero exit code
.errexit:
  mov 	esp, ebp
  pop 	ebp

  mov 	eax, SYS_EXIT
  mov 	ebx, 1
  int 	80h

; close the open file , and exit with zero exit code
.exit:
  mov 	eax, SYS_CLOSE
  int 	80h
 
  mov 	esp, ebp
  pop  	ebp
 
  mov 	eax, SYS_EXIT
  xor 	ebx, ebx
  int  	80h

;-------------------------------------------------------------------------------
; strlen : returns the string length of the string pointed to by EDI in EDX
;-------------------------------------------------------------------------------
strlen:
  push 	ecx 		; preserve ecx

  xor 	ecx, ecx 	; clear ecx
  not 	ecx 		; ECX <- -1; one 's complement negation ( flip bits )'
  xor 	eax, eax 	; clear eax
  cld 				; clear D flag , to make EDI increment automatically
  repne scasb 		; repeat SCASB while not equal to AL , decrementing ECX
  neg 	ecx 		; ECX <- - ECX
  lea 	edx, [ecx-2]; EDX <- ECX - 2
  
  pop 	ecx 		; restore ecx
  ret

;-------------------------------------------------------------------------------
; fwrite : writes string at counter ECX of length in EDX to the file descriptor
; in ebx
;-------------------------------------------------------------------------------
fwrite:
  push 	ecx 			; preserve ecx

  mov 	ecx, dword argv
  mov 	eax, SYS_WRITE
  int 	80h

  mov 	eax, SYS_WRITE
  mov 	ecx, lineFeed
  mov 	edx, 1
  int	80h

  pop 	ecx 			; restore ecx
  ret
