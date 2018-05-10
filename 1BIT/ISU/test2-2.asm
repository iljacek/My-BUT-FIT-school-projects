%include "rw32-2018.inc"

section .data
    pA dd 42, 24, 33, -11, 4, 62
    pB dd 1, 0, 0, 0, 0, 0
    N dd 6
    x dd 10
    ; zde budou vase data

section .text
;--- Úkol 2 ---
;
;Naprogramujte funkci "subX", která od jednotlivých prvku pole A odecte hodnotu x a jednotlivé výsledky uloží do pole B.
;
;Délka polí je dána parametrem N. Funkce musí být naprogramována s využitím predávání parametru na zásobníku tak,
;že parametry funkce se ukládají na zásobník od posledního do prvního (zprava doleva),
;parametry ze zásobníku uklízí volající a výsledek funkce se vrací v registru EAX.
;
;int subX(int *pA, unsigned int N, int x, int *pB)
;  - vstup:
;    pA: ukazatel na pole A (pole A obsahuje 32bitové hodnoty se znaménkem)
;     N: pocet prvku pole A (32bitové císlo bez znaménka)
;     x: hodnota x (32bitové císlo se znaménkem)
;    pB: ukazatel na pole B (pole B bude obsahovat 32bitové hodnoty se znaménkem)
;  - výstup:
;    - EAX =  0: v prípade, že nenastala žádná chyba  
;    - EAX = -1: v prípade, že je neplatný ukazatel *pA (tj. pA == 0)
;    - EAX = -2: v prípade, že je neplatný ukazatel *pB (tj. pB == 0)
;  - funkce musí zachovat obsah všech registru, krome registru EAX a príznakového registru
subX:
    push ebp
    mov ebp, esp
    push ebx
    push edx
    push ecx
    
    ; zde bude vas kod
    mov edx, [ebp + 8]
    cmp edx, 0
    je error_pA
    
    mov edx, [ebp + 20]
    cmp edx, 0
    je error_pB
    
    mov ebx,0
    mov ecx, [ebp + 12]
    
    for:
        dec ecx
        mov edx, [ebp + 8]
        mov eax, [edx + ebx*4]
        sub eax, [ebp + 16]        
        mov edx, [ebp +20]
        mov [edx + ebx*4], eax
        inc ebx
        cmp ecx,0
        jne for
    
    mov eax, 0
    jmp end
    
    error_pA:
    mov eax, -1
    jmp end
    
    error_pB:
    mov eax, -2
    
    end:
    pop ecx
    pop edx
    pop ebx
    pop ebp
    ret

_main:
    push ebp
    mov ebp, esp
    
    push dword pB
    push dword [x]
    push dword [N]
    push dword pA
    call subX
    
    add esp, 16 ; uklizi volajici
    
    call WriteInt32
    call WriteNewLine
    
    pop ebp
    ret