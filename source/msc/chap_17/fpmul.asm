
; this function uses 64 bit math to multiply two 32 bit fixed point numbers
; in 16:16 format

.MODEL MEDIUM,C              ; use medium memory model C function names

.386

.CODE                        ; begin the code segment

PUBLIC FP_Mul               ; export function name to linker


FP_Mul PROC FAR C multiplier:DWORD, multiplicand:DWORD

    mov eax,multiplicand    ; move into eax multiplicand
    imul multiplier         ; multiply by multiplier, result edx:eax
    shr eax,16              ; need to shift upper 16 bits of eax to right
                            ; so that result is in dx:ax instead of edx:eax
    ret                     ; return to caller

FP_Mul ENDP

END





