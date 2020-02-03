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
  push	0
  push	c
  push	inp
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
%define	depth	 [ebp+16]
%define	pptr	 [ebp+12]
%define ppc	 	 [ebp+8]
%define ptr		 [ebp-8]
%define	pc		 [ebp-12]
%define while	 [ebp-16]
brain:
  push	ebp
  mov	ebp, esp

  mov	edi, dword pptr
  mov	ptr, edi
.wStart:
  mov	edi, dword ppc
  mov	pc, edi

.start:
  mov	edi, pc
  movzx	ecx, byte [edi]
  cmp	ecx, 0
  je	.end
  cmp	ecx, 93
  je	.ewhile

  mov	edx, while
  cmp	edx, 1
  je	.nextChar

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
  cmp	ecx, 91
  je	.swhile
  jmp	.nextChar
.incPtr:							;++ptr
  mov	edi, ptr
  inc	edi
  mov	ptr, edi
  jmp	.nextChar
.decPtr:							;--ptr
  mov	edi, ptr
  dec	edi
  mov	ptr, edi
  jmp	.nextChar
.inc:								;++*ptr
  mov	edi, ptr
  mov	ecx, [edi]
  inc	ecx
  mov	[edi], ecx
  jmp	.nextChar
.dec:								;--*ptr
  mov	edi, ptr
  mov	ecx, [edi]
  dec	ecx
  mov	[edi], ecx
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
  add   pc, byte 1
  mov	edi, ptr
  movzx	ecx, byte [edi]
  cmp	ecx, 0
  jne	.makewhile
  mov	while, dword 1
  jmp	.nextChar

.makewhile
  mov	ecx, depth
  inc	ecx
  mov	esi, pc
  push	ecx
  push	edi
  push	esi

  call	brain

  mov	while, dword 0
  mov	ptr, eax
  mov	pc, edi

  add	esp, 12

  jmp	.nextChar
.ewhile:							;while (*ptr)
  mov	ecx, while
  cmp	ecx, 1
  jne	.endwhile
  mov	while, dword 0
  jmp	.nextChar
.endwhile:
  mov	edi, ptr
  movzx	ecx, byte [edi]
  cmp	ecx, 0
  jne	.wStart

  mov	eax, ptr
  mov	edi, pc

  mov	esp, ebp
  pop	ebp
  ret
.nextChar:  
  mov	edi, pc
  inc	edi
  mov	pc, edi
  jmp	.start
.end:
  mov	eax, depth
  cmp	eax, 0
  je	.ret
  
  mov	eax, SYS_WRITE
  mov	ebx, STDOUT
  mov	ecx, brackMsg
  mov	edx, brackMsgLen
  int 	80h

  mov	eax, SYS_EXIT
  xor	ebx, ebx
  int	80h

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

  ;mov	edi, inp
  ;call	strLen
  ;mov	eax, SYS_WRITE
  ;mov	ebx, STDERR
  ;mov	ecx, inp 
  ;int	80h

  mov	esp, ebp
  pop	ebp
  ret
