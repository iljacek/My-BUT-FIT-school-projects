%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov eax, 2
    mov ebx, 3
    mov ecx, 4
    
    shl ecx, 3
    sub ecx, 935
    add ecx, ebx
    
    mov esi, eax
    cdq
    idiv ebx
    sub ecx, eax
    
    mov eax, esi
    sub eax, 25
    mov ebx, -381
    imul ebx
    add eax, ecx
    
    
    ; zde bude vas kod

    pop ebp
    ret