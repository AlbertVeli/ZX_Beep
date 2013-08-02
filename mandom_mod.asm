;;; Mandom Mod (Gånglåt från Orsa)
;;;
;;; Old Swedish folk song, from a time long
;;; before the destructive age of copyright...
;;;
;;; (k) ALL RIGHTS REVERSED - Reprint what you like
;;;
;;; Prickle-Prickle, the 53rd day of Confusion in the YOLD 3179
;;;
;;; Albert Veli


;;; Data structure: enter tone data pairs as Pitch,Len.
;;;
;;; Len:   1 -> Whole note (1 s)
;;;        2 -> Half note (1/2 s)
;;;        3 -> Quarter note (1/4 s)
;;;        4 -> Eigth note (1/8 s)
;;;        5 -> Sixteenth note (1/16 s)
;;;        6 -> Thirty-second note (1/32 s)
;;;        etc
;;;
;;; Pitch: A2 = A number 2 on a piano
;;;
;;;  0 A2,  1 Bb,  2 B,  3 C,  4 C#,  5 D,  6 Eb,  7 E,  8 F,  9 F#,  10 G,  11 G#
;;; 12 A3, 13 Bb, 14 B, 15 C, 16 C#, 17 D, 18 Eb, 19 E, 20 F, 21 F#,  22 G,  23 G#
;;; 24 A4, 25 Bb, 26 B, 27 C, 28 C#, 29 D, 30 Eb, 31 E, 32 F, 33 F#,  34 G,  35 G#
;;;
;;; Example. 19,4 -> E3, Eigth note.

;;; Verse, part 1
_mandom_mod:
	;;      Am                                  F
	db	19,4, 0,4, 19,3, 0,3, 19,4, 0,4,    24,4, 8,5, 24,5, 8,4, 12,4, 24,4, 20,4, 12,4, 15,4
	;;      G7                                   C
	db	22,4, 10,4, 5,4, 20,4, 19,4, 10,4,   20,4, 10,4, 7,3, 3,4, 7,4, 15,4, 10,4, 19,4, 15,4
	;;      F                                                       C
	db	12,3,  8,4, 15,4, 12,5, 24,5, 12,5, 24,5, 10,4, 12,4,   10,4, 3,5, 10,5, 15,4, 3,4,  7,3, 10,4, 7,4
	;;	D7                                                     G
	db	7,3, 12,4, 15,4,  5,4, 21,4, 15,5, 21,5, 15,5, 21,5,   5,3, 14,5, 22,5, 24,5, 22,5, 5,3, 17,4, 24,4
_mandom_len:
	dw	($ - _mandom_mod) / 2

;;; Verse part 2
_mandom2_mod:
	;;      Am                                                    E7
	db	15,3, 0,5, 7,5, 12,4, 15,4, 19,4, 17,4, 15,5, 17,5,   14,4, 19,4, 14,3, 12,3, 11,4, 5,4
	;;	Am                                                  E
	db	12,4, 7,4, 14,4, 7,4, 15,4, 0,4, 17,4, 7,5, 12,5,   19,4, 14,4, 11,4, 2,4, 7,4, 19,5, 14,5, 11,4, 19,4
	;;      Am                                                               E7
	db	15,4, 0,4, 7,5, 0,5, 12,4, 15,4, 19,5, 12,5, 17,5, 12,5, 15,4,   14,4, 19,4, 14,4, 5,4, 12,4, 2,4, 11,4, 5,4
	;; 	Am
	db	12,4, 0,4, 7,4, 15,4, 12,4, 0,4, 24,5, 19,5, 0,4,   12,4, 0,4, 7,5, 24,5, 19,5, 15,5, 12,3, 24,4, 0,4
_mandom2_len:
	dw	($ - _mandom2_mod) / 2

;;; There is also a Verse, part 3, but it is not included.
;;; I like part 1 and 2 best. There is even a part 4, but
;;; it is an exact copy of part 2. No need for double info.

mandom_mod.play:
	ld	de, _mandom_mod
	ld	hl, _mandom_len	; Size matters
	call	engine.play
	;; Play verse again to build up an illusion of a long (kal) song
	ld	de, _mandom_mod
	ld	hl, _mandom_len
	call	engine.play
	;; Finally, second half of first half
	ld	de, _mandom2_mod
	ld	hl, _mandom2_len
	call	engine.play
	;; And first half again :)
	ld	de, _mandom_mod
	ld	hl, _mandom_len
	call	engine.play
	ld	de, _mandom2_mod
	ld	hl, _mandom2_len
	call	engine.play
	ld	de, _mandom_mod
	ld	hl, _mandom_len
	call	engine.play
	ld	de, _mandom_mod
	ld	hl, _mandom_len
	call	engine.play
	;; Ok, that's enough, return to caller
	ret
