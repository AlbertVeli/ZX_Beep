;;; (k) ALL RIGHTS REVERSED - Reprint what you like
;;;
;;; Setting Orange, the 64th day of Confusion in the YOLD 3179
;;;
;;; Albert Veli


;;; Octave in DE
set_octave:
	ld	hl, base_octave
	ld	(hl), e
	inc	hl
	ld	(hl), d
	ret

;;; HL = Pointer to songlen
;;; DE = Pointer to song notes
play_song:
	ld	b, (hl)		; BC = songlen
	inc	hl
	ld	c, (hl)
	push	bc		; Save songlen on stack
	;; HL = DE (pointer to notes)
	push	de
	pop	hl
song_loop:
	ld	d, (hl)
	inc	hl
	ld	a, (hl)
	inc	hl
	push	hl		; Save HL and BC
	push	bc
	call	beeper		; Play note
	pop	bc		; Restore HL and BC
	pop	hl
	djnz	song_loop
	;; if c > 0, decrease c and jump to song_loop
	ld	a, 0
	cp	c
	jr	Z, song_end
	;; b = 0 -> 256 more djnz iterations
	;; but first decrease c
	dec	c
	jp	song_loop
song_end:
	pop	bc		; return songlen in BC
	ld	a, c		; little endian, swap BC
	ld	c, b
	ld	b, a
	ret

;;; Base octave for songs.
;;; Call set_octave to set it.
base_octave:
	dw	0

;;; Arguments:
;;; D - number of semitones above base_octave (aka pitch)
;;; A - duration: 1 = 1s, 2 = 1/2s, 3 = 1/4s, 4 = 1/8s etc
beeper:
	;; Address for wanted row = A2 + 4 * D
	ld	l, d		; HL = 4 * D
	ld	h, 0
	add	hl, hl
	add	hl, hl
	ex	de, hl		; DE = HL
	;; Move everything up/down octaves by changing base_octave
	ld	hl, (base_octave)	; HL = address of first note in first octave

	add	hl, de		; HL = HL + DE
	;; Read 2 bytes into DE (since HL is busy), frequency
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
	push	de		; Push 2 bytes (destined for HL)
	;; Read 2 bytes into DE (tone cycles/second)
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	pop	hl		; Pop 2 bytes into HL
	;; Shift DE to the right A bits
shift_loop:
	sub	a, 1
	jr	Z, shift_done
	srl d			; D >>= 1, carry <- dropped bit
	rr e			; E >>= 1, bit 7 <- carry
	jp	shift_loop
shift_done:
	call	$03b5		; Call BEEPER routine in ROM
	ret
