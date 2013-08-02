;;; All Rights Reversed - No Rights Reserved
;;;
;;; Boomtime, the 51st day of Confusion in the YOLD 3179
;;;
;;; Albert Veli


;;; Play sweep. Octave in DE.
sweep_octave:
	call	octave.set
	ld	de, _sweep
	ld	hl, _sweep_len
	call	engine.play
	ret

;;; Downward sweep. Octave in DE.
sweepdown_octave:
	call	octave.set
	ld	de, _sweepdown
	ld	hl, _sweepdown_len
	call	engine.play
	ret

_sweep:
	;; Sweep one octave
	db	$00,5, $01,5, $02,5, $03,5, $04,5, $05,5, $06,5, $07,5, $08,5, $09,5, $0a,5, $0b,5
_sweep_len:
	;; Number of tones
	dw	($ - _sweep) / 2

_sweepdown:
	;; One octave, downwards
	db	$0b,5, $0a,5, $09,5, $08,5, $07,5, $06,5, $05,5, $04,5, $03,5, $02,5, $01,5, $00,5
_sweepdown_len:
	dw	($ - _sweepdown) / 2
