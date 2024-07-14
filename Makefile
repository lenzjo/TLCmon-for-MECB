
%.bin : %.asm
	sbasm tlcmon.asm

all : tlcmon.bin

burn : tlcmon.bin
	minipro -p AT28C256 -w tlcmon.bin

clean :
	rm -f *.bin
	rm -f *.list
	
