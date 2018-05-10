%include "rw32-2018.inc"

section .data
    ; zde budou vase data
text db "hello_",0

section .bss
text2 resb 13


section .text
;text hello_ a text2=resb 
;nahraj do text2 hello_hello
;vyuzi MOVS

_main:
    push ebp
    mov ebp, esp
    
    mov esi, text
    mov edi, text2
   
    mov ecx, 13
cyklus:
    movsb
    cmp ecx, 8
    je else
    jmp endif
else:
    mov esi, text
endif:
    dec ecx
    cmp ecx,0
    je end
    jmp cyklus   
    
end:
    mov esi, text2
    call WriteString
    call WriteNewLine
    
    ; zde bude vas kod

    pop ebp
    ret