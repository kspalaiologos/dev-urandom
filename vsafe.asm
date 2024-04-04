; Reset vsafe (0xFA) warning options (0x02) to none (0).
; Hardcoded password is 05945.
mov ax, 0fa02
mov bl, 0
mov dx, 05945
int 016