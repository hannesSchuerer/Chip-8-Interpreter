#include "chip.h"

void executionCycle(Chip *chip)
{
	chip->opcode = chip->memory[chip->pc] << 8 | chip->memory[chip->pc+1];	
	
	switch(chip->opcode & 0xF000)
	{
		case 0x0000:
			switch(chip->opcode & 0x00FF)
			{
				case 0x00E0:				// CLS Clear the display
					for(int i = 0; i < 64; i++)
						for(int j = 0; j < 32; j++)
							chip->gfx[i][j] = 0x00;
					chip->pc += 2;
					break;
				case 0x00EE:				// RET Return from a subroutine
					chip->sp -= 1;
					chip->pc = chip->stack[chip->sp];
					chip->pc += 2;
					break;
					
				default:
					printf("Invalid instruction: %x\n", chip->opcode);
					exit(1);
			}
			break;
			
		case 0x1000:						// JP addr Jump to location nnn
			chip->pc = chip->opcode & 0x0FFF;
			break;
			
		case 0x2000:						// Call addr Call subroutine at nnn
			chip->stack[chip->sp] = chip->pc;
			chip->sp += 1;
			chip->pc = chip->opcode & 0x0FFF;
			break;
			
		case 0x3000:						// SE Vx, byte Skip next instruction if Vx == kk
			if(chip->V[(chip->opcode & 0x0F00) >> 8] == (chip->opcode & 0x00FF))
				chip->pc += 4;				// Skips one instruction
			else
				chip->pc += 2;
			break;
		
		case 0x4000:						// SNE Vx, byte Skip next instruction if Vx != kk
			if(chip->V[(chip->opcode & 0x0F00) >> 8] != (chip->opcode & 0x00FF))
				chip->pc += 4;				// Skips one instruction
			else
				chip->pc += 2;
			break;
			
		case 0x5000:						// SE Vx, Vy Skip the next instruction if Vx == Vy
			if(chip->V[(chip->opcode & 0x0F00) >> 8] == chip->V[(chip->opcode & 0x00F0) >> 4])
				chip->pc += 4;
			else
				chip->pc += 2;
			break;
			
		case 0x6000:						// LD Vx, byte Set Vx = kk
			chip->V[(chip->opcode & 0x0F00) >> 8] = (chip->opcode & 0x00FF);
			chip->pc += 2;
			break;
			
		case 0x7000:						// ADD Vx, byte Set Vx = Vx + kk
			chip->V[(chip->opcode & 0x0F00) >> 8] += (chip->opcode & 0x00FF);
			chip->pc += 2;
			break;
			
		case 0x8000:
			switch(chip->opcode & 0x000F)
			{
				case 0x0000:				// LD Vx, Vy Set Vx = Vy
					chip->V[(chip->opcode & 0x0F00) >> 8] = chip->V[(chip->opcode & 0x00F0) >> 4];
					chip->pc += 2;
					break;
				
				case 0x0001:				// OR Vx, Vy Set Vx = Vx OR Vy
					chip->V[(chip->opcode & 0x0F00) >> 8] |= chip->V[(chip->opcode & 0x00F0) >> 4];
					chip->pc += 2;
					break;
				
				case 0x0002:				// AND Vx, Vy Set Vx = Vx AND Vy
					chip->V[(chip->opcode & 0x0F00) >> 8] &= chip->V[(chip->opcode & 0x00F0) >> 4];
					chip->pc += 2;
					break;
				
				case 0x0003: 				// XOR Vx, Vy Set Vx = Vx XOR Vy
					chip->V[(chip->opcode & 0x0F00) >> 8] ^= chip->V[(chip->opcode & 0x00F0) >> 4];
					chip->pc += 2;
					break;
					
				case 0x0004:				// ADD Vx, Vy Set Vx = Vx + Vy, set VF = carry
					chip->V[(chip->opcode & 0x0F00) >> 8] += chip->V[(chip->opcode & 0x00F0) >> 4];
					if(chip->V[(chip->opcode & 0x00F0) >> 4] > (0xFF - chip->V[(chip->opcode & 0x0F00) >> 8]))
						chip->V[0x0F] = 0x01;
					else
						chip->V[0x0F] = 0x00;
					chip->pc += 2;
					break;
				
				case 0x0005:				// SUB Vx, Vy Set Vx = Vx - Vy, set VF = NOT borrow						May not work!
					if(chip->V[(chip->opcode & 0x0F00) >> 8] < chip->V[(chip->opcode & 0x00F0) >> 4])
						chip->V[0x0F] = 0x00;
					else
						chip->V[0x0F] = 0x01;
					chip->V[(chip->opcode & 0x0F00) >> 8] -= chip->V[(chip->opcode & 0x00F0) >> 4];
					chip->pc += 2;
					break;
				
				case 0x0006:				// SHR Vx {, Vy} Set Vx = Vx SHR 1										May not work!
					chip->V[0x0F] = (chip->V[(chip->opcode & 0x0F00) >> 8] & 0x01);
					chip->V[(chip->opcode & 0x0F00) >> 8] >>= 1;
					chip->pc += 2;
					break;
				
				case 0x0007:				// SUBN Vx, Vy Set Vx = Vy - Vx, set VF = NOT borrow					May not work!
					if(chip->V[(chip->opcode & 0x00F0) >> 4] < chip->V[(chip->opcode & 0x0F00) >> 8])
						chip->V[0x0F] = 0x00;
					else
						chip->V[0x0F] = 0x01;
					chip->V[(chip->opcode & 0x0F00) >> 8] = chip->V[(chip->opcode & 0x00F0) >> 4] - chip->V[(chip->opcode & 0x0F00) >> 8];
					chip->pc += 2;
					break;
				
				case 0x000E:				// SHL Vx {, Vy} Set Vx = Vx SHL 1										May not work!
					chip->V[0x0F] = (chip->V[((chip->opcode & 0x0F00) >> 8)] >> 7);
					chip->V[(chip->opcode & 0x0F00) >> 8] <<= 1;
					chip->pc += 2;
					break;
				
				default:
					printf("Invalid instruction: %x\n", chip->opcode);
					exit(1);
			}
			break;
			
		case 0x9000:						// SNE Vx, Vy Skip next instruction if Vx != Vy
			if(chip->V[(chip->opcode & 0x0F00) >> 8] != chip->V[(chip->opcode & 0x00F0) >> 4])
				chip->pc += 4;
			else
				chip->pc += 2;
			break;
			
		case 0xA000:						// LD I, addr Set I = nnn
			chip->I = (chip->opcode & 0x0FFF);
			chip->pc += 2;
			break;
			
		case 0xB000:						// JP V0, addr Jump to location nnn + V0
			chip->pc = chip->V[0x00] + (chip->opcode & 0x0FFF);
			break;
			
		case 0xC000:						// RND Vx, byte Set Vx = random byte AND kk
			chip->V[(chip->opcode & 0x0F00) >> 8] = rand() & (chip->opcode & 0x00FF);
			chip->pc += 2;
			break;
			
		case 0xD000:						// DRW Vx, Vy, nibble Display n-byte sprite starting at memory location I at (Vx, Vy), VF = collision (May not work)
		{
			unsigned char xCoord = chip->V[(chip->opcode & 0x0F00) >> 8];
			unsigned char yCoord = chip->V[(chip->opcode & 0x00F0) >> 4];
			unsigned char n = (chip->opcode & 0x000F);
			
			chip->V[0x0F] = 0x00;
			
			for(int i = 0; i < n; i++)
			{
				unsigned char pixelLine = chip->memory[chip->I + i];
				int xPixelLine = 7;
				
				for(int j = 0; j < 8; j++, xPixelLine--)
				{
					int mask = 1 << xPixelLine;
					if(pixelLine & mask)
					{
						if(chip->gfx[xCoord + j][yCoord + i] > 0)
							chip->V[0x0F] = 0x01;
						chip->gfx[xCoord + j][yCoord + i] ^= 1;
					}
				}
				
			}
			chip->pc += 2;
			break;
		}
		
		case 0xE000:						
			switch(chip->opcode & 0x00FF)
			{
				case 0x009E:				// SKP Vx Skip next instruction if key with the value of Vx is pressed
					if(chip->key[chip->V[(chip->opcode & 0x0F00) >> 8]] != 0)
						chip->pc += 4;
					else
						chip->pc += 2;
					break;
				
				case 0x00A1:				// SKNP Vx Skip next instruction if key with the value Vx is not pressed
					if(chip->key[chip->V[(chip->opcode & 0x0F00) >> 8]] == 0)
						chip->pc += 4;
					else
						chip->pc += 2;
					break;
				
				default:
					printf("Invalid instruction: %x\n", chip->opcode);
					exit(1);
			}
			break;
			
		case 0xF000:						
			switch(chip->opcode & 0x00FF)
			{
				case 0x0007:				// LD Vx, DT Set Vx = delay timer value
					chip->V[(chip->opcode & 0x0F00) >> 8] = chip->delayTimer;
					chip->pc += 2;
					break;
				
				case 0x000A:				// LD Vx, K Wait for a key press, store the value of the key in Vx 			(May not work)
					for(int i = 0; i < 16; ++i)
					{
						if(chip->key[i] != 0)
						{
							chip->V[(chip->opcode & 0x0F00) >> 8] = i;
							chip->pc += 2;
						}
					}
					break;
				
				case 0x0015:				// LD DT, Vx Set delay timer = Vx
					chip->delayTimer = chip->V[(chip->opcode & 0x0F00) >> 8];
					chip->pc += 2;
					break;
				
				case 0x0018:				// LD ST, Vx Set sound timer = Vx
					chip->soundTimer = chip->V[(chip->opcode & 0x0F00) >> 8];
					chip->pc += 2;
					break;
					
				case 0x001E:				// ADD I, Vx Set I = I + Vx													(May not work)
					chip->I += chip->V[(chip->opcode & 0x0F00) >> 8];
					chip->pc += 2;
					break;
				
				case 0x0029:				// LD F, Vx Set I = location of sprite for digit Vx							(May not work)
					chip->I = chip->V[(chip->opcode & 0x0F00) >> 8] * 0x05;
					chip->pc += 2;
					break;
				
				case 0x0033:				// LD B, Vx Store BCD representation of Vx in memory locations I, I+1, I+2
					chip->memory[chip->I] = chip->V[(chip->opcode & 0x0F00) >> 8] / 100;
					chip->memory[chip->I + 1] = (chip->V[(chip->opcode & 0x0F00) >> 8] / 10) % 10;
					chip->memory[chip->I + 2] = (chip->V[(chip->opcode & 0x0F00) >> 8]) % 10;
					chip->pc += 2;
					break;
				
				case 0x0055:				// LD [I], Vx Store registers V0 through Vx in memory starting at location I
					for(int i = 0; i <= ((chip->opcode & 0x0F00) >> 8); i++)
						chip->memory[chip->I + i] = chip->V[i];
					//chip->I = chip->I + ((chip->opcode & 0x0F00) >> 8) + 1;
					chip->pc += 2;
					break;
				
				case 0x0065:
					for(int i = 0; i <= ((chip->opcode & 0x0F00) >> 8); i++)
						chip->V[i] = chip->memory[chip->I + i];
					//chip->I = chip->I + ((chip->opcode & 0x0F00) >> 8) + 1;
					chip->pc += 2;
					break;
				
				default:
					printf("Invalid instruction: %x\n", chip->opcode);
					exit(1);
				
			}
			break;
			
		default:
			printf("Invalid instruction: %x\n", chip->opcode);
			exit(1);
	}
}

