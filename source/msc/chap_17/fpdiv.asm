
; this function uses 64 bit math to divide two numbers in 32 bit 16:16
; fixed point format

.MODEL MEDIUM,C              ; use medium memory model C function names

.386

.CODE                        ; begin the code segment

PUBLIC FP_Div               ; export function name to linker


FP_Div PROC FAR C dividend:DWORD, divisor:DWORD

    mov eax,dividend ; move dividend into eax
    cdq              ; convert eax into edx:eax using sign extension
    shld edx,eax,16  ; shift edx:eax 16 bits to the left in so that
                     ; result is in fixed point
    sal eax,16       ; but manually shift eax since shld doesn't change the
                     ; source register (pretty stupid!)

    idiv divisor     ; perform divison

    shld edx,eax,16  ; move result into dx:ax since it's in eax
    ret              ; return to caller

FP_Div ENDP

END





