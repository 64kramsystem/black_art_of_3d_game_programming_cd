
; this function draws a line from xs to xe using 32 bit data movement

.MODEL MEDIUM,                  ; use medium memory model C function names

.CODE                           ; begin the code segment

.386

PUBLIC _Triangle_32Line          ; export function name to linker


_Triangle_32Line PROC

ARG dest:DWORD, xs:WORD, xe:WORD, color:WORD


    push bp         ; create stack frame
    mov bp,sp

    push di         ; save di


    les di, dest    ; point es:di to start of line
    add di,xs

; process special cases first, i.e. lines of length 1,2,3 or 4

begin:

    mov ax,xe       ; ax=xs-xe;
    sub ax,xs

    mov cx, color   ; cx = color | color << 8
    mov ch,cl

test_0:

    cmp ax,0        ; if (ax==0)
    jne test_1      ; else goto test_1

    mov es:[di],cl
    ret


test_1:

    cmp ax,1        ; if (ax==1)
    jne test_2      ; else goto test_2

    mov es:[di],cx
    ret


test_2:

    cmp ax,2        ; if (ax==2)
    jne test_3      ; else goto test_3

    mov es:[di],cx
    add di,2
    mov es:[di],cl
    ret


test_3:

    cmp ax,3                   ; if (ax==0)
    jne process_left_end       ; else process left end

    mov es:[di],cx
    add di,2
    mov es:[di],cx
    ret

process_left_end:

    mov ax,xs                  ; ax=xs & 0x03
    and ax,03h

test_l1:

    cmp ax,1                   ; if (ax==1)
    jne test_l2

    mov es:[di], cl
    inc di
    mov es:[di], cx

    add xs,3                   ; xs +=3

    jmp process_right_end

test_l2:                       ; if (ax==2)

    cmp ax,2
    jne test_l3

    mov es:[di],cx

    add xs,2                   ; xs+=2

    jmp process_right_end


test_l3:                       ; if (ax==3)

    cmp ax,3
    jne process_right_end

    mov es:[di],cl

    inc xs                     ; xs+=1


process_right_end:

    les di, dest               ; point es:di to start of line
    add di,xe

    mov ax,xe                  ; ax=xs & 0x03
    and ax,03h

test_r0:                       ; if (ax==0)

    cmp ax,0
    jne test_r1

    mov es:[di],cl

    dec xe                     ; xe-=1

    jmp process_middle

test_r1:                       ; if (ax==1)

    cmp ax,1
    jne test_r2

    dec di
    mov es:[di],cx

    sub xe,2                   ; xe-=2

    jmp process_middle


test_r2:                       ; if (ax==2)

    cmp ax,2
    jne process_middle

    mov es:[di],cl
    sub di,2
    mov es:[di],cx

    sub xe,3                   ; xe-=3

process_middle:


    les di,dest                ; point es:di to start of line
    add di,xs

    cld                        ; clear the direction of movement

    mov eax, ecx               ; move the color data into eax
    shl eax,16
    or eax,ecx

    mov cx,xe                  ; compute number of words to move  (xe-xs+1)/2
    sub cx,xs
    inc cx
    shr cx,2                   ; divide by 4

    rep stosd                  ; fill the region with data

    pop di                     ; restore di
    pop bp                     ; fixup stack

    ret                        ; return to caller

Triangle_32Line ENDP

END





