
; this function uses 64 bit math to multiply two 32 bit fixed point numbers
; in 16:16 format

.MODEL MEDIUM              ; use medium memory model C function names

.386

.CODE                        ; begin the code segment

PUBLIC _FP_Mul               ; export function name to linker


_FP_Mul PROC

ARG multiplier:DWORD, multiplicand:DWORD

    push bp          ; create stack frame
    mov bp,sp

    mov eax,multiplicand    ; move into eax multiplicand
    imul multiplier         ; multiply by multiplier, result edx:eax
    shr eax,16              ; need to shift upper 16 bits of eax to right
                            ; so that result is in dx:ax instead of edx:eax

    pop bp                  ; fixup stack
    ret                     ; return to caller

_FP_Mul ENDP

END





