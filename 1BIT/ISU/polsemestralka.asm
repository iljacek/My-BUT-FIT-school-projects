%include "rw32-2018.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    XOR eax,eax
    MOV al, 8   ;AL=8
    MOV bl, 17  ;BL=17
    MOV cl, al  ;CL=8
    ADD cl, 2   ;CL=10
    TEST cl, 7        ;CL=10      1010 & 0111 = 0010 != 0
    JNZ hop1    ;jump
    XOR cl, 255 ;-
     
hop1:
    MOV ah, cl      ;AX= 1010 1010 = AA = 170
    AND cl, 7   ;cl= 1010 & 0111 = 0010 = 2
    SAR ax, cl        ;AX= posun do prava o 2 miesta, kopiruje sa MSB = 1010 1010 -> 1110 1010
    XCHG al, dl ;dl=1010 = 10
    AND dl, 127     ;DL= 1010 & 1111 1111 = 1010 = 10, alebo error 

    pop ebp
    ret