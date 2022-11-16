
; this function draws a line from from xs to xe using 16 bit dat movement


.MODEL MEDIUM                 ; use medium memory model C function names

.CODE                           ; begin the code segment

.386

PUBLIC _Triangle_16Line          ; export function name to linker


_Triangle_16Line PROC

ARG dest:DWORD, xs:WORD, xe:WORD, color:WORD


begin:

    push bp                   ; create stack frame
    mov bp,sp

    push di                   ; save this guy


    les di,dest               ; point es:di to start of line
    add di,xs

    mov cx, color             ; cx = color | color << 8
    mov ch,cl

process_left_end:

    mov ax,xs                 ; ax=xs & 0x01
    and ax,01h

test_l1:

    cmp ax,1                  ; if (ax==1)
    jne process_right_end

    mov es:[di], cl           ; plot pixel
    inc xs                    ; xs++

process_right_end:

    les di,dest               ; point es:di to start of line
    add di,xe

    mov ax,xe                 ; ax=xs & 0x01
    and ax,01h

test_r0:                      ; if (ax==0)

    cmp ax,0
    jne process_middle

    mov es:[di],cl            ; plot pixel

    dec xe                    ; xe-=1

process_middle:


    les di,dest               ; point es:di to start of line
    add di,xs

    cld                       ; clear the direction of movement

    mov ax, cx                ; move the color data into eax

    mov cx,xe                 ; compute number of words to move  (xe-xs+1)/2
    sub cx,xs
    inc cx
    shr cx,1                  ; divide by 2

    rep stosw                 ; fill the region with data


    pop di                    ; restore di
    pop bp                    ; fixup stack

    ret                       ; return to caller

_Triangle_16Line ENDP

END





