%include	"constants.inc"

;****************************************
section .data
;****************************************
  hello		db		"Hello, world!", 10
  helloLen	equ 	$-hello

;****************************************
section .text
;****************************************

;****************************************
global _start
;****************************************
_start:
main:
  push	ebp
  mov	ebp, esp

  mov	eax, SYS_WRITE
  mov	ebx, STDERR
  mov	ecx, hello
  mov	edx, helloLen
  int	80h

  mov	esp, ebp
  pop	ebp

  mov	eax, SYS_EXIT
  xor	ebx, ebx
  int	80h

