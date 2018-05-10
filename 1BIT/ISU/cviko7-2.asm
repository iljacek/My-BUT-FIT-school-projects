%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
A2_B2:
    push ebp
    mov ebp, esp
    sub esp, 8
    push edx
    
    mov eax, [ebp+8]
    add eax, [ebp+12]
    mov [ebp-4], eax
    
    mov eax, [ebp+8]
    sub eax, [ebp+12]
    
    mul dWord [ebp-4]
    
    pop edx
    mov esp, ebp
    pop ebp
    ret 8

_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    push 4
    push 6
    call A2_B2
    call WriteInt32
    call WriteNewLine
    
    push 4
    push 6
    call A2_B2
    call WriteInt32
    call WriteNewLine

    pop ebp
    ret