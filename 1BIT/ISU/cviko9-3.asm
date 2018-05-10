%include "rw32-2018.inc"

section .data
    ; zde budou vase data
d1 dw 48
d dw 1,20,48,48,48,56,28,48,27,48

section .bss



section .text
; cisla dx ....
; hladaj konkretne cislo 
;a spocitaj kolkokrat je toto cislo v poli


_main:
    push ebp
    mov ebp, esp
       
    mov ecx, 10
    mov ax,48
    mov edi, d
    mov ebx, 0

cyklus:
    repne scasw
    inc ebx
    cmp ecx, 0
    jne cyklus
 
    ;mov esi, d1
    ;cmpsw 
end:
    dec ebx
    mov eax,ebx
    call WriteUInt32
    
    ; zde bude vas kod

    pop ebp
    ret