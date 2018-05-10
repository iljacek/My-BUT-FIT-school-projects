%include "rw32-2018.inc"

section .data
    ; zde budou vase data
b dw 10,20,30,40
text db "hello",0
text2 resb 6

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov esi, text
    mov edi, text2
    cld
    
    mov ecx, 6
    rep movsb
    
    ; zde bude vas kod

    pop ebp
    ret