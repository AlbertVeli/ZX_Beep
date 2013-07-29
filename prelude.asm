;;;	    Prelude No. 1 in C Major BWV 846
;;;			J.S. Bach
;;;
;;; (k) ALL RIGHTS REVERSED - Reprint what you like
;;;
;;;
;;; Setting Orange, the 64th day of Confusion in the YOLD 3179
;;;
;;; Albert Veli

	org	32768

start:
	ld	de, C2
	call	set_octave
	call	play_prelude
	ret


;;; Pitch: 0 = C2, 12 = C3 etc
;;;
;;;  0 C,   1 C#,  2 D,	 3 Eb,	4 E,  5 F,  6 F#,  7 G,	 8 G#,	9 A, 10 Bb, 11 B
;;; 12 C,  13 C#, 14 D, 15 Eb, 16 E, 17 F, 18 F#, 19 G, 20 G#, 21 A, 22 Bb, 23 B
;;; 24 C,  25 C#, 26 D, 27 Eb, 28 E, 29 F, 30 F#, 31 G, 32 G#, 33 A, 34 Bb, 35 B
;;; 36 C,  37 C#, 38 D, 39 Eb, 40 E, 41 F, 42 F#, 43 G, 44 G#, 45 A, 46 Bb, 47 B


;;; http://www.youtube.com/watch?v=vKABgs7CGP0

prel_1:
	db	24,4, 28,4, 31,4, 36,4, 40,4, 31,4, 36,4, 40,4
prel_2:
	db	24,4, 26,4, 33,4, 38,4, 41,4, 33,4, 38,4, 41,4
prel_3:
	db	23,4, 26,4, 31,4, 38,4, 41,4, 31,4, 38,4, 41,4
;;; 4 same as 1
prel_5:
	db	24,4, 28,4, 33,4, 40,4, 45,4, 33,4, 40,4, 45,4
prel_6:
	db	24,4, 26,4, 30,4, 33,4, 38,4, 30,4, 33,4, 38,4
prel_7:
	db	23,4, 26,4, 31,4, 38,4, 43,4, 31,4, 38,4, 43,4
prel_8:
	db	23,4, 24,4, 28,4, 31,4, 36,4, 28,4, 31,4, 36,4
prel_9:
	db	21,4, 24,4, 28,4, 31,4, 36,4, 28,4, 31,4, 36,4
prel_10:
	db	14,4, 21,4, 26,4, 30,4, 36,4, 26,4, 30,4, 36,4
prel_11:
	db	19,4, 23,4, 26,4, 31,4, 35,4, 26,4, 31,4, 35,4
prel_12:
	db	19,4, 22,4, 28,4, 31,4, 37,4, 28,4, 31,4, 37,4
prel_13:
	db	17,4, 21,4, 26,4, 33,4, 38,4, 26,4, 33,4, 38,4
prel_14:
	db	17,4, 20,4, 26,4, 29,4, 34,4, 26,4, 29,4, 34,4
prel_15:
	db	15,4, 19,4, 24,4, 31,4, 36,4, 24,4, 31,4, 36,4
prel_16:
	db	15,4, 17,4, 21,4, 24,4, 28,4, 21,4, 24,4, 28,4
prel_17:
	db	14,4, 16,4, 21,4, 24,4, 29,4, 21,4, 24,4, 29,4
prel_18:
	db	 7,4, 14,4, 19,4, 23,4, 29,4, 19,4, 23,4, 29,4
prel_19:
	db	12,4, 16,4, 19,4, 24,4, 28,4, 19,4, 24,4, 28,4
prel_20:
	db	12,4, 19,4, 22,4, 24,4, 28,4, 22,4, 24,4, 28,4
prel_21:
	db	 5,4, 17,4, 19,4, 24,4, 28,4, 19,4, 24,4, 28,4
prel_22:
	db	 6,4, 12,4, 21,4, 24,4, 27,4, 21,4, 24,4, 27,4
prel_23:
	db	 8,4, 18,4, 23,4, 24,4, 26,4, 23,4, 24,4, 26,4
prel_24:
	db	 7,4, 17,4, 19,4, 23,4, 26,4, 19,4, 23,4, 26,4
prel_25:
	db	 7,4, 16,4, 19,4, 24,4, 28,4, 19,4, 24,4, 28,4
prel_26:
	db	 7,4, 14,4, 19,4, 24,4, 29,4, 19,4, 24,4, 29,4
prel_27:
	db	 7,4, 14,4, 19,4, 23,4, 29,4, 19,4, 23,4, 29,4
prel_28:
	db	 7,4, 15,4, 21,4, 24,4, 30,4, 21,4, 24,4, 30,4
prel_29:
	db	 7,4, 16,4, 19,4, 24,4, 31,4, 19,4, 24,4, 31,4
prel_30:
	db	 7,4, 14,4, 19,4, 24,4, 29,4, 19,4, 24,4, 29,4
prel_31:
	db	 7,4, 14,4, 19,4, 23,4, 29,4, 19,4, 23,4, 29,4
prel_32:
	db	 0,4, 12,4, 19,4, 22,4, 28,4, 19,4, 22,4, 28,4
prel_post:
 	db	 0,4, 12,4, 17,4, 21,4, 24,4, 29,4, 24,4, 21,4
	db	24,4, 21,4, 17,4, 21,4, 17,4, 14,4, 17,4, 14,4
 	db	 0,4,  9,4, 31,4, 35,4, 38,4, 41,4, 38,4, 35,4
	db	38,4, 35,4, 31,4, 35,4, 26,4, 29,4, 28,4, 27,4
	db	 0,3, 24,4, 12,4, 0,5, 12,5, 28,5, 31,5, 36,5, 31,5, 28,5, 12,2
prelpost_len:
	dw	($ - prel_post) / 2


;;; Each row above is 8 notes
rowlen:	dw	8

double_play:
	push	de
	ld	hl, rowlen
	call	play_song
	pop	de
	ld	hl, rowlen
	call	play_song
	ret

play_prelude:
	ld	de, prel_1
	call	double_play
	ld	de, prel_2
	call	double_play
	ld	de, prel_3
	call	double_play
	ld	de, prel_1
	call	double_play
	ld	de, prel_5
	call	double_play
	ld	de, prel_6
	call	double_play
	ld	de, prel_7
	call	double_play
	ld	de, prel_8
	call	double_play
	ld	de, prel_9
	call	double_play
	ld	de, prel_10
	call	double_play
	ld	de, prel_11
	call	double_play
	ld	de, prel_12
	call	double_play
	ld	de, prel_13
	call	double_play
	ld	de, prel_14
	call	double_play
	ld	de, prel_15
	call	double_play
	ld	de, prel_16
	call	double_play
	ld	de, prel_17
	call	double_play
	ld	de, prel_18
	call	double_play
	ld	de, prel_19
	call	double_play
	ld	de, prel_20
	call	double_play
	ld	de, prel_21
	call	double_play
	ld	de, prel_22
	call	double_play
	ld	de, prel_23
	call	double_play
	ld	de, prel_24
	call	double_play
	ld	de, prel_25
	call	double_play
	ld	de, prel_26
	call	double_play
	ld	de, prel_27
	call	double_play
	ld	de, prel_28
	call	double_play
	ld	de, prel_29
	call	double_play
	ld	de, prel_30
	call	double_play
	ld	de, prel_31
	call	double_play
	ld	de, prel_32
	call	double_play
	;; Last part
	ld	de, prel_post
	ld	hl, prelpost_len
	call	play_song

	ret


	;; Include play_song/set_octave subroutines
	include "beep_song.asm"

	;; Include octaves data
	include "octaves_c.asm"
