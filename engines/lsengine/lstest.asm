	;; org $c000
	org $8000

begin:
	ld	hl, musicData
	call	lsengine.play
	jp	begin
	ret

	include "lsengine.asm"

musicData:
	include "music.asm"

;; .loop:
;; 	dw .pattern0
;; 	dw 0
;; 	dw .loop
;; .pattern0:
;; 	db 4,7	;length of pattern (0=256), speed
;; 	db 10,0,20,0
;; 	db 0,0,0,0
