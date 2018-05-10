%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    r: dq 30.0 ; C-typ double, 64-bitu
    styri: dq 4.0
    tri:   dq 3.0
    obsah db "S = ",0
    objem db "V = ",0

section .text
;spocitaj obsah a objem pre zadane r


_main:
    push ebp
    mov ebp, esp
    
    
    
    ; zde bude vas kod


    FLD qword [r]
    
V:        
    FLD qword [styri]
        
    FDIV qword [tri]
    FLDPI       ; st0=pi, st1=4/3, st2= r
    FMULP st1, st0 ; st0=4/3 * pi, st1= r
    
    FMUL st1
    FMUL st1
    FMUL st1
        
    mov ESI, objem
    call WriteString
    call WriteDouble
    call WriteNewLine
    
    FSTP ST0
        
S:

    FLDPI ; st0=pi, st2=r
    
    FMUL qword [styri]
    FMUL st1
    FMULp st1, st0    
        
    mov ESI, obsah
    call WriteString
    call WriteDouble
    call WriteNewLine
    
    FSTP ST0
    
    

    call WriteNewLine
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