%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    pole dd -12, -4, -6, -5, -8, -42
    pocet dd 6

section .text
;--- �kol 1 ---
;
;Naprogramujte funkci "getMax", kter� v poli 32bitov�ch c�sel se znam�nkem nalezne maximum a vr�t� jeho hodnotu.
;
;int getMax(ESI, ECX)
;  - vstup:
;    - ESI: ukazatel na zac�tek pole 32bitov�ch prvku se znam�nkem
;    - ECX: pocet prvku pole (32bitov� hodnota bez znam�nka)
;  - v�stup:
;    - EAX: hodnota 32bitov�ho maxima se znam�nkem    
;  - funkce mus� zachovat obsah v�ech registru, krome registru EAX a pr�znakov�ho registru
getMax:
    push ebp
    mov ebp, esp
    push esi
    push ecx

    mov esi, [ebp+12]
    mov ecx, [ebp+8]
    mov eax, 0x80000000
    for:
        dec ecx
    if:
        cmp eax, [esi + 4*ecx]
        jg then
        mov eax, [esi + 4*ecx]
    then: 
        cmp ecx,0
        je end
        jmp for
    end:
    
    pop ecx
    pop esi
    pop ebp
    ret 

_main:
    push ebp
    mov ebp, esp
    
    mov esi, pole
    mov ecx, [pocet]
    
    push esi
    push ecx
    call getMax
    add esp,8
    
    call WriteInt32
    call WriteNewLine
    
    ; zde bude vas kod
    
    pop ebp
    ret