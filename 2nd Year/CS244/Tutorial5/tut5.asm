; asmsyntax=nasm
;*****************************************************************************
;* CS244: Assembler implementation of external functions defined in          *
;*        'tut5.h'.                                                          *
;*****************************************************************************

extern free

global binary_sort
global delete_node
global bin_to_string

;*****************************************************************************
;* void binary_sort(node *list, int n);                                      *
;*****************************************************************************
%define list   [ebp+8]
%define n      [ebp+12]
%define i	   [ebp-4]
%define j	   [ebp-8]
%define bottom [ebp-12]
%define top    [ebp-16]
%define middle [ebp-20]
%define	temp   [ebp-24]
binary_sort:
  push  ebp
  mov   ebp, esp
  sub	esp, 24

  mov 	edi, list
  cmp	edi, 0
  je	.end

  mov 	dword i, 1
  push 	36
  extern malloc
  call 	malloc
  mov	temp, eax
.smainwhile:
  mov 	eax, i
  mov	edx, n
  cmp	eax, edx
  jge	.end

	;start of copy to temp
  mov	eax, i
  imul	eax, 9
  mov	edi, list
  lea	esi, [edi + eax * 4]
  ;mov	eax, temp
  mov	edi, temp
  mov	ecx, 9
  cld
  rep	movsd
  	;end of copy to temp
  
  mov	dword bottom, 0
  mov	edx, i
  dec	edx
  mov	top, edx
.smiddlewhile:
  mov	eax, bottom
  mov	edx, top
  cmp	eax, edx
  jg	.emiddlewhile

  mov	eax, bottom
  mov	edx, top
  add	eax, edx
  shr	eax, 1
  mov	middle, eax
  	;start of if statement
  mov	esi, temp
  mov	ecx, dword [esi]
  mov	eax, middle
  imul	eax, 9
  mov	edi, list
  lea	esi, [edi + eax * 4]
  mov	edx, dword [esi]

  cmp	ecx, edx
  jge	.else
  	;end of if statement
  mov	eax, middle
  dec	eax
  mov	top, eax
  jmp	.smiddlewhile
.else:
  mov	eax, middle
  inc	eax
  mov	bottom, eax
  jmp	.smiddlewhile

.emiddlewhile:
  mov	edx, i
  dec	edx
  mov	j, edx

.smovwhile:
  mov	eax, j
  mov	edx, bottom
  cmp	eax, edx
  jl	.emovwhile
  	;start move while
  mov	edi, list
  imul	eax, 9
  lea	esi, [edi + eax * 4]
  add	eax, 9
  lea	edi, [edi + eax * 4]
  mov	ecx, 9
  cld
  rep 	movsd
  	;end move while
  mov	eax, j
  dec	eax
  mov	j, eax
  jmp	.smovwhile
.emovwhile:
	;start of copy back
  mov	eax, bottom
  mov	edi, list
  imul	eax, 9
  lea	edi, [edi + eax * 4]
  mov	esi, temp
  mov	ecx, 9
  cld
  rep	movsd
  	;end of copy back

  mov	edx, i
  inc	edx
  mov	i, edx
  jmp	.smainwhile
.end:

  mov   esp, ebp
  pop   ebp
  ret

;*****************************************************************************
;* void remove_max(node **root, node **max);                                 *
;*****************************************************************************
%define root [ebp+8]
%define node [ebp+12]
remove_max:
  push  ebp
  mov   ebp, esp
  
  mov   esp, ebp
  pop   ebp
  ret

;*****************************************************************************
;* void delete_node(node **root, char *name);                                *
;*****************************************************************************
%define root [ebp+8]
%define name [ebp+12]
delete_node:
  push  ebp
  mov   ebp, esp
  
  mov   esp, ebp
  pop   ebp
  ret  
  
;*****************************************************************************
;* void bin_to_string(int n, char *s);                                       *
;*****************************************************************************
%define n [ebp+8]
%define s [ebp+12]
bin_to_string:
  push  ebp
  mov   ebp, esp

  mov	ebx, n
  mov	eax, 48
  mov	edi, s
  mov	ecx, 32
  cld
  rep	stosb
  std
  mov	eax, 0
  stosb
.conversion:
  cmp	ebx, 0
  je	.end
  mov	ecx, ebx
  mov	eax, ebx
  shr	ecx, 1
  shl	ecx, 1
  sub	eax, ecx

  shr	ebx, 1

  add	eax, 48
  stosb
  jmp	.conversion
.end:

  mov   esp, ebp
  pop   ebp
  ret 
  