void decreaseTimers(Chip *chip)
{
	if(chip->delayTimer > 0)
		chip->delayTimer -= 1;
	
	if(chip->soundTimer > 0)
	{
		if(chip->soundTimer == 1)
		chip->soundTimer -= 1;
	}
}

void checkKeyState(Chip *chip, const uint8_t *state)
{
	if(state[SDL_SCANCODE_1])		// Check if 1 is pressed
		chip->key[0x0] = 0x1;
	else
		chip->key[0x0] = 0x0;
	
	if(state[SDL_SCANCODE_2])		// Check if 2 is pressed
		chip->key[0x1] = 0x1;
	else
		chip->key[0x1] = 0x0;
	
	if(state[SDL_SCANCODE_3])		// Check if 3 is pressed
		chip->key[0x2] = 0x1;
	else
		chip->key[0x2] = 0x0;
	
	if(state[SDL_SCANCODE_4])		// Check if 4 is pressed
		chip->key[0x3] = 0x1;
	else
		chip->key[0x3] = 0x0;
	
	if(state[SDL_SCANCODE_Q])		// Check if Q is pressed
		chip->key[0x4] = 0x1;
	else
		chip->key[0x4] = 0x0;
	
	if(state[SDL_SCANCODE_W])		// Check if W is pressed
		chip->key[0x5] = 0x1;
	else
		chip->key[0x5] = 0x0;
	
	if(state[SDL_SCANCODE_E])		// Check if E is pressed
		chip->key[0x6] = 0x1;
	else
		chip->key[0x6] = 0x0;
	
	if(state[SDL_SCANCODE_R])		// Check if R is pressed
		chip->key[0x7] = 0x1;
	else
		chip->key[0x7] = 0x0;
	
	if(state[SDL_SCANCODE_A])		// Check if A is pressed
		chip->key[0x8] = 0x1;
	else
		chip->key[0x8] = 0x0;
	
	if(state[SDL_SCANCODE_S])		// Check if S is pressed
		chip->key[0x9] = 0x1;
	else
		chip->key[0x9] = 0x0;
	
	if(state[SDL_SCANCODE_D])		// Check if D is pressed
		chip->key[0xA] = 0x1;
	else
		chip->key[0xA] = 0x0;
	
	if(state[SDL_SCANCODE_F])		// Check if F is pressed
		chip->key[0xB] = 0x1;
	else
		chip->key[0xB] = 0x0;
	
	if(state[SDL_SCANCODE_Y])		// Check if Y is pressed
		chip->key[0xC] = 0x1;
	else
		chip->key[0xC] = 0x0;
	
	if(state[SDL_SCANCODE_X])		// Check if X is pressed
		chip->key[0xD] = 0x1;
	else
		chip->key[0xD] = 0x0;
	
	if(state[SDL_SCANCODE_C])		// Check if C is pressed
		chip->key[0xE] = 0x1;
	else
		chip->key[0xE] = 0x0;
	
	if(state[SDL_SCANCODE_V])		// Check if V is pressed
		chip->key[0xF] = 0x1;
	else
		chip->key[0xF] = 0x0;
}

