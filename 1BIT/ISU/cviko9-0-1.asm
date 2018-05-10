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
    
    mov esi, b
    mov edi, b
    cld
    
    mov ecx, 4
cyklus:    
    lodsw
    add ax,ax
    stosw
    
    loop cyklus
    
    ; zde bude vas kod

    pop ebp
    ret