; asmsyntax=nasm
;*****************************************************************************
;* CS244: Assembler implementation of external functions defined in          *
;*        'tut4.h'.                                                          *
;*****************************************************************************

global fact
global power
global ackerman
global swap
global binary_search

;*****************************************************************************
;* int fact(int n);                                                          *
;*****************************************************************************
%define n [ebp+8]
fact:
  push 	ebp
  mov	ebp, esp

  mov 	eax, 1
  mov	ecx, 0
.fac:
  inc	ecx
  imul	eax, ecx
  cmp	ecx, n
  jne	.fac

  mov	esp, ebp
  pop	ebp
  ret
;*****************************************************************************
;* int power(int x, int y);                                                  *
;*****************************************************************************
%define y [ebp+12]
%define x [ebp+8]
power:
  push 	ebp
  mov	ebp, esp

  mov	ecx, y
  mov	eax, 1
.pow:
  cmp	ecx, 0
  je	.end
  imul	eax, x
  dec	ecx
  jmp	.pow
.end:

  mov	esp, ebp
  pop	ebp
  ret
;*****************************************************************************
;* int ackerman(int x, int y);                                               *
;*****************************************************************************
%define y [ebp+12]
%define x [ebp+8]
ackerman:
  push 	ebp
  mov	ebp, esp

  mov	edx, x
  mov	ecx, y

  cmp	edx, 0
  jne	.if1
  inc	ecx
  mov	eax, ecx
  jmp	.end
.if1:
  cmp	ecx, 0
  jne	.if2
  dec	edx
  push	dword 1
  push	edx
  call	ackerman
  add	esp, 8
  jmp	.end
.if2:
  dec	ecx
  push	edx
  push	ecx
  push	edx
  call	ackerman
  add	esp, 8
  pop	edx
  push	eax
  dec	edx
  push	edx
  call 	ackerman
  add	esp, 8
.end:
  mov	esp, ebp
  pop	ebp
  ret
;*****************************************************************************
;* void swap(int *x, int *y);                                                *
;*****************************************************************************
%define x_ptr [ebp+8]
%define y_ptr [ebp+12]
swap:
  push  ebp
  mov   ebp, esp
  
  mov   edi, x_ptr
  mov   esi, y_ptr
  
  mov   eax, [edi]
  xchg  eax, [esi]
  xchg  eax, [edi]
  
  mov   esp, ebp
  pop   ebp
  ret
  
;*****************************************************************************
;* int binary_search(int n, int list[], int low, int high);                *
;*****************************************************************************
%define high [ebp+20]
%define	low	 [ebp+16]
%define list [ebp+12]
%define n	 [ebp+8]
binary_search:
  push	ebp
  mov	ebp, esp

  mov	ecx, low
  mov	ebx, n
  mov	edx, high
  mov	edi, list

  cmp	ecx, edx
  jle	.start	
  mov	eax, -1
  jmp	.end

.start:
  mov	eax, ecx
  add	eax, edx 
  shr	eax, 1

  lea	esi, [edi + eax * 4]
  cmp	ebx, [esi]
  jne	.less
  jmp	.end

.less:  
  jge	.great
  dec	eax
  mov	edx, eax
  jmp 	.func

.great:
  inc 	eax
  mov	ecx, eax
  jmp	.func

.func:
  push	edx
  push  ecx
  push	edi
  push	ebx
  call 	binary_search
  add	esp, 16

.end:
  mov	esp, ebp
  pop 	ebp
  ret

