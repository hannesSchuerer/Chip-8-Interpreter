# Chip-8 Interpreter

I wanted to learn how an emulator, in this case an interpreter works.
After a bit of research I stumbled across the **Chip-8**.
For the first emulation project, the **Chip-8** is perfect.

## Resources I have used

* https://en.wikipedia.org/wiki/CHIP-8
* http://www.codeslinger.co.uk/pages/projects/chip8.html

## Requirements

* MinGW gcc compiler http://www.mingw.org/
* SDL2 https://www.libsdl.org/download-2.0.php

## How to run it

At first I need to say that any download of Chip-8 Roms is illegal.
So if you want to run any Roms you need to download them by yourself.

1. Go into the project folder and change the paths in the makefile to your SDL2 location.
1. Cd to your project folder in the CMD and run the command mingw32-make. This should build the program.
1. Now you can drag your Rom files at the .exe and the Interpreter should start up.

## Controls

The Chip-8 uses a hex keyboard for its user input.
The problem is that the original keyboard don't match with modern ones, so i mapped the keys appropriate to new keyboards (QWERTZ).
If you want to know what controls a specific game has, you should find it in the internet.

### Original keyboard mapping

1 | 2 | 3 | C
4 | 5 | 6 | D
7 | 8 | 9 | E
A | 0 | B | F

### Modern keyboard mapping

1 | 2 | 3 | 4
Q | W | R | T
A | S | D | F
Y | X | C | V

