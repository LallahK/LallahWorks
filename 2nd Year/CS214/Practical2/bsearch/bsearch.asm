global binary_search

%define n			 [ebp+8]
%define list		 [ebp+12]
%define low			 [ebp+16]
%define high		 [ebp+20]
binary_search:
  push	ebp
  mov	ebp, esp

.search:
  mov	eax, low
  mov	ecx, high
  cmp	eax, ecx
  jge	.notFound

  add	eax, ecx
  shr	eax, 1
  mov	edx, eax
  mov	edi, list
  mov	ecx, [edi + eax * 4]
  mov	ebx, n
  cmp	ecx, ebx
  je	.found
  jl	.greater
  jg	.less
.greater:
  inc	edx
  mov	low, edx

  jmp	.search
.less:
  dec	edx
  mov	high, edx

  jmp	.search
.notFound:
  mov	eax, dword 0
.found:
  mov	eax, dword 1
.end:
  mov	esp, ebp
  pop	ebp

