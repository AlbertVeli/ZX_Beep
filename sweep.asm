;;; All Rights Reversed - No Rights Reserved
;;;
;;; Boomtime, the 51st day of Confusion in the YOLD 3179
;;;
;;; Albert Veli


;;; Play sweep. Octave in DE.
sweep_octave:
	call	set_octave
	ld	de, sweep
	ld	hl, sweep_len
	call	play_song
	ret

;;; Downward sweep. Octave in DE.
sweepdown_octave:
	call	set_octave
	ld	de, sweepdown
	ld	hl, sweepdown_len
	call	play_song
	ret

sweep:
	;; Sweep one octave
	db	$00,5, $01,5, $02,5, $03,5, $04,5, $05,5, $06,5, $07,5, $08,5, $09,5, $0a,5, $0b,5
sweep_len:
	;; Number of tones
	dw	($ - sweep) / 2

sweepdown:
	;; One octave, downwards
	db	$0b,5, $0a,5, $09,5, $08,5, $07,5, $06,5, $05,5, $04,5, $03,5, $02,5, $01,5, $00,5
sweepdown_len:
	dw	($ - sweepdown) / 2
