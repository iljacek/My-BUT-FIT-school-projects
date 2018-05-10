%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    ;variable1 dw 42
    ;variable2 dw 50
    ;res resb
    string db "Ahoj123"

section .text
_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    ;mov ax,0x7FFF
    ;sub ax,0x8000
    
    ;mov ebx,variable1
    ;mov ax,[ebx]
    ;mov ebx,variable2
    ;add ax,[ebx]
    ;mov [res],ax

    
    mov ecx,0
_loop:
    mov esi,string
    mov al,[esi+ecx]
    call WriteChar
    add ecx,1
    cmp ecx,3
    jne _loop 

    
    pop ebp
    ret