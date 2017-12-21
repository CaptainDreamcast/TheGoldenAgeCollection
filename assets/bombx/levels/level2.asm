
 Level2:
 
	dc.l  level1_fieldA  ; map 	
	
	dc.w 60 ; time gain

	dc.w 0x1 ; player start x (16er t)
	dc.w 0x6 ; player start y (16er t)
 
	dc.w  0x1  ; boxAmount
	

	; per Box
	dc.w  0x3 ; x pos (16er tile)
	dc.w  0x6 ; y pos (16er tile)
	dc.w  13  ; timer value


level1_fieldA:
	INCBIN	"assets\sprites\level1_fieldA.bin"
level1_fieldA_end