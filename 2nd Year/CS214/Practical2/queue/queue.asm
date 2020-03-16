;asmsyntax=nasm

;************************************************************
section .text
;************************************************************

global enqueue
global dequeue

;************************************************************
; enqueue(QueueNode **queue, void *elem)
;************************************************************
%define q			 [ebp+8]
%define elem		 [ebp+12]
enqueue:
  push 	ebp
  mov	ebp, esp

  cld
  mov	edi, q
  mov	edi, [edi]
  cmp	edi, dword 0
  jne	.notLast

  push	8
  extern malloc
  call	malloc
  pop	ecx
  
  mov	edi, q
  stosd
  mov	edi, q
  mov	edi, [edi]
  mov	edi, eax
  mov	eax, elem
  stosd
  jmp	.end
.notLast:
  lea	edi, [edi + 4]
  mov	esi, edi
  mov	edi, [edi]
  cmp	edi, dword 0
  je	.Last

  jmp	.notLast
.Last:
  push	8
  extern malloc
  call 	malloc
  pop	ecx

  mov	edi, esi
  stosd
  mov	edi, eax
  mov	eax, elem
  stosd  
.end:
  mov	esp, ebp
  pop	ebp
  ret

;************************************************************
; dequeue(QueueNode **queue)
;************************************************************
%define q		 [ebp+8]
dequeue:
  push	ebp
  mov	ebp, esp

  mov	edi, q
  mov	edi, [edi]
  cmp	edi, 0
  jne	.deq
  mov	eax, 0
  jmp	.end
.deq:
  mov	esi, edi
  lodsd
  mov	edx, eax
  lodsd
  mov	edi, q
  mov	[edi], eax
  mov	eax, edx

.end:

  mov	esp, ebp
  pop	ebp
  ret

