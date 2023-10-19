run: libmos6502.so emulator a1basic.bin wozmon.bin
	LD_LIBRARY_PATH=. ./emulator

clean:
	rm -f emulator libmos6502.so *.bin

emulator: main.cpp loadfile.c
	gcc $^ -L. -lmos6502 -o $@

libmos6502.so: mos6502.cpp
	gcc -shared -fPIC $^ -o $@

%.bin: %.s
	vasm6502_oldstyle $^ -Fbin -dotdir -o $@
