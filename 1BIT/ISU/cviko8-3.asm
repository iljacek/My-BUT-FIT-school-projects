%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .bss
    Tx resb 6

section .text
;funkcia ktora zmeni vstupny string na uppercase
;vyuziva fuknciu delka z 8-1
;znaky ktore nie su male pismena skopiruje
;na tabuli:
;readstring 
;velka (*text2)
;eax = * prevede mala na velka
toUpper:

_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod

    pop ebp
    ret