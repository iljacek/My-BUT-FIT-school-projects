%include "rw32-2018.inc"

section .data
    ; zde budou vase data
b dw 10,20,30,40
text db "hello",0
text2 resb 6

c1 dd 100,200,300,400
c2 dd 100,200,301,400

d1 dw 48
d dw 15,20,52,31,24,56,28,48,27,46

section .text
; kolko je v poli cisla cisel vacsich ako x


_main:
    push ebp
    mov ebp, esp
       
    mov ecx, 10
    mov ax,48
    mov edi, d

cyklus:
    repne scasw
    ;mov esi, d1
    ;cmpsw 
end:
    
    
    ; zde bude vas kod

    pop ebp
    ret