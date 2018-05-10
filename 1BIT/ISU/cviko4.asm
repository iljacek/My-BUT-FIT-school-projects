%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    memw1 dw 0x01FF
    memw2 dw 0x0001
    vypis db 'Vysledek: ', 0
    membarr db -100, 5
    memwarr dw -200, 10
    
section .bss
    ; zde budou deklarace neicializovanych dat
    vysledek resw 2

section .text
_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
ukol3:
    mov ax, [memw1]
    mov dx, [memw2]
    add al, dl
    adc ah, dh
    
    mov esi, vypis
    call WriteString
    call WriteHex16
    call WriteNewLine

    mov ax, [memw1]
    mov dx, [memw2]
    sub al, dl
    sbb ah, dh
    
    mov esi, vypis
    call WriteString
    call WriteHex16
    call WriteNewLine

ukol4:
    xor eax, eax
    xor edx, edx
    mov dx, [membarr]
    mov al, dl
    cbw
    idiv dh
    
    call WriteInt8
    call WriteNewLine
    mov al, ah    
    call WriteInt8
    call WriteNewLine

ukol5:
    xor eax, eax
    xor edx, edx
    mov ax, [memwarr]
    imul word [memwarr+2]
    
    mov [vysledek], ax
    mov [vysledek+2], dx
    mov eax,[vysledek]
    call WriteInt32
    call WriteNewLine


    pop ebp
    ret