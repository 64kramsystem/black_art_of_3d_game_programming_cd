
; this function fills a region of memory using 32 bit stores

.MODEL MEDIUM                   ; use medium memory model C function names

.386

.CODE                           ; begin the code segment

PUBLIC _fquadset                 ; export function name to linker

_fquadset PROC

ARG dest:DWORD, data:DWORD, count:DWORD

push bp         ; create the stack frame
mov bp,sp
push di         ; save this just in case

cld             ; clear the direction of movement

les di, dest    ; point es:di at destination
mov ecx, count  ; move into ecx number of quads
mov eax, data   ; move the data into eax
rep stosd       ; fill the region with data

pop di          ; restore di
pop bp          ; fixup stack

ret             ; return to caller

_fquadset ENDP

END





