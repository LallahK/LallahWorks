; asmsyntax=nasm

%include "constants.inc"

;************************************************************
section .bss
;************************************************************

;************************************************************
section .data
;************************************************************

fErrMsg		db	"bfi: could not open "
fErrMsgLen	equ $-fErrMsg
brackMsg	db	"bfi: unbalanced brackets", 10
brackMsgLen	equ $-brackMsg
lineFeed	db	LF
c			times 10240 db 0
inp			times 10240 db 0

;************************************************************
section .text
;************************************************************
global _start

%define fname		[ebp+12]
%define	progname	[ebp+8]
%define argc		[ebp+4]
_start:
  push	ebp
  mov	ebp, esp

  cmp	dword argc, 1
  je	.noFile

.openFile:
  mov	ebx, fname
  mov	eax, SYS_OPEN
  mov	ecx, O_RDONLY
  int	80h

  cmp	eax, 0
  jl	.fileError

  mov	eax, SYS_READ
  mov	ebx, eax
  mov	ecx, inp
  mov	edx, 10240
  int	80h

  ;mov	edi, inp
  ;call	strLen
  ;mov	eax, SYS_WRITE
  ;mov	ebx, STDERR
  ;mov	ecx, inp 
  ;int	80h

  jmp	.fileExit

.fileError:
  mov	eax, SYS_WRITE
  mov	ebx, STDOUT
  mov	ecx, fErrMsg
  mov	edx, fErrMsgLen
  int 	80h

  mov	edi, fname
  mov	ecx, fname
  call	strLen
  mov	eax, SYS_WRITE
  mov	ebx, STDOUT
  int	80h

  mov	eax, SYS_WRITE
  mov	ebx, STDOUT
  mov	ecx, lineFeed
  mov	edx, 1
  int 	80h
  
  jmp	.exit

.noFile:
  call	input

  jmp	.fileExit

.fileExit:
  mov	eax, SYS_CLOSE
  int	80h
.bf:
  cld
  call	brain

.exit:
  mov	esp, ebp
  pop	ebp

  mov	eax, SYS_EXIT
  xor	ebx, ebx
  int	80h

;************************************************************
; brain: actually interprets and executes the program
; BF Index (char, UTF8): (>, 62), (<, 60), (+, 43), (-, 45),
;					     (., 46), (,, 44), ([, 91), (], 93)
;************************************************************
%define ptr		 [ebp-4]
%define	pc		 [ebp-8]
%define	ignore	 [ebp-12]
%define while	 [ebp-16]
brain:
  push	ebp
  mov	ebp, esp
  sub	esp, 16

  mov	dword pc, dword inp
  mov	dword ptr, dword c
  mov	ignore, dword 0
.start:
  mov	edi, pc
  movzx	ecx, byte [edi]
  cmp	ecx, 0
  je	.end
  cmp	ecx, 93
  je	.ewhile
  cmp	ecx, 91
  je	.swhile

  mov	edx, ignore
  cmp	edx, 0
  jne	.nextChar

  cmp	ecx, 62
  je	.incPtr
  cmp	ecx, 60
  je	.decPtr
  cmp	ecx, 43
  je	.inc
  cmp	ecx, 45
  je	.dec
  cmp	ecx, 46
  je	.put
  cmp	ecx, 44
  je	.get

  jmp	.nextChar
.incPtr:							;++ptr
  inc	dword ptr
  jmp	.nextChar
.decPtr:							;--ptr
  dec	dword ptr
  jmp	.nextChar
.inc:								;++*ptr
  mov	edi, ptr
  mov	eax, [edi]
  inc	eax
  stosb
  jmp	.nextChar
.dec:								;--*ptr
  mov	edi, ptr
  mov	eax, [edi]
  dec	eax
  stosb
  jmp	.nextChar
.put:								;printf("%c", *ptr)
  mov   eax, SYS_WRITE
  mov   ebx, STDOUT
  mov	ecx, ptr
  mov	edx, 1
  int   80h
  jmp	.nextChar
.get:								;fgetc(stdin)
  mov	eax, SYS_READ
  mov	ebx, STDIN
  mov	ecx, ptr
  mov	edx, 1
  int	80h
  jmp	.nextChar
 
.swhile:							;while (*ptr) {
  mov	ecx, ignore
  cmp	ecx, 0
  jne	.dontswhile
  mov	edi, ptr
  movzx	eax, byte [edi]
  cmp	eax, 0
  je	.dontswhile
  mov	ebx, pc
  push	ebx
  mov	while, dword 1
  jmp	.nextChar
.dontswhile:
  inc	dword ignore
  jmp	.nextChar

.ewhile:							;while (*ptr)
  mov	ecx, ignore
  cmp	ecx, 0
  jne	.notwhile
  pop	ebx
  mov	pc, ebx
  mov	while, dword 0
  jmp	.start
.notwhile:
  dec dword ignore

.nextChar:  
  mov	eax, pc
  inc	eax
  mov	pc, eax
  jmp	.start

.end:
  mov	eax, while
  cmp	eax, 0
  je	.ret
  
  mov	eax, SYS_WRITE
  mov	ebx, STDOUT
  mov	ecx, brackMsg
  mov	edx, brackMsgLen
  int 	80h

.ret:
  mov	esp, ebp
  pop	ebp
  ret
;************************************************************
; strLen: Finds the length of a string
;************************************************************
strLen:
  push	ecx

  xor	ecx, ecx
  not	ecx
  xor	eax, eax
  cld
  repne	scasb
  neg	ecx
  lea	edx, [ecx-2]

  pop	ecx
  ret

;************************************************************
; inputFile: Allows the user to enter filename from STDIN
;************************************************************
input:
  push	ebp
  mov	ebp, esp

  mov	eax, SYS_READ
  mov	ebx, STDIN
  mov	ecx, inp
  mov	edx, 10240
  int	80h

  mov	esp, ebp
  pop	ebp
  ret
