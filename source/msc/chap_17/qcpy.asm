
; this function performs a memory copy from source to destination using 32
; bit moves

.MODEL MEDIUM,C                 ; use medium memory model C function names

.CODE                           ; begin the code segment

PUBLIC fquadcpy                 ; export function name to linker


fquadcpy PROC FAR C USES ds, dest:DWORD, source:DWORD, count:DWORD

.386

cld             ; clear the direction of movement

lds si, source  ; point ds:si at source
les di, dest    ; point es:di at destination

mov ecx,count   ; move into ecx number of words
rep movsd       ; move the data

ret

fquadcpy ENDP

END





