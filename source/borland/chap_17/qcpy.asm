
; this function performs a memory copy from source to destination using 32
; bit moves

.MODEL MEDIUM                   ; use medium memory model C function names

.CODE                           ; begin the code segment

.386

PUBLIC _fquadcpy                 ; export function name to linker

_fquadcpy PROC

ARG dest:DWORD, source:DWORD, count:DWORD

push bp             ; create stack frame
mov bp,sp

push di             ; save a few registers
push si
push ds


cld             ; clear the direction of movement

lds si, source  ; point ds:si at source
les di, dest    ; point es:di at destination

mov ecx,count   ; move into ecx number of words
rep movsd       ; move the data

pop ds          ; restore the registers
pop si
pop di

pop bp          ; restore the stack

ret

_fquadcpy ENDP

END