void chipInitialize(Chip *chip)
{
	chip->pc 			= 0x200; 			// Set program counter to 0x200
	chip->opcode 		= 0x0000;			// Reset current opcode
	chip->I				= 0x0000;			// Reset index register
	chip->sp 			= 0x0000;			// Reset stack pointer
	
	chip->delayTimer 	= 0x00;				// Reset delay timer
	chip->soundTimer 	= 0x00;				// Reset sound timer
	
	for(int i = 0; i < 64; i++)				// Reset the graphics
		for(int j = 0; j < 32; j++)
			chip->gfx[i][j] = 0x00;
	
	for(int i = 0; i < 16; i++)				// Reset the stack
		chip->stack[i] = 0x0000;
	
	for(int i = 0; i < 4069; i++)			// Reset memory
		chip->memory[i] = 0x00;
	
	for(int i = 0; i < 16; i++)				// Reset registers
		chip->V[i] = 0x00;
	
	for(int i = 0; i < 16; i++)				// Reset key state
		chip->key[i] = 0x00;
	
	loadFontInMemory(chip);					// Loads the font-set into memory
}

void loadProgramInMemory(Chip *chip, char *program)
{
	int bufferSize = 0xFFF - 0x200;			// Size of the memory which is reserved for the program
	unsigned char buffer[bufferSize];
	FILE *ptr;
	
	ptr = fopen(program, "rb");
	fread(buffer, bufferSize, 1, ptr);
	
	for(int i = 0; i < bufferSize; i++)
		chip->memory[i + 512] = buffer[i];	// Reads in the program and stores it in memory at location 0x200 or 512
	
}

