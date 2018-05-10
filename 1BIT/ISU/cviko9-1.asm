%include "rw32-2018.inc"

section .data
    ; zde budou vase data
text db "hello_world",0
text2 db "hello_woald",0
good db "no error occured",0
bad db "failure in character number: ",0


section .text
;mame 2 textove polia v ktorych je retazec 
;citaj odzadu a najdi prvu poziciu 
;na ktorej sa retazce lisia, vyuzi CMPS

_main:
    push ebp
    mov ebp, esp
    pushf
    
    mov esi, text+10
    mov edi, text2+10
   
    std
    mov ecx, 11
    repe cmpsb ;to iste ako cmp esi,edi
    jecxz success
         
failure:
    mov eax,ecx
    inc eax
    mov esi, bad
    call WriteString
    call WriteUInt32
    call WriteNewLine
    jmp end
    
success:
    mov esi, good
    call WriteString
    call WriteNewLine
    jmp end
        
end:
    
    
    ; zde bude vas kod
    popf
    pop ebp
    ret