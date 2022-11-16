
; this function draws a triangle from y1 to y2 using x1 and x2 as the starting
; x points along with deltas dx_right amd dx_left. The function uses 32 bit
; fixed point math, notice the use of floating point instructions to convert
; the floating point values into fixed point values

.MODEL MEDIUM,C              ; use medium memory model C function names

.386

.CODE                        ; begin the code segment

PUBLIC Triangle_Asm          ; export function name to linker


Triangle_Asm PROC FAR C dest:DWORD, y1:WORD, y2:WORD, x1:REAL4, x2:REAL4, dx_left:REAL4, dx_right:REAL4, color:WORD

; local variables

LOCAL xs_f:DWORD, xe_f:DWORD,dx_left_f:DWORD, dx_right_f:DWORD,xs:WORD,xe:WORD


    mov bx,0                 ; reset bx to 0, use for line increment

    mov dx,y1                ; dx=y1

    fld  x1                  ; xs_f = (x1*65536)
    fmul FPSHIFT
    fistp xs_f

    fld  x2                  ; xe_f = (x2*65536)
    fmul FPSHIFT
    fistp xe_f

    fld  dx_left             ; dx_left_f = (dx_left*65536)
    fmul FPSHIFT
    fistp dx_left_f

    fld  dx_right            ; dx_right_f = (dx_right*65536)
    fmul FPSHIFT
    fistp dx_right_f


; for (dx=y1; dx<=y2; dx++)

begin_line:

    mov ax,WORD PTR xs_f[2]  ; get whole part of xs_f into xs
    mov xs,ax

    mov ax,WORD PTR xe_f[2]  ; get whole part of xe_f into xe
    mov xe,ax


    les di,dest              ; point es:di to start of line
    add di,xs
    add di,bx                ; add vertical offset

    mov cx, color            ; cx = color | color << 8
    mov ch,cl

process_left_end:

    mov ax,WORD PTR xs_f[2]  ; ax=xs & 0x01
    and ax,01h

test_l1:

    cmp ax,1                 ; if (ax==1)
    jne process_right_end

    mov es:[di], cl          ; plot pixel
    inc xs                   ; xs++;

process_right_end:

    les di,dest              ; point es:di to start of line
    add di,xe
    add di,bx                ; add vertical offset

    mov ax,xe                ; ax=xs & 0x01
    and ax,01h

test_r0:                     ; if (ax==0)

    cmp ax,0
    jne process_middle

    mov es:[di],cl           ; plot pixel

    dec xe                   ; xe-=1

process_middle:


    les di,dest              ; point es:di to start of line
    add di,xs
    add di,bx                ; add vertical offset

    cld                      ; clear the direction of movement

    mov ax, cx               ; move the color data into eax

    mov cx,xe                ; compute number of words to move  (xe-xs+1)/2
    sub cx,xs
    inc cx
    shr cx,1                 ; divide by 2

    rep stosw                ; fill the region with data


end_line:
                             ; perform fixed point addition of deltas
    mov eax,dx_left_f        ; xs_f+=dx_left_f
    add xs_f,eax

    mov eax,dx_right_f       ; xe_f+=dx_right_f
    add xe_f,eax

    add bx,320               ; move to next line in destination buffer


    inc dx                   ; dx++
    cmp dx,y2

    jle begin_line           ; if (dx<=y2) process next line


    ret                      ; later!


; constants section, must be here so that processor doesn't try to execute
; data

FPSHIFT REAL4 65536.0 ; this is equivalent to FP_SCALE in the fixed point defines

Triangle_Asm ENDP

END