void loadFontInMemory(Chip *chip)
{
	// Zero							// Six							// C
	chip->memory[0x000] = 0xF0;		chip->memory[0x01E] = 0xF0;		chip->memory[0x03C] = 0xF0;
	chip->memory[0x001] = 0x90;		chip->memory[0x01F] = 0x80;		chip->memory[0x03D] = 0x80;
	chip->memory[0x002] = 0x90;		chip->memory[0x020] = 0xF0;		chip->memory[0x03E] = 0x80;
	chip->memory[0x003] = 0x90;		chip->memory[0x021] = 0x90;		chip->memory[0x03F] = 0x80;
	chip->memory[0x004] = 0xF0;		chip->memory[0x022] = 0xF0;		chip->memory[0x040] = 0xF0;
	
	// One							// Seven						// D
	chip->memory[0x005] = 0x20;		chip->memory[0x023] = 0xF0;		chip->memory[0x041] = 0xE0;
	chip->memory[0x006] = 0x60;		chip->memory[0x024] = 0x10;		chip->memory[0x042] = 0x90;
	chip->memory[0x007] = 0x20;		chip->memory[0x025] = 0x20;		chip->memory[0x043] = 0x90;
	chip->memory[0x008] = 0x20;		chip->memory[0x026] = 0x40;		chip->memory[0x044] = 0x90;
	chip->memory[0x009] = 0x70;		chip->memory[0x027] = 0x40;		chip->memory[0x045] = 0xE0;
	
	// Two							// Eight						// E
	chip->memory[0x00A] = 0xF0;		chip->memory[0x028] = 0xF0;		chip->memory[0x046] = 0xF0;
	chip->memory[0x00B] = 0x10;		chip->memory[0x029] = 0x90;		chip->memory[0x047] = 0x80;
	chip->memory[0x00C] = 0xF0;		chip->memory[0x02A] = 0xF0;		chip->memory[0x048] = 0xF0;
	chip->memory[0x00D] = 0x80;		chip->memory[0x02B] = 0x90;		chip->memory[0x049] = 0x80;
	chip->memory[0x00E] = 0xF0;		chip->memory[0x02C] = 0xF0;		chip->memory[0x04A] = 0xF0;
	
	// Three						// Nine							// F
	chip->memory[0x00F] = 0xF0;		chip->memory[0x02D] = 0xF0;		chip->memory[0x04B] = 0xF0;
	chip->memory[0x010] = 0x10;		chip->memory[0x02E] = 0x90;		chip->memory[0x04C] = 0x80;
	chip->memory[0x011] = 0xF0;		chip->memory[0x02F] = 0xF0;		chip->memory[0x04D] = 0xF0;
	chip->memory[0x012] = 0x10;		chip->memory[0x030] = 0x10;		chip->memory[0x04E] = 0x80;
	chip->memory[0x013] = 0xF0;		chip->memory[0x031] = 0xF0;		chip->memory[0x04F] = 0x80;
	
	// Four							// A
	chip->memory[0x014] = 0x90;		chip->memory[0x032] = 0xF0;
	chip->memory[0x015] = 0x90;		chip->memory[0x033] = 0x90;
	chip->memory[0x016] = 0xF0;		chip->memory[0x034] = 0xF0;
	chip->memory[0x017] = 0x10;		chip->memory[0x035] = 0x90;
	chip->memory[0x018] = 0x10;		chip->memory[0x036] = 0x90;
	
	// Five							// B
	chip->memory[0x019] = 0xF0;		chip->memory[0x037] = 0xE0;
	chip->memory[0x01A] = 0x80;		chip->memory[0x038] = 0x90;
	chip->memory[0x01B] = 0xF0;		chip->memory[0x039] = 0xE0;
	chip->memory[0x01C] = 0x10;		chip->memory[0x03A] = 0x90;
	chip->memory[0x01D] = 0xF0;		chip->memory[0x03B] = 0xE0;
}