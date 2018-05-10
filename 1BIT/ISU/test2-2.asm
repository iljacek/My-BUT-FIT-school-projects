%include "rw32-2018.inc"

section .data
    pA dd 42, 24, 33, -11, 4, 62
    pB dd 1, 0, 0, 0, 0, 0
    N dd 6
    x dd 10
    ; zde budou vase data

section .text
;--- �kol 2 ---
;
;Naprogramujte funkci "subX", kter� od jednotliv�ch prvku pole A odecte hodnotu x a jednotliv� v�sledky ulo�� do pole B.
;
;D�lka pol� je d�na parametrem N. Funkce mus� b�t naprogramov�na s vyu�it�m pred�v�n� parametru na z�sobn�ku tak,
;�e parametry funkce se ukl�daj� na z�sobn�k od posledn�ho do prvn�ho (zprava doleva),
;parametry ze z�sobn�ku ukl�z� volaj�c� a v�sledek funkce se vrac� v registru EAX.
;
;int subX(int *pA, unsigned int N, int x, int *pB)
;  - vstup:
;    pA: ukazatel na pole A (pole A obsahuje 32bitov� hodnoty se znam�nkem)
;     N: pocet prvku pole A (32bitov� c�slo bez znam�nka)
;     x: hodnota x (32bitov� c�slo se znam�nkem)
;    pB: ukazatel na pole B (pole B bude obsahovat 32bitov� hodnoty se znam�nkem)
;  - v�stup:
;    - EAX =  0: v pr�pade, �e nenastala ��dn� chyba  
;    - EAX = -1: v pr�pade, �e je neplatn� ukazatel *pA (tj. pA == 0)
;    - EAX = -2: v pr�pade, �e je neplatn� ukazatel *pB (tj. pB == 0)
;  - funkce mus� zachovat obsah v�ech registru, krome registru EAX a pr�znakov�ho registru
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