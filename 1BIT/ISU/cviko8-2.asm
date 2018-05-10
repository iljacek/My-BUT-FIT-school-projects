%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    character db -1
    alfa db "alfa",0
    beta db "beta",0
    gamma db "gamma",0
    delta db "delta",0
    error db "error",0
    cases DD a
         DD b
         DD c
         DD d

section .bss


section .text

;funkcia ktora po zadani vstupu preklada do greckej abecedy
;a -> alfa(ako string),...
grecka:
    push ebp
    mov ebp, esp
    push edx
    push ebx
    
    xor ebx, ebx
    mov bl, [character]
    sub bl, 97
    
    CMP bl,0
    jl default_case
    CMP bl, 3
    jg default_case        
    JMP dword [cases+4*EBX]
    
a:
    mov eax, alfa
    JMP break
b:
    mov eax, beta
    JMP break
c:
    mov eax, gamma
    JMP break
d:
    mov eax, delta
    JMP break
default_case:
    mov eax, error
    JMP break
       
break:

    pop edx
    mov esp, ebp
    pop ebp
    ret 4

_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    call ReadChar
    mov [character], al
    
    push dword [character]
    call grecka
    mov esi,eax    
    call WriteString   
    call WriteNewLine


    pop ebp
    ret