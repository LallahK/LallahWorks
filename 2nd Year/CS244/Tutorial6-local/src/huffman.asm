; asmsyntax=nasm
;*****************************************************************************
;* CS244: Huffman Tree						                                 *
;* tut6 - 21865728 - Abdullah Karbanee
;*****************************************************************************

extern heap_initialize
extern heap_insert
extern heap_remove
extern malloc

global huffman_build_tree
global huffman_initialize_table
global huffman_build_table

;*****************************************************************************
;*void huffman_build_tree(Heap *h, HeapNode **t)							 *
;*****************************************************************************
%define h			 [ebp+8]
%define t			 [ebp+12]
%define node1		 [ebp-4]
%define node2		 [ebp-8]
%define parent		 [ebp-12]
huffman_build_tree:
  push 	ebp
  mov	ebp, esp

.swhile:
  mov	esi, h
  lodsd
  cmp	eax, 0
  jle	.ewhile

  cmp	eax, 1
  jg	.else

  push	16
  call	malloc
  pop	edx
  mov	parent, eax
  push	dword parent
  push	dword h
  call	heap_remove
  mov	esi, t

  mov	edi, parent
  mov	[esi], edi

  jmp	.swhile
.else:
  push 	16
  call 	malloc
  pop	edx
  mov	edi, eax
  mov	node1, edi
  push	edi
  push	dword h
  call	heap_remove

  push 	16
  call	malloc
  pop	edx
  mov	esi, eax
  mov	node2, esi
  push	esi
  push	dword h
  call	heap_remove

  push	16
  call	malloc
  pop 	edx
  mov	parent, eax
  mov	edi, eax
  mov	esi, node1
  mov	eax, dword [esi]
  mov	esi, node2
  add	eax, dword [esi]
  stosd	
  mov	eax, 0
  stosd
  mov	eax, node1
  stosd
  mov	eax, node2
  stosd
  push	dword parent
  push	dword h
  call	heap_insert

  jmp	.swhile
.ewhile:

  mov	esp, ebp
  pop	ebp
  ret
;*****************************************************************************
;*void huffman_initialize_table(HuffmanNode *t)								 *
;*****************************************************************************
%define	t			 [ebp+8]
huffman_initialize_table:
  push 	ebp
  mov	ebp, esp

  mov	ecx, 2048
  mov	edi, t
  cld
  mov	eax, 255
  rep	stosb

  mov	esp, ebp
  pop	ebp
  ret
;*****************************************************************************
;*void huffman_build_table(HeapNode *root, HuffmanNode *t, int code, int len)*
;*****************************************************************************
%define root		 [ebp+8]
%define t			 [ebp+12]
%define code		 [ebp+16]
%define len			 [ebp+20]
huffman_build_table:
  push 	ebp
  mov	ebp, esp

  mov	esi, root
  mov	ebx, dword [esi + 4]
  cmp	ebx, 0
  jle	.jempty
  mov	edi, t
  lea	edi, [edi + ebx * 8]
  mov	eax, dword code
  cld
  stosd
  mov	eax, dword len
  stosd
.jempty:
  mov	ecx, len
  inc	ecx
  mov	eax, code
  shl	eax, 1
  mov	esi, root
  lea	esi, [esi + 8]
  mov	ebx, [esi]
  cmp	ebx, 0
  je	.notl
  push	ecx
  push	eax
  push	dword t
  push 	dword [esi]
  call	huffman_build_table
.notl:
  mov	ecx, len
  inc	ecx
  mov	eax, code
  shl	eax, 1
  inc	eax
  mov	esi, root
  lea	esi, [esi + 12]
  mov	ebx, [esi]
  cmp	ebx, 0
  je	.notr
  push	ecx
  push	eax
  push	dword t
  push	dword [esi]
  call	huffman_build_table  
.notr:

  mov	esp, ebp
  pop	ebp
  ret
