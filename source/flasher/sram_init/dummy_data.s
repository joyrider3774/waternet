; Character: 4 banks of 8k (0x2000) filled with 0xff because flasher always uses multiple of 4 rom banks because of 64k sector size of the chip

.area _CODE_4

.rept 0x4000
	.db 0xff
.endm

.area _CODE_5

.rept 0x4000 
	.db 0xff
.endm

.area _CODE_6

.rept 0x4000 
	.db 0xff
.endm

.area _CODE_7

.rept 0x4000 
	.db 0xff
.endm