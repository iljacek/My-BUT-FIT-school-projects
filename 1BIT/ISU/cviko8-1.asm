%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    TEXT DB "ABCDEFGH",0

section .bss

section .text

delka:
;vstup je ukazatel na text
;do eax vrati dlzku retazca (pocet znakov po prvu 0
    push ebp
    mov ebp, esp
    push edx
    push ebx
    
    mov dl, [TEXT]
    mov ebx, 0
    
    while:
        cmp dl, 0
        jz end
        inc ebx
        mov dl, [TEXT+ebx]
        JMP while
    end:
    mov eax, ebx
    
    pop ebx
    pop edx
    mov esp, ebp
    pop ebp
    ret 4

_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod
    push dword [TEXT]
    call delka
    call WriteInt32
    call WriteNewLine

    pop ebp
    ret