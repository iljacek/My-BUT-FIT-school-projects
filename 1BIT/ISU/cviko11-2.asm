%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    a: dq 3.0 ; C-typ double, 64-bitu
    b: dq 4.0

section .text
;vypocitaj preponu



_main:
    push ebp
    mov ebp, esp
    
    
    
    ; zde bude vas kod

    
    ;push qword [a]
    ;push qword [b]
    
    ;add esp 8

    FLD qword [a]
    FLD qword [b]   
    
    FMUL ST0
    FXCH ST1
    FMUL ST0    
    FXCH ST1
    FADD ST1      
    FSQRT
    
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