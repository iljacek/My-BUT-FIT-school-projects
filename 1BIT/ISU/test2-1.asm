%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    pole dd -12, -4, -6, -5, -8, -42
    pocet dd 6

section .text
;--- Úkol 1 ---
;
;Naprogramujte funkci "getMax", která v poli 32bitových císel se znaménkem nalezne maximum a vrátí jeho hodnotu.
;
;int getMax(ESI, ECX)
;  - vstup:
;    - ESI: ukazatel na zacátek pole 32bitových prvku se znaménkem
;    - ECX: pocet prvku pole (32bitová hodnota bez znaménka)
;  - výstup:
;    - EAX: hodnota 32bitového maxima se znaménkem    
;  - funkce musí zachovat obsah všech registru, krome registru EAX a príznakového registru
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