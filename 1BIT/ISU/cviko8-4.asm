%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    cisla DW 10,20,30,40

section .text
;cisla DW 10,20,30,40
;vynasob dvomi
vynasob2:
    push ebp
    mov ebp, esp
    push edx
    push ebx
    
    xor eax,eax
    mov eax,[cisla+EBX*2] 
    shl eax, 4
    shr eax, 3
    
    pop ebx
    pop edx
    mov esp, ebp
    pop ebp
    ret 4

_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    
    
 
    MOV ECX,4
    MOV EBX,0
    for:
        push dword [cisla]
        call vynasob2
        call WriteInt32
        call WriteNewLine   
        inc EBX     
    LOOP for 
    


    pop ebp
    ret