# Apple 1 Emulator
This is an Apple 1 emulator made using [this really cool 6502 emulator](https://github.com/gianlucag/mos6502).

You can run it by executing
```
make
```
This will compile the emulator and load it with WozMon and Apple I BASIC, written by Steve Wozniak. It is also possible to run other programs by typing them into WozMon or BASIC.

When the Emulator starts up, you should get a `\` character. This means that WozMon is running. To launch BASIC from WozMon, run the command:
```
E000R<enter>
```
This will run the program loaded at 0xE000 in memory, in this case BASIC. To return to WozMon, run the following command in BASIC:
```
CALL -256<enter>
```
This will run the program loaded at 0xFF00 in memory. 