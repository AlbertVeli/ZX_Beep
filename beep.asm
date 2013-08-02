;;; (k) ALL RIGHTS REVERSED - Reprint what you like
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
	ld	de, octave.A1
	call	sweep_octave
	ld	de, octave.A2
	call	sweep_octave
	ld	de, octave.A2
	call	sweepdown_octave
	ld	de, octave.A1
	call	sweepdown_octave

	;; Use A2 as base octave for song
	ld	de, octave.A2
	call	octave.set

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

	call	mandom_mod.play

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


	;; Include engine subroutines
	include "abengine.asm"

	;; Include sweep "special effect" data
	include "sweep.asm"

	;; And a song
	include "mandom_mod.asm"
