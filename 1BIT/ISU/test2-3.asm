%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    pA dw 12, 16, 42, 4, 32, 6
    N dd 6

section .text
;
;--- �kol 3 ---
;
;Naprogramujte funkci "sort", kter� sestupne (od nejvet��ho k nejmen��mu) serad� pole 16bitov�ch prvku bez znam�nka A.
;
;Uk�zka algoritmu razen� v jazyce C:
;
;int *pA, i, j, N;
;for(i = 0; i < N; i++) {
;    for(j = i + 1; j < N; j++) {
;        if (pA[i] < pA[j]) { tmp = pA[i]; pA[i] = pA[j]; pA[j] = tmp; }      
;    }
; }
;
;void sort(unsigned short *pA, unsigned int N)
;  - vstup:
;    ESI: ukazatel na pole A (pole A obsahuje 16bitov� hodnoty bez znam�nka)
;    ECX: pocet prvku pole A (32bitov� c�slo bez znam�nka)
;  - funkce mus� zachovat obsah v�ech registru, krome registru EAX a pr�znakov�ho registru
sort:
    push ebp
    mov ebp, esp
    push esi
    push ecx
    push ebx
    push edx
    mov ebx, 0 ;i
    mov edx, 0 ;j
    mov esi, [ebp + 8] ;pA
    mov ecx, [ebp + 12] ;N
    
    ; zde bude vas kod
    ;int *pA, i, j, N;
    ;for(i = 0; i < N; i++) {
    for1:
        mov edx, ebx
        inc edx 
        ;    for(j = i + 1; j < N; j++) {
        for2:           
            ;   if (pA[i] < pA[j]) { tmp = pA[i]; pA[i] = pA[j]; pA[j] = tmp; }    
            if:             
                mov ax, [esi + ebx*2]
                cmp ax,[esi + edx*2]
                jl then
                jmp endif
            then:
                mov ax, [esi + ebx*2]
                xchg ax, [esi + edx*2]
                mov [esi + ebx*2], ax
            endif:
                inc edx
                cmp edx, ecx
                jge endfor2
                jmp for2
        endfor2:
            inc ebx
            mov eax, ecx
            dec eax
            cmp ebx, eax
            jge end
            jmp for1
            
      
    end:
    pop edx
    pop ebx
    pop ecx
    pop esi
    pop ebp
    ret

_main:
    push ebp
    mov ebp, esp
    
    mov esi, pA
    mov ecx, [N]
    
    push ecx
    push esi
    call sort
    
    add esp, 8 ; uklizi volajici
    
    call WriteInt32
    call WriteNewLine

    pop ebp
    ret