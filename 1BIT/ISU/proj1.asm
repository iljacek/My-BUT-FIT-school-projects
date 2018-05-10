%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    myVar1 db 0x42,0x84,0x33,0x66
    myVar2 dw 0x4200,0x5069
    myVar3 dd 0x42424242, 0x12345678

section .text
_main:
    push ebp
    mov ebp, esp
        
    mov eax,[myVar3+4]
    ; zde bude vas kod

    pop ebp
    ret