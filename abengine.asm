;;; (k) ALL RIGHTS REVERSED - Reprint what you like
;;;
;;; Setting Orange, the 64th day of Confusion in the YOLD 3179
;;;
;;; Albert Veli


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
	ld	hl, (_base_octave)	; HL = address of first note in first octave

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
	call	single_beep	; Call BEEPER routine in ROM
	ret


;;; From Spectrum 48k ROM ($03b5)
;;;
;;; DE = #cycles - 1
;;; HL = tone period
;;;
;;; The tone period is measured in T states and consists of
;;; three parts: a coarse part (H register), a medium part
;;; (bits 7..2 of L) and a fine part (bits 1..0 of L) which
;;; contribute to the waveform timing as follows:
;;;
;;;                          coarse     medium        fine
;;; duration of low  = 118 + 1024 * H + 16 * (L>>2) + 4 * (L & 3)
;;; duration of hi   = 118 + 1024 * H + 16 * (L>>2) + 4 * (L & 3)
;;;
;;; Tp = tone period = 236 + 2048 * H + 32 * (L>>2) + 8 * (L & 3)
;;;                  = 236 + 2048 * H + 8 * L = 236 + 8 * HL
;;;
single_beep:
	di			; Disable Interrupts so they don't disturb timing
	ld	a, l
	srl	l
	srl	l		; L >>= 2, medium part of tone period
	cpl
	and	$03		; A = L&3 = fine part of tone period
	ld	c, a		; bc = 0-3
	ld	b, $00
	ld	ix, ix_plus3	; IX = ix_plus3 + bc (0-3)
	add	ix, bc		;   IX holds address of entry into the loop
				;   the loop will contain 0-3 NOPs, implementing
				;   the fine part of the tone period.

	;; A = val to write to beeper port
	;; bits 0-2 = border colour
	;; bit 3 = loud sound output
	;; bit 4 = high or low pulse
	ld	a, ($5C48)	; BORDCR
	and	$38		; bits 5..3 contain border colour
	rrca			; border colour bits moved to 2..0
	rrca			;   to match border bits on port #FE
	rrca			;
	or	$08		; bit 3 set (tape output bit on port #FE)
				;   for loud sound output
ix_plus3:
	nop		  ;(4)	; optionally executed NOPs for small
	nop		  ;(4)	;   adjustments to tone period
	nop		  ;(4)
;;; ix_plus0:
	inc	b	  ;(4)
	inc	c	  ;(4)

	;; hl_loop will loop $3f*b + c times = ~2048 * b t-states
hl_lp:
	dec	c	  ;(4)	; timing loop for duration of
	jr	nz, hl_lp ;(12/7) high or low pulse of waveform

	ld	c, $3f	  ;(7)
	dec	b	  ;(4)
	jp	nz, hl_lp ;(10) ; to H&L loop

	xor	$17	  ;(7)	; toggle output beep bit (and border)
	out	($fe), a  ;(11)	; output pulse

	ld	b, h	  ;(4)	; B = coarse part of tone period
	ld	c, a	  ;(4)	; save port ($fe) output byte in C
	bit	4, a	  ;(8)	; if new output bit is high, go
	jr	nz, again ;(12/7)   to again

	ld	a, d	  ;(4)	; one cycle of waveform has completed
	or	e	  ;(4)	;   (low->low). if cycle countdown = 0
	jr	z, end	  ;(12/7)   go to end

	ld	a, c	  ;(4)	; restore output byte for port #FE
	ld	c, l	  ;(4)	; C = medium part of tone period
	dec	de	  ;(6)	; decrement cycle count
	jp	(ix)	  ;(8)	; do another cycle

again:				; halfway through cycle
	ld	c, l	  ;(4)	; C = medium part of tone period
	inc	c	  ;(4)	; adds 16 cycles to make duration of high = duration of low
	jp	(ix)	  ;(8)	; do high pulse of tone

end:
	ei			; Enable Interrupts
	ret


	;; Include octaves data
	include "octave.asm"
