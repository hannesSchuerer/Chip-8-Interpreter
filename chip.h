#ifndef CHIP_H
#define CHIP_H

#include "window.h"

typedef struct chip{
	
	unsigned char memory[4096];
	
	unsigned short opcode;
	unsigned short pc;
	unsigned short stack[16];
	unsigned short sp;
	
	unsigned char gfx[64][32];
	
	unsigned char V[16];
	unsigned short I;
	
	unsigned char key[16];
	
	unsigned char delayTimer;
	unsigned char soundTimer;
	
}Chip;

void chipInitialize(Chip *chip);
void loadFontInMemory(Chip *chip);
void loadProgramInMemory(Chip *chip, char *program);
void executionCycle(Chip *chip);
void checkKeyState(Chip *chip, const uint8_t *state);
void decreaseTimers(Chip *chip);

#endif