# TLCmon for MECB v1.3.9

## What is it?

It's a basic monitor program for the 65c02. It is far from finished but it is 
in a usable state and so I am making it public now.

## Setup

I wrote this using [SBASM](https://www.sbprojects.net/sbasm/). It is written in Python 3 and so works on all platforms. You can use it to write and assemble for AVR, PIC and most of the "popular" cpus such as 65xx, 68xx, z8x, 80xx and more, even the SC/MP! Did I say it is FREE as well. 

This is set up for 48k ram and 16k of rom. IO starts at $C000 and is $10 bytes per device as I want to add a 6522 and a uart that has 16 regs. To change it for your setup, edit lines 59-67 of tlcmon.asm.

Compilation is easy enough just type make then if you use minipro type make burn to burn the eeprom.

## Misc.

TLCmon uses a vector table for both NMI and INT interrupts. Usable ram starts at $400.

## Syntax

1. Numbers can be entered in hex, binary or decimal. Hex numbers have the $ prefix, binary numbers have the % prefix. Any number entered without either prefix will be recognized as a decimal number.

2. TLCmon is not case sensitive, so you can use upper, lower or mixed case when 
   entering commands.

3. Delimiters : Where a command uses arguements, you must use a space before the
   first arguement. You can use a space, comma or colon to seperate arguements.

## Commands

**PEEK** *addr* : Examples. peek 1234, peek $fe89, peek %1010011101101110
This returns the hex byte residing at *addr*.

**POKE** *addr byte* : Examples. poke $FE34,10, poke 12345 %10101100
This writes *byte* to location *addr*. It cannot write to rom and will warn you if you try.

**DUMP** *faddr taddr*  : Examples. dump $1001 $1030, dump 6547
This will write a hex dump to the screen starting at *faddr* and finishing at *taddr*. If you do not supply *taddr* it will finish at *faddr*+ $100.

**EDMEM** *faddr*  : Examples. edmem 6782
This will let you write sequential bytes starting at *faddr*. Bytes are entered as 
hex ONLY and you do not use the $ signifier. As you enter two digits it will auto-
matically advance to the next addr. To end the sequence press either the escape
or return key.

**COPY**  *faddr taddr daddr* : Example copy 53290 63990 %7000
This will let you copy a block of memory (*faddr*-*taddr*) to another location (*daddr*)
as long as does not overlap rom. Note: It does not and cannot check for overwriting
IO space.

**FILL** *faddr taddr byte*  : Example. fill 321e $3ac1 42
This will let you fill a region of memory (*faddr*-*taddr*) with a byte. Like copy, fill
will not allow you to overwrite rom space.

**GO** *addr*  : Example go 1050, go 400
This will attempt to execute instructions starting at *addr*. They should be terminated 
with an RTS instruction ($60) to return to TLCmon.

**HELP** : Example. help, peek ?
HELP will display all available commands, and their format, on screen. If you type a ?
after a command it will show that commands help info.

**HEX - DEC - BIN** : Examples. hex 31087, dec %10101011001, bin 6574
These 3 cmds operate in a similar manner. For example for hex you can enter a decimal or binary number and it will return the hex equivalent. So for dec you can enter a hex or binary number and for bin you can enter a hex or binary number.

## What's next?

I am working adding file load and save, register display and tracing/single-stepping. I
would also like to add disassembly.
