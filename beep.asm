;;; All Rights Reversed - No Rights Reserved
;;;
;;; Boomtime, the 51st day of Confusion in the YOLD 3179
;;;
;;; Albert Veli


;;; bin2tap will carefully place the code at 32768.
;;; Communicate this to the assembler so it can
;;; calculate absolute addresses, if needed.
	org	32768

;;; entry point
start:
	;; Sweep the dance floory, go speccy!
	ld	de, A1
	call	sweep_octave
	ld	de, A2
	call	sweep_octave
	ld	de, A2
	call	sweepdown_octave
	ld	de, A1
	call	sweepdown_octave

	;; Use A2 as base octave for song
	ld	de, A2
	call	set_octave

	;; Ready, Set ...
	;;    ,
	;;    |\      __
	;;    | |    |--|           __
	;;    |/     |  |          |~'
	;;   /|_    () ()          |
	;;  //| \          |\    ()
	;; | \|_ |         | \
	;;  \_|_/         ()  |
	;;    |               |
	;;   @'              ()
	;;
	;;
	;;            oOo

	call	play_mandom_mod

	;; Sweep baby, baby
	ld	de, A7
	call	sweepdown_octave
	ld	de, A6
	call	sweepdown_octave
	ld	de, A5
	call	sweepdown_octave
	ld	de, A4
	call	sweepdown_octave
	ld	de, A3
	call	sweepdown_octave
	ld	de, A2
	call	sweepdown_octave
	ld	de, A1
	call	sweepdown_octave

	;; Finish with some armagheddonly dark tones
	;;
	;;         o_0
	;;
	;; (freqs -j -o 2 13.75)
	ld	hl, $1ef5
	ld	de, $0018
	call	$03b5
	ld	hl, $2951
	ld	de, $0014
	call	$03b5
	ld	hl, $3e08
	ld	de, $001b
	call	$03b5
	ld	hl, $52bf
	ld	de, $0015
	call	$03b5
	ld	hl, $7c2d
	ld	de, $001c
	call	$03b5

	ret	;; Return to basic... Bye.


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


;;; Base octave for songs, default A2 (Second A on piano).
;;; Call set_octave to change it.
base_octave:
	dw	A2


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


	;; Include octaves data
	include "octaves.asm"

	;; Include sweep "special effect" data
	include "sweep.asm"

	;; And a song
	include "mandom_mod.asm"
