%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    xor eax,eax
    mov edx, 0xFFFFFFFF
    mov cl, 0

search:   
    test edx, 0x80000000     
    jz found
    inc cl
    rol edx, 1
    cmp cl, 32
    je not_found
    jmp search

found:
    mov bl, cl
    jmp end
    
not_found:
    mov bl, 42

end:
    pop ebp
    ret