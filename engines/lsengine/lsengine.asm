;Beeper music engine
;Two channels of tone, no volume or timbre control, non-interrupting drums
;Originally written by Lyndon Sharp circa 1989
;Reverse-engineered from Zanthrax game and modified in 2011 by Shiru
;Modifications are:
; minor optimizations
; order list added
; drum numbers changed
; notes shifted one semitone down


lsengine.play:
	xor	a
	in	a, ($1f)
	or	a
	jr	z, $+4
	ld	a, $ff
	cpl
	ld	(playLoop.checkKempston), a
playLoop:
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
	ld	a, d
	or	e
	jr	nz, playLoop.noLoop
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	ex	de, hl
	jr	playLoop
playLoop.noLoop:
	push	hl
	ex	de, hl
	ld	b, 0
	ld	c, (hl)
	inc	hl
	ld	a, (hl)
	inc	hl
	ld	(playRow.speed), a
	ld	a, c
	or	a
	jr	nz, $+3
	inc	b
	ld	(playRow.ch0ptr), hl
	add	hl, bc
	ld	(playRow.ch1ptr), hl
playLoop.playPtn:
	push	bc
	call	playRow
	xor	a
	in	a, ($1f)
playLoop.checkKempston: EQU $+1
	and	0
	jr	nz, playLoop.stop
	xor	a
	in	a, ($fe)
	cpl
	and	$1f
	jr	nz, playLoop.stop
	pop	bc
	dec	bc
	ld	a, b
	or	c
	jr	nz, playLoop.playPtn
	pop	hl
	jr	playLoop
playLoop.stop:
	pop	bc
	pop	hl
	ret

playRow:
	di
	ld	ix, $1010	;enable both tone channels (it is mask to xor with output)
	ld	d, 0
playRow.ch0ptr:	EQU $+1
	ld	hl, 0		;read byte from first channel pointer
	ld	c, (hl)
	ld	a, c
	and	$3f
	jr	nz, playRow.noMute0	;if it is zero, mute the channel
	ld	ixl, d
playRow.noMute0:
	inc	hl		;increase pointer
	ld	(playRow.ch0ptr), hl	;store pointer
	ld	e, a			;read divider from note table
	ld	hl, noteDivTable
	add	hl, de
	ld	a, (hl)
	ld	(playRow.ch0div), a	;set divider
playRow.ch1ptr:	EQU $+1
	ld	hl, 0			;the same for second channel
	ld	b, (hl)
	ld	a, b
	and	$3f
	jr	nz, playRow.noMute1
	ld	ixh, d
playRow.noMute1:
	inc	hl
	ld	(playRow.ch1ptr), hl
	ld	e, a
	ld	hl, noteDivTable
	add	hl,de
	ld	a, (hl)
	ld	(playRow.ch1div), a
	ld	a, b	;now use note values to get drum number, four bits, lower always 0
	rlca		;two top bits of note of second channel are top bits of the number
	rlca
	rl	c	;and two top bits of note of first channel are lower bits
	rla
	add	a, a
	and	$0f	;now there is drum number in A
	ld	e, a
	ld	hl, drumTable	;read drum parameters pointer from drum table
	add	hl, de
	ld	a, (hl)
	inc	hl
	ld	h, (hl)
	ld	l, a
	ld	(playRow.drumPtr), hl
	ld	a, $10
	ld	(playRow.drumParam0), a
	ld	a, $29
	ld	(playRow.drumParam1), a
	ld	l, a
	xor	a
	ex	af, af'
	xor	a
	ld	h, $10
	ld	de, $0101
	exx
	ld	b, a
playRow.drumPtr: EQU $+1
	ld	hl, 0
	ld	e, $01
	exx
playRow.speed:	EQU $+1
	ld	c, 7
playRow.loop0:
	ld	b, 0
playRow.loop1:
	ex	af, af'
	dec	l
	jr	nz, $+3
	xor	a
	out	($fe), a
	dec	d
	jr	nz, playRow.delay0
playRow.ch0div:	EQU $+1
	ld	d, 0
	xor	ixl
playRow.drumParam0: EQU $+1
	ld	l, 0

