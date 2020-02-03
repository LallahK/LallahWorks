; asmsyntax=nasm
;*****************************************************************************
;* CS244: Assembler implementation of external functions defined in          *
;*        'tut1.h'.                                                          *
;*****************************************************************************

global max
global power
global gcd

;*****************************************************************************
;* int max(int x, int y);                                                    *
;*****************************************************************************
%define y [ebp+12]
%define x [ebp+8]
max:
  push  ebp
  mov   ebp, esp
  
  mov   eax, x                     ; if (x > y)
  cmp   eax, y                     ;   return x;
  jg    .else                      ; else
  mov   eax, y                     ;   return y;
                                   
.else:
  mov   esp, ebp
  pop   ebp
  ret

;*****************************************************************************
;* int power(int x, int y);                                                  *
;*****************************************************************************
%define y [ebp+12]
%define x [ebp+8]
power:
  push 	ebp
  mov 	ebp, esp

  mov 	ecx, y
  mov 	eax, 1
.pow:
  cmp 	ecx, 0
  je	.end
  imul	eax, x
  dec	ecx
  jmp	.pow
.end:

  mov 	esp, ebp
  pop 	ebp
  ret
  
;*****************************************************************************
;* int gcd(int m, int n);                                                    *
;*****************************************************************************
%define n [ebp+12]
%define m [ebp+8]
gcd:
  push 	ebp
  mov 	ebp, esp

  mov	eax, m
  mov	ebx, n
  xor	edx, edx
.gcd:
  mov	ecx, eax
  idiv	ebx
  imul	ebx
  sub	ecx, eax

  cmp	ecx, 0
  je	.end
  mov	eax, ebx
  mov	ebx, ecx
  jmp	.gcd
.end:
  cmp	ecx, 0
  jne	.gcd
  mov	eax, ebx

  mov	esp, ebp
  pop	ebp
  ret
