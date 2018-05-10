%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
; spoj 4 8bitove cisla do jednoho registru
;vytvorte 2 lokalne premenne s ktorymi budete pracovat

join:
    push ebp
    mov ebp, esp
    push edx
    
    mov edx, [ebp+8]
    mov al, dl
    rol eax, 8
    
    mov edx, [ebp+12]
    mov al, dl
    rol eax, 8
    
    mov edx, [ebp+16]
    mov al, dl
    rol eax, 8
    
    mov edx, [ebp+20]
    mov al, dl
    
    pop edx
    mov esp, ebp
    pop ebp
    ret 16
        
_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    push 0xDD
    push 0xCC
    push 0xBB
    push 0xAA
    call join
    call WriteHex32
    call WriteNewLine

    push 0xDD
    push 0xCC
    push 0xBB
    push 0xAA
    call join
    call WriteHex32
    call WriteNewLine

    pop ebp
    ret