playRow.delay0ret:
	exx
	ld	c, a
	ld	a, b
	out	($fe),a
	dec	e
	jr	nz, playRow.delay1
	ld	a, (hl)
	or	a
	jp	z, playRow.delay2
	ld	e, a
	ld	a, b
	xor	$10
	inc	hl
playRow.delay2ret:
	ld	b, a
	ld	a, c
	exx
	ex	af, af'
	dec	h
	jr	nz, $+3
	xor	a
	out	($fe),a
	dec	e

	jr	nz, playRow.delay3
playRow.ch1div:	EQU $+1
	ld	e, $9d

	xor	ixh
playRow.drumParam1: EQU $+1
	ld	h, $17
playRow.delay3ret:
	djnz	playRow.loop1
	push	af
	ld	a, (playRow.drumParam0)
	dec	a
	ld	(playRow.drumParam0), a
	ld	a, (playRow.drumParam1)
	sub	3
	ld	(playRow.drumParam1), a
	pop	af
	dec	c
	jp	nz, playRow.loop0

	ld	hl, $2758
	ei
	ret

playRow.delay0:
	xor	0
	jp	playRow.delay0ret

playRow.delay1:
	ld	(0), hl
playRow.delay2:
	ld	r, a
	jr	playRow.delay2ret

playRow.delay3:
	xor	0
	jp	playRow.delay3ret

drumTable:
	dw drum0
	dw drum1
	dw drum2
	dw drum3
	dw drum4
	dw drum5
	dw drum6
	dw drum7

drum0:
	db $00
drum1:
	db $05,$05,$0e,$0e,$17,$17,$2a,$17
	db $2a,$17,$2a,$17,$2a,$17,$17,$0e
	db $0e,$04,$04,$00
drum2:
	db $11,$08,$18,$06,$20,$09,$25,$0c
	db $2a,$0a,$2e,$08,$32,$0a,$37,$0d
	db $3d,$0b,$42,$09,$4c,$0b,$52,$0e
	db $5a,$0c,$62,$0a,$69,$0c,$70,$0e
	db $7b,$10,$89,$11,$96,$13,$9c,$15
	db $70,$12,$12,$72,$72,$0c,$55,$0c
	db $7a,$0b,$6d,$0b,$71,$0a,$74,$0a
	db $77,$09,$7c,$06
drum3:
	db $05,$0a,$0f,$14,$1b,$20,$1b,$1e
	db $10,$14,$17,$1a,$1d,$20,$2a,$36
	db $42,$4f,$5c,$4f
drum4:
	db $47,$41,$0f,$0f,$09,$0c,$01,$0d
	db $03,$05,$0e,$0c,$09,$06,$09,$0a
	db $0e,$0f,$01,$0c,$04,$0e,$08,$09
	db $06,$0b,$02,$02,$05,$06,$0c,$0b
	db $00
drum5:
	db $0b,$06,$0d,$0a,$0f,$0e,$11,$12
	db $13,$16,$15,$1a,$17,$1e,$19,$22
	db $1b,$26,$1d,$00
drum6:
	db $11,$05,$17,$05,$20,$05,$23,$05
	db $2a,$05,$2d,$05,$30,$05,$34,$05
	db $3f,$05,$42,$05,$4c,$05,$52,$05
	db $5f,$05,$65,$05,$69,$05,$6e,$05
	db $7b,$05,$7f,$05,$84,$05,$9f,$05
	db $65,$0f,$67,$19,$69,$23,$6b,$2d
	db $6d,$37,$6f,$41,$71,$4b,$73,$55
	db $75,$5f,$77
drum7:
	db $00

noteDivTable:
	db $00	;no div for mute, shifts all the notes one semitone down compared to the original
	db $fa,$eb,$de,$d2,$c6,$bb,$b0,$a6
	db $9d,$94,$8c,$84,$7c,$75,$6f,$69
	db $63,$5d,$58,$53,$4e,$4a,$46,$42
	db $3e,$3b,$37,$34,$31,$2e,$2c,$29
	db $27,$25,$23,$21,$1f,$1d,$1c,$1a
	db $19,$17,$16,$15,$14,$12
