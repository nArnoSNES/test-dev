/*---------------------------------------------------------------------------------


	Frankensteinish mix to test PVSNESLIB
	-- n_arno


---------------------------------------------------------------------------------*/
#include <snes.h>

#include "soundbank.h"
extern char __SOUNDBANK__0;

extern char patkawaii, patkawaii_end;
extern char mapkawaii, mapkawaii_end;
extern char palkawaii, palkawaii_end;
extern char pattilekawaii, pattilekawaii_end;
extern char maptilekawaii, maptilekawaii_end;
extern char paltilekawaii, paltilekawaii_end;

extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;

extern char snesfont;

extern char soundbrr,soundbrrend;
brrsamples hihisound;

void consoleSetShadowCol(u8 paletteNumber, u16 colorShad) {
	// For 3 color Font
	REG_CGADD = 0x02+(paletteNumber<<4); *CGRAM_PALETTE = colorShad & 0xFF; *CGRAM_PALETTE = colorShad>>8;
}

//---------------------------------------------------------------------------------
int main(void) {
	short scrX=0, scrY=0;
	unsigned short pad0;
	char sz[16];

	// Initialize sound engine (take some time)
	spcBoot();
	
    // Initialize SNES 
	consoleInit();

	// Set give soundbank
	spcSetBank(&__SOUNDBANK__0);
	
	// allocate around 10K of sound ram (39 256-byte blocks)
	spcAllocateSoundRegion(39);

	// Load music
	spcLoad(MOD_ANGELIQUE);
	
	// Copy tiles & Map to VRAM
	bgInitTileSet(1, &patkawaii, &palkawaii, 0, (&patkawaii_end - &patkawaii), 16*2, BG_16COLORS, 0x4000);
	bgInitMapSet(1, &mapkawaii, (&mapkawaii_end - &mapkawaii),SC_32x32, 0x3000);

	// Initialize text console with our font
	consoleInitText(0, 1, &snesfont);
	consoleSetTextCol(RGB15(0,0,0),RGB15(31,31,31));
	consoleSetShadowCol(1,RGB15(15,15,15));

	// Load effect
	spcSetSoundEntry(15, 8, 4, &soundbrrend-&soundbrr, &soundbrr, &hihisound);
	
	// Now Put in 16 color mode and out text on top of screen
	setMode(BG_MODE1,0);  bgSetDisable(2);

	// Draw a wonderful text :P
	consoleDrawText(10,20,"Press Start");
	
	// wait for press start
	pad0 = padsCurrent(0);
	while(pad0 != KEY_START) {
		pad0 = padsCurrent(0);
		WaitForVBlank(); 
	}
	
	// Play Giggle and wait a few VBlank for to finish SPC processing
	spcPlaySound(0);
	short wait = 0;
	u16 curCol = RGB15(31,31,31);
	int timer = 0;
	while(wait<40) {
		wait++;
		timer++;
		if (timer>3) {
			curCol = RGB15(31,31,31) - curCol;
			consoleSetTextCol(curCol,RGB15(31,31,31));
			timer = 0;
		}
		spcProcess();
		WaitForVBlank();
	}
	
	// Fade Out
	setFadeEffect(FADE_OUT);
	WaitForVBlank();
	
	// Clean previous text (dirty) and switch BG
	consoleDrawText(10,20,"           ");
	bgInitTileSet(1, &pattilekawaii, &paltilekawaii, 0, (&pattilekawaii_end - &pattilekawaii), 16*2, BG_16COLORS, 0x4000);
	bgInitMapSet(1, &maptilekawaii, (&maptilekawaii_end - &maptilekawaii),SC_32x32, 0x3000);
	
	
	// Write new text
	consoleSetTextCol(RGB15(15,5,5),RGB15(0,0,0));
	consoleSetShadowCol(1,RGB15(31,20,20));
	consoleDrawText(6,10,"{_________________}");
	consoleDrawText(6,11,"|                 |");
	consoleDrawText(6,12,"|      Menu 1     |");
	consoleDrawText(6,13,"|                 |");
	consoleDrawText(6,14,"|      Menu 2     |");
	consoleDrawText(6,15,"|                 |");
	consoleDrawText(6,16,"^_________________~");
	//                   "6789012345678901267"  
	
	// Init sprite
	oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end-&gfxpsrite), &palsprite, (&palsprite_end-&palsprite), 0, 0x6000, OBJ_SIZE8);

	int posmenu = 0;
	
	// Define sprites parameters
	oamSet(0, 10*8-1, (12+posmenu)*8-1, 3, 0, 0, 0, 0); 
	oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
	oamSetVisible(0,OBJ_SHOW);
	
	// Fade IN
	setFadeEffect(FADE_IN);
	WaitForVBlank();
	
	// Play file from the beginning
	spcPlay(0);
	
	// Wait for start
	pad0 = padsCurrent(0);
	while(pad0 != KEY_START) {
		// Scroll BG		
	    scrX++; scrY++; 
		bgSetScroll(1, scrX,scrY);
		
		// Refresh pad values in VBL and Get current #0 pad
		pad0 = padsCurrent(0);
		
		if (pad0) {
			// Update menu choice with current pad
			if(pad0 & KEY_UP) {
				posmenu = 0;
			}
			if(pad0 & KEY_DOWN) {
				posmenu = 2;
			}
		}
		
		// Update Sprite position based on menu choice.
		oamSet(0, 10*8-1, (12+posmenu)*8-1, 3, 0, 0, 0, 0); 
		
		// tell SPC to process sound and wait vBlank
		spcProcess();
		WaitForVBlank();
	}
	
	// Fade Out and stop music
	setFadeEffect(FADE_OUT);
	spcStop();
	spcProcess();
	WaitForVBlank();
	
	// Clean Everything (still dirty)
	bgSetDisable(1);
	setPaletteColor(0x00,0);
	consoleSetTextCol(RGB15(31,31,31),RGB15(0,0,0));
	consoleSetShadowCol(1,RGB15(15,15,15));
	consoleDrawText(6,10,"                   ");
	consoleDrawText(6,11,"                   ");
	consoleDrawText(6,12,"                   ");
	consoleDrawText(6,13,"                   ");
	consoleDrawText(6,14,"                   ");
	consoleDrawText(6,15,"                   ");
	consoleDrawText(6,16,"                   ");
	oamSetEx(0, OBJ_SMALL, OBJ_HIDE);

	// Write posmenu
	sprintf(sz,"! MENU %01X !",posmenu);
	consoleDrawText(10,14,sz); 
	
	// Fade IN
	setFadeEffect(FADE_IN);
	WaitForVBlank();
	
	//Final Wait
	while(1) {
		WaitForVBlank();
	}
	
	return 0;
}
