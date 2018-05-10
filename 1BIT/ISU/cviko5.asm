%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    delitel dw 5
    konstanta dw 142
    vypismensi db 'Zadane cislo je mensie', 0
    vypisvacsi db 'Zadane cislo je vacsie', 0
    vypisrovny db 'Zadane cislo je rovne', 0
    
section .bss
    ; zde budou deklarace neicializovanych dat
    
section .text
_main:
    push ebp
    mov ebp, esp
        
    ; zde bude vas kod
    jmp uloha1
    
logOperacie:
    mov ax, 0xFF88
    and ax, 0xFA77
    or ax, 0x00FF
    xor ax, ax
    not ax
    neg ax
    jmp end

uloha1:
    ;vydel 32 bit cislo a uloz zbytok a celu cast do ECX
    mov edx, 0
    mov eax, 42
    div dword [delitel]
    mov cx, dx
    shl ecx, 16
    mov cx, ax
    jmp end
 
uloha2:
    ;zadajte cislo z klavesnice a porovnaj s konstantou
    ;vypiste vacsie mensie rovno
    call ReadUInt16 
    cmp [konstanta], ax 
    jg  mensi
    
    cmp [konstanta], ax 
    jl  vacsi
    
    
    mov esi, vypisrovny
    call WriteString
    jmp end
    
vacsi:    
    mov esi, vypisvacsi
    call WriteString
    jmp end
    
mensi:    
    mov esi, vypismensi
    call WriteString
    jmp end    
      
                            
uloha3:
    mov eax, 0xAAAA
    mov ecx, 16
    mov edx, 0
.again:
    TEST ax, 0x0001
    shr ax, 1   
    cmp cx, 0
    jmp end                                                                         
    loop .again                                                                                                            
end:                                                                                               
    pop ebp
    ret