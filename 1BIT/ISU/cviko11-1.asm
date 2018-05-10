%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    a: dq 1.0 ; C-typ double, 64-bitu
    b: dq 2.0
    c: dq 3.0    
    d: dq 1.0 ; C-typ double, 64-bitu
    e: dq 2.0
    f: dq 3.0    
    g: dq 1.0 ; C-typ double, 64-bitu
    h: dq 2.0

section .text
;urob nedestruktivni vypis hodnot z reg ST0 az ST7

_main:
    push ebp
    mov ebp, esp
    
    ; zde bude vas kod

    FLD qword [a]
    FLD qword [b]
    FLD qword [c]
    
    call WriteDouble
    call WriteNewLine
    
    FXCH ST1
    call WriteDouble
    call WriteNewLine
    FXCH ST0    
    
    FXCH ST2
    call WriteDouble
    call WriteNewLine
    FXCH ST0 
    
    FXCH ST3
    call WriteDouble
    call WriteNewLine
    FXCH ST0 
    
    FXCH ST4
    call WriteDouble
    call WriteNewLine
    FXCH ST0 
    
    FXCH ST5
    call WriteDouble
    call WriteNewLine
    FXCH ST0 
    
    FXCH ST6
    call WriteDouble
    call WriteNewLine
    FXCH ST0 
    
    FXCH ST7
    call WriteDouble
    call WriteNewLine
    FXCH ST0 

    pop ebp
    ret