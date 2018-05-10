%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    MEZ: dq 31.0 ; C-typ double, 64-bitu
    mensi db "zadane cislo je mensie ako mez",0
    vacsi db "zadane cislo je vacsie ako mez",0
    rovny db "zadane cislo je rovnake ako mez",0

section .text
;nacitaj cez readdouble a porovnaj s MEZou, vypis vysledok > < =



_main:
    push ebp
    mov ebp, esp
    
    
    
    ; zde bude vas kod


    FLD qword [MEZ]
    call ReadDouble
    
    FSUB ST1
    FTST
    FSTSW AX
    SAHF
    JA bigger
    
    FTST
    FSTSW AX
    SAHF
    JZ equal
    
    mov ESI, mensi
    call WriteString
    call WriteNewLine
    jmp end
    
    bigger:    
    mov ESI, vacsi
    call WriteString
    call WriteNewLine
    jmp end
    
    equal:
    mov ESI, rovny
    call WriteString
    call WriteNewLine
    
    end:
    call WriteDouble
    call WriteNewLine
    
    FXCH ST1
    call WriteDouble
    call WriteNewLine
    FXCH ST1    
    
    FXCH ST2
    call WriteDouble
    call WriteNewLine
    FXCH ST2 
    
    FXCH ST3
    call WriteDouble
    call WriteNewLine
    FXCH ST3 
    
    FXCH ST4
    call WriteDouble
    call WriteNewLine
    FXCH ST4 
    
    FXCH ST5
    call WriteDouble
    call WriteNewLine
    FXCH ST5 
    
    FXCH ST6
    call WriteDouble
    call WriteNewLine
    FXCH ST6 
    
    FXCH ST7
    call WriteDouble
    call WriteNewLine
    FXCH ST7 

    pop ebp
    ret