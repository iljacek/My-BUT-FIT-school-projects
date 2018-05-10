%include "rw32-2018.inc"

section .data
    ; zde budou vase data
b dw 10,20,30,40
text db "hello",0
text2 resb 6

c1 dd 100,200,300,400
c2 dd 100,200,301,400

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov esi, c1
    mov edi, c2
   
    mov ecx, 4
cyklus:
    cmpsd ;to iste ako cmp esi,edi
    jne pryc
    loop cyklus
    jmp end   
         
pryc:
    mov eax,0xFFFFFFFF
    
end:
    
    
    ; zde bude vas kod

    pop ebp
    ret