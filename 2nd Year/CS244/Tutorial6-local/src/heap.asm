; asmsyntax=nasm
;*****************************************************************************
;* CS244: Heap Implementation												 *
;* tut6 - 21865728 - Abdullah Karbanee										 *
;*****************************************************************************

global heap_initialize
global heap_insert
global heap_remove

;*****************************************************************************
;* void heap_intialize(Heap *H)												 *
;*****************************************************************************
%define	Heap	[ebp+8]
heap_initialize:
  push 	ebp
  mov	ebp, esp

  mov	edi, Heap
  mov	eax, 0
  cld
  stosd
  mov	ecx, 1024
  rep	stosd

  mov	esp, ebp
  pop 	ebp
  ret
;*****************************************************************************
;* void heap_insert(Heap *H, HeapNode *node)								 *
;*****************************************************************************
%define H	 	 [ebp+8]
%define node 	 [ebp+12]
%define c		 [ebp-4]
%define	p 		 [ebp-8]
%define n		 [ebp-12]
heap_insert:
  push 	ebp
  mov	ebp, esp

  mov	esi, H
  mov	eax, dword [esi]
  mov	n, eax						
  mov	c, eax

  mov	esi, H
  mov	eax, n
  imul	eax, 16
  add	eax, 4
  lea	edi, [esi + eax]
  mov	esi, node
  mov	ecx, 4
  cld
  rep	movsd

  mov	eax, c
  cmp	eax, 0
  jle	.cgo
  dec	eax
  shr	eax, 1
  mov	p, eax						
  jmp	.swhile
.cgo:
  mov	p, dword 0

.swhile:
  mov	edx, p
  cmp	edx, 0
  jl	.ewhile

  mov	esi, H
  mov	eax, p
  imul	eax, 16
  add	eax, 4
  lea	edi, [esi + eax]
  mov	ecx, dword [edi]
  mov	eax, c
  imul	eax, 16
  add	eax, 4
  mov	edx, dword [esi + eax]
  
  cmp	ecx, edx
  jle	.else
	
  mov	edi, H
  mov	eax, p
  imul	eax, 16
  add	eax, 4
  lea	ebx, [edi + eax]
  mov	eax, c
  imul	eax, 16
  add	eax, 4
  lea	edi, [edi + eax]
  mov	ecx, 4
.smove:
  cmp	ecx, 0
  je	.esmove
  mov	edx, dword [ebx]
  mov	esi, edi
  mov	eax, dword [esi]
  mov	edi, ebx
  stosd
  mov	ebx, edi
  mov	eax, edx
  mov	edi, esi
  stosd
  dec	ecx
  jmp	.smove
.esmove:
  
  mov	edx, p
  mov	c, edx
  mov	eax, c
  cmp	eax, 0
  jle	.cgo2
  dec	eax
  shr	eax, 1
  mov	p, eax						
  jmp	.swhile
.cgo2:
  mov	p, dword 0
  jmp	.swhile

.else:
  mov	p, dword -1
  jmp	.swhile
.ewhile:
  mov	eax, n
  inc	eax
  mov	edi, H
  stosd

  mov	esp, ebp
  pop 	ebp
  ret
;*****************************************************************************
;* void heap_remove(Heap *H, HeapNode *node)								 *
;*****************************************************************************
%define H			 [ebp+8]
%define node		 [ebp+12]
%define c			 [ebp-4]
%define p			 [ebp-8]
%define n			 [ebp-12]
heap_remove:
  push 	ebp
  mov	ebp, esp

  mov	esi, H
  lodsd
  cmp	eax, 0
  jl	.emainif

  mov	edi, node
  mov	esi, H
  lea	esi, [esi + 4]
  mov	ecx, 4
  cld
  rep	movsd
  
  mov	esi, H
  lea	edi, [esi + 4]
  mov	eax, dword [esi]
  dec	eax
  imul	eax, 16
  add	eax, 4
  lea	esi, [esi + eax]
  mov	ecx, 4
  rep	movsd
  
  mov	edi, H
  mov	eax, dword [edi]
  dec	eax
  stosd

  mov	p, dword 0
  mov	c, dword 1
.swhile:
  mov	edx, c
  mov	edi, H
  mov	ecx, dword [edi]
  dec	ecx
  cmp	edx, ecx
  jg	.emainif

  mov	edi, H
  mov	eax, c
  imul	eax, 16
  add	eax, 4
  lea	esi, [edi + eax]
  mov	ecx, dword [esi]
  mov	eax, c
  inc	eax
  imul	eax, 16
  add	eax, 4
  lea	esi, [edi + eax]
  mov	edx, dword [esi]
  cmp	ecx, edx
  jl	.eif1
  mov	edx, c
  inc	edx
  mov	c, edx
.eif1:
  mov	esi, H
  mov	eax, c
  imul	eax, 16
  add	eax, 4
  lea	edi, [esi + eax]
  mov	ecx, dword [edi]
  mov	eax, p
  imul	eax, 16
  add	eax, 4
  lea	edi, [esi + eax]
  mov	edx, dword [edi]
  
  cmp	ecx, edx
  jg	.else

  mov	edi, H
  mov	eax, p
  imul	eax, 16
  add	eax, 4
  lea	ebx, [edi + eax]
  mov	eax, c
  imul	eax, 16
  add	eax, 4
  lea	edi, [edi + eax]
  mov	ecx, 4
.smove:
  cmp	ecx, 0
  je	.esmove
  mov	edx, dword [ebx]
  mov	esi, edi
  mov	eax, dword [esi]
  mov	edi, ebx
  stosd
  mov	ebx, edi
  mov	eax, edx
  mov	edi, esi
  stosd
  dec	ecx
  jmp	.smove
.esmove:
	;end swap
  mov	ecx, c
  mov	p, ecx
  add	ecx, ecx
  inc	ecx
  mov	c, ecx
  jmp	.swhile
.else:
  mov	esi, H
  mov	ecx, dword [esi]
  mov	c, ecx
  jmp	.swhile
.emainif:

  mov	esp, ebp
  pop 	ebp
  ret
