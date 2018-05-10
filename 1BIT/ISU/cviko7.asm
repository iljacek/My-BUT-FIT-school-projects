%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
mocnina2:
    push ebp
    mov ebp, esp
    push edx
    
    mov eax,[ebp+8]
    mul eax

    pop edx
    mov esp, ebp
    pop ebp
    ret 4

_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    
    ;hodnoty sa do stacku vkladaju naopak !! 
    ;pretoze plati last in first out
    
    ;na zaciatku pushnem registre s ktorymi budem pracovat
    ;aby som nestratil ich povodne hodnoty
    ;predpokladam ze eax neukladam lebo v nom bude vysledok funkcie
    
    push dword 5
    call mocnina2
    call WriteInt32
    call WriteNewLine
    
    pop ebp
    ret