;;;                   Fur Elise
;;;                  L.V. Beethoven
;;;
;;; (k) ALL RIGHTS REVERSED - Reprint what you like
;;;
;;;
;;; Setting Orange, the 64th day of Confusion in the YOLD 3179
;;;
;;; Albert Veli

	org	32768

start:
	ld	de, A1
	call	set_octave
	ld	de, fur_elise_1
	ld	hl, furelen1
	call	dual_song
	ld	de, fur_elise_1
	ld	hl, furelen1
	call	dual_song
	ld	de, fur_elise_2
	ld	hl, furelen2
	call	dual_song
	ld	de, fur_elise_1
	ld	hl, furelen1
	call	dual_song
	ld	de, fur_elise_2
	ld	hl, furelen2
	call	dual_song
	ld	de, fur_elise_1
	ld	hl, furelen1
	call	dual_song

	ret

;;; Pitch: 0 = C2, 12 = C3 etc
;;;
;;;  0 C,   1 C#,  2 D,	 3 Eb,	4 E,  5 F,  6 F#,  7 G,	 8 G#,	9 A, 10 Bb, 11 B
;;; 12 C,  13 C#, 14 D, 15 Eb, 16 E, 17 F, 18 F#, 19 G, 20 G#, 21 A, 22 Bb, 23 B
;;; 24 C,  25 C#, 26 D, 27 Eb, 28 E, 29 F, 30 F#, 31 G, 32 G#, 33 A, 34 Bb, 35 B
;;; 36 C,  37 C#, 38 D, 39 Eb, 40 E, 41 F, 42 F#, 43 G, 44 G#, 45 A, 46 Bb, 47 B

;;; http://www.youtube.com/watch?v=48eJZrhLA8Q

fur_elise_1:
	db	0,43,3, 0,42,3, 0,43,3, 0,42,3, 0,43,3, 0,38,3, 0,41,3, 0,39,3
	db	1,12,36,2, 1,19,36,2, 1,24,36,2
	db	0,27,3, 0,31,3, 0,36,3, 1,7,38,2, 1,19,38,2
	db	0,23,3, 0,31,3, 0,35,3, 0,38,3
	db	1,12,39,2, 1,19,39,2, 1,24,39,2, 0,31,3
	db	0,43,3, 0,42,3, 0,43,3, 0,42,3, 0,43,3, 0,38,3, 0,41,3, 0,39,3
	db	1,12,36,2, 1,19,36,2, 1,24,36,2
	db	0,27,3, 0,31,3, 0,36,3, 1,7,38,2, 1,19,38,2
	db	0,23,3, 0,31,3, 0,39,3, 0,38,3, 1,12,36,2, 1,19,36,2, 1,24,36,2
furelen1:
	dw	47

fur_elise_2:

	db	0,38,3, 0,39,3, 0,41,3
	db	1,15,43,2, 1,22,43,2, 1,27,43,2
	db	0,34,3, 0,44,3, 0,43,3
	db	1,10,41,2, 1,24,41,2, 1,26,41,2
	db	0,32,3, 0,43,3, 0,41,3
	db	1,12,39,2, 1,19,39,2, 1,24,39,2
	db	0,31,3, 0,41,3, 0,39,3
	db	1,7,38,2, 1,19,38,2
	db	0,31,3, 0,31,3, 0,43,3, 0,31,3, 0,43,3, 0,43,3, 0,55,3
	db	0,42,3, 0,43,3, 0,42,3, 0,43,3, 0,42,3
furelen2:
	dw	35


;;; HL = Pointer to songlen
;;; DE = Pointer to song notes
dual_song:
	ld	b, (hl)		; BC = songlen
	inc	hl
	ld	c, (hl)
	push	bc		; Save songlen on stack
	;; HL = DE (pointer to notes)
	push	de
	pop	hl
dual_loop:
	ld	a, (hl)
	inc	hl
	bit	0, a
	jr	z, single_note

;;; dual note
	push	bc
	ld	d, (hl)
	inc	hl
	ld	e, (hl)
	inc	hl
	ld	b, (hl)
	inc	hl
	push	hl
	call	dual_note
	pop	hl
	pop	bc
	djnz	dual_loop
	jp	byte_end
single_note:
	ld	d, (hl)
	inc	hl
	ld	a, (hl)
	inc	hl
	push	hl
	push	bc
	call	beeper
	pop	bc
	pop	hl
	djnz	dual_loop
byte_end:
	;; if c > 0, continue
	ld	a, 0
	cp	c
	jr	Z, dual_end
	;; b = 0 -> 256 more djnz iterations
	;; but first decrease c
	dec	c
	jp	song_loop
dual_end:
	pop	bc		; return songlen in BC
	ld	a, c		; little endian, swap BC
	ld	c, b
	ld	b, a
	ret

;;; dual_note
;;; D - pitch 1
;;; E - pitch 2
;;; B - duration 1=1/16, 2=2/16, 3=3/16 etc
;;; beeper
;;; D - pitch
;;; A - duration
dual_note:
	ld	c, 0
dn_loop:
	push	bc
	push	de
	;; p1
	ld	a, 6
	call	beeper
	;; p2
	pop	de
	push	de
	ld	d, e
	ld	a, 6
	call	beeper
	;; p1
	pop	de
	push	de
	ld	a, 7
	call	beeper
	;; p2
	pop	de
	push	de
	ld	d, e
	ld	a, 7
	call	beeper
	;; p1
	pop	de
	push	de
	ld	a, 7
	call	beeper
	;; p2
	pop	de
	push	de
	ld	d, e
	ld	a, 7
	call	beeper

	pop	de
	pop	bc
	djnz	dn_loop
	ret


	;; Include play_song/set_octave subroutines
	include "beep_song.asm"

	;; Include octaves data
	include "octaves.asm"
