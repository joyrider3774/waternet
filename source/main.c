#include <stdint.h>
#include <stdio.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/bgb_emu.h>
#include <rand.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "savestate.h"

#include "maps/gamebackground.h"
#include "maps/titlescreen_map.h"
#include "maps/congratsscreen_map.h"
#include "maps/othermaps.h"
#include "maps/helprotatemap.h"
#include "maps/helprotate2map.h"
#include "maps/helprotateslidemap.h"
#include "maps/helprotateslide2map.h"
#include "maps/helpslidemap.h"
#include "maps/helpslide2map.h"

#include "tiles/blocks.h"
#include "tiles/titlescreen.h"
#include "tiles/selector.h"
#include "tiles/congratsscreen.h"


#define PAL_RED(C)   (((C)      ) & 0x1F)
#define PAL_GREEN(C) (((C) >>  5) & 0x1F)
#define PAL_BLUE(C)  (((C) >> 10) & 0x1F)

const uint16_t blackPallette[] = {
	RGB_BLACK, RGB_BLACK, RGB_BLACK, RGB_BLACK
};

const uint16_t bkgPaletteGame[] = {
	RGB_LIGHTGRAY, RGB(0,0,15), RGB(0,0,31), RGB(0,0,7)
};

const uint16_t bkgPaletteTitle[] = {
	RGB_LIGHTGRAY, RGB(0,0,31), RGB(0,0,15), RGB(0,0,7)
};

const uint16_t sprPalette[] = {
	RGB_LIGHTGRAY, RGB_WHITE, RGB_WHITE, RGB_BLACK
};

//I (joyrider3774) created the music in this tool : https://onlinesequencer.net
//and used this page to convert notes to frequencies for gameboy: http://www.devrs.com/gb/files/sndtab.html
//music uses channel 2 only using simple notes
//sound uses channel 1 only

//MUSIC PLAYING CONCEPT based on
//      Sheep it up! - A tiny GB/GBC game for the bitbitjam4 game jam
//		by Dr. Ludos (http://www.ludoscience.com)

//Here is kind of "quick'n dirty" custom music player made using solely the Channel 2 of the GB
//The main parameters of the channel are set manually, then this table defines "notes", as in "frequencies" to play that will go to the NR23 & NR24 registers
//Here is the "note table". Each note is defined by 4 hexa numbers, directly setting the values for NR21, NR22, NR23 and NR24 registers, in that order
const uint8_t music_notes [] ={
    0x82, 0x00, 0x00, 0x00, //for silence
    0x82, 0x77, 0x16, 0x84, //C  4
    0x82, 0x77, 0x50, 0x84, //C# 4
    0x82, 0x77, 0x83, 0x84, //D  4
    0x82, 0x77, 0xB5, 0x84, //D# 4
    0x82, 0x77, 0xE5, 0x84, //E  4
    0x82, 0x77, 0x2D, 0x87, //D# 6
    0x82, 0x77, 0x22, 0x87, //D 6
    0x82, 0x77, 0x39, 0x87, //E 6
    0x82, 0x77, 0x44, 0x87, //F 6
    0x82, 0x77, 0x15, 0x85, //E 4
    
};

//helper defines
#define P   0
#define C4  1
#define C4S 2 
#define D4  3
#define D4S 4
#define E4  5
#define D6S 6
#define D6  7
#define E6  8
#define F6  9
#define F4  10
#define PAUSE 15
#define PAUSE2 6
#define PAUSE3 14
#define PAUSE4 13

//Here is the "partition" table. Each note is defined by two numbers in this array
//- The first number is the "note", an index to the "notes" tables array / 4 (as each note is defined by 4 entry in notes, we can use note 0,1,2,3... instead of note 0,4,8,12,16 - it's easier to read for us mere humans)
//- The second number is the "delay" after the next note : the player will wait XX cycles (max: 240 - 4 seconds (60 cycles per seconds) and play the next "note"
//Music Table Format: Note index in "music_notes" / 4, Delay before next note

//https://onlinesequencer.net/2498607
const uint8_t music_levelsCleared [] ={
    D6,PAUSE4,
    D6S,PAUSE4,
    D6,PAUSE4,
    P,PAUSE4,
    D6,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4,
    E6,PAUSE4,
    E6,PAUSE4,
    E6,PAUSE4,
    D6,PAUSE4,
    P,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    P,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4,
    E6,PAUSE4,
    P,PAUSE4,
    D6,PAUSE4,
    D6S,PAUSE4,
    D6,PAUSE4,
    E6,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    D6S,PAUSE4,
    D6,PAUSE4,
    D6,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    P,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    P,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    P,PAUSE4,
    F6,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4
};


//https://onlinesequencer.net/2484974
const uint8_t music_won [] ={
  D6S, PAUSE2,
  D6,  PAUSE2,
  D6S, PAUSE2,
  E6 , PAUSE2,
  D6S, PAUSE2,
  E6 , PAUSE2,
  F6 , PAUSE2 << 2,
  P, PAUSE2  
};

//https://onlinesequencer.net/2485064
const uint8_t music_game [] ={ 
    P, PAUSE3,
    C4, PAUSE3,
    C4S, PAUSE3,
    P, PAUSE3,
    C4, PAUSE3,
    C4S, PAUSE3,
    C4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    E4, PAUSE3,
    F4, PAUSE3,
    P, PAUSE3,

    C4, PAUSE3,
    C4S, PAUSE3,
    P, PAUSE3,
    C4, PAUSE3,
    C4S, PAUSE3,
    C4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    E4, PAUSE3,
    F4, PAUSE3,
    P, PAUSE3,

    D4, PAUSE3,
    C4, PAUSE3,
    E4, PAUSE3,
    D4, PAUSE3,
    P,  PAUSE3,
    E4, PAUSE3,
    P,  PAUSE3,
    E4, PAUSE3,
    D4, PAUSE3,
    P,  PAUSE3,
    D4S, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    P, PAUSE3,
    E4, PAUSE3,
    D4S, PAUSE3,
    D4, PAUSE3,
    C4, PAUSE3,
    D4S, PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    P,PAUSE3,
    D4S,PAUSE3,
    D4,PAUSE3,
    C4,PAUSE3,
    P,PAUSE3,
    D4,PAUSE3,
    C4,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    P,PAUSE3,
    E4,PAUSE3,
    P,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    P,PAUSE3,
    D4S,PAUSE3,
    D4S,PAUSE3,
    E4,PAUSE3,
    P,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    C4,PAUSE3,
    C4S,PAUSE3,
    D4S,PAUSE3,
    F4,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    D4S,PAUSE3,
    D4S,PAUSE3,
    P, PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    D4,PAUSE3,
    P, PAUSE3,
    D4S,PAUSE3,
    C4S,PAUSE3,
    C4S,PAUSE3,
    P, PAUSE3,
    D4,PAUSE3
   
};

//https://onlinesequencer.net/2484977
const uint8_t music_intro [] ={
    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    P,   PAUSE * 8,
    
    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    D4,  PAUSE,
    C4,  PAUSE,
    C4S, PAUSE,
    P,   PAUSE * 4,

    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    P,   PAUSE * 8,
    
    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    D4,  PAUSE,
    C4,  PAUSE,
    C4S, PAUSE,
    P,   PAUSE * 4,

    D4,  PAUSE,
    D4,  PAUSE, 
    C4S, PAUSE,
    C4,  PAUSE,
    D4S, PAUSE,
    C4S, PAUSE,
    D4,  PAUSE,
    D4S, PAUSE,
    C4S, PAUSE,
    D4S, PAUSE,
    D4,  PAUSE,
    D4,  PAUSE,
    D4,  PAUSE,
    E4,  PAUSE,
    C4,  PAUSE,
    C4S, PAUSE,
    E4,  PAUSE,
    D4,  PAUSE * 3,
    P,   PAUSE * 4
};


uint8_t startPos, menuPos, 
        maxLevel, selectedLevel, boardX, boardY, joyPad, difficulty, 
        gameState, boardWidth, boardHeight, boardSize, prevBoardWidth, prevBoardHeight,
        i, j, x, y, rnd, index, cc, maxcc, clearbit, redrawLevelbit, fadein, fadeCounter, levelDone,
        cursorFrameCount, cursorFrame, prevJoyPad, titleStep, gameMode, posAdd, redrawLevelDoneBit,
        tmp, neighboursFound, selectedNeighbour, currentPoint, visitedRooms, music_note, music_tempo,
        music_length, music_loop, prev_music, fadeStep, startFade, fadeSteps, mainMenu;

uint8_t musicArray[255];
int16_t selectionX, selectionY,i16;
uint16_t rnd16, randomSeed, moves;
uint8_t neighbours[4];
uint8_t cellStack[maxBoardSize+1];
point lookUpTable[maxBoardSize];
unsigned char level[maxBoardSize];
unsigned char number[10];
unsigned char titleMessage[20];
uint16_t  toPallette[4];

void initSaveState()
{
    if (strcmp(header, "waternet_001") != 0)
    {
        for (j=0; j<gmCount; j++)
        {
            for (i=0; i<diffCount; i++)
            {
                for (y = 0; y<levelCount; y++)
                {
                    LevelLocks[j][i][y] = 0;
                }
                //unlock first level
                LevelLocks[j][i][0] = 1; 
            }
        }
        strcpy(header, "waternet_001");
    } 
}

void initSound()
{
    // these registers must be in this specific order!
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels
}

void SelectMusic(uint8_t musicFile, uint8_t loop)
{
    if (prev_music != musicFile)
    {
        prev_music = musicFile;
        CRITICAL {
            music_note = 0;
            music_tempo = 0;
            music_loop = loop;
            memset(musicArray, 0, sizeof(musicArray));
            switch (musicFile) 
            {
                case musTitle:
                    memcpy(musicArray, music_intro, sizeof(music_intro));
                    music_length = sizeof(music_intro);
                    break;
                case musLevelClear:
                    memcpy(musicArray, music_won, sizeof(music_won));
                    music_length = sizeof(music_won);
                    break;
                case musAllLevelsClear:
                    memcpy(musicArray, music_levelsCleared, sizeof(music_levelsCleared));
                    music_length = sizeof(music_levelsCleared);
                    break;
                case musGame:
                    memcpy(musicArray, music_game, sizeof(music_game));
                    music_length = sizeof(music_game);
                    break;
            }
        }
    }
}

void playNote()
{    
    if(music_note < music_length)
    {
        //Play current note ( << 2 is like a *4 (bitshift is faster than * for the GB), because music note is 0,1,2,3,4... while our actual note array uses 4 entry per note, 
        //so we have to multiply note by 4 to get the actual registers index for each note)
        NR21_REG = music_notes[ musicArray[music_note]<<2 ];
        NR22_REG = music_notes[ (musicArray[music_note]<<2)+1 ];
        NR23_REG = music_notes[ (musicArray[music_note]<<2)+2 ];
        NR24_REG = music_notes[ (musicArray[music_note]<<2)+3 ];
        
        //Set the new delay to wait
        music_tempo = musicArray[ music_note+1 ];

        //Skip to the next note
        music_note += 2;
        
        if(music_loop && (music_note > music_length-1 ))
        {
            music_note = 0;
        }
    }

}


void musicTimer()
{
    //Play some music
    if( music_tempo == 0 )
    {
       playNote();
    }
    //Else wait for the next note to play
    else 
    {
        music_tempo--;
    }
}

void initMusic()
{
    CRITICAL {
        add_TIM(musicTimer);
        prev_music = 0;
        music_note = 1;
        music_length = 1;
        music_tempo = 0;
        music_loop = 0;
    }

    if (_cpu == CGB_TYPE)
    {
        TMA_REG = 0x80;
        TAC_REG = 0x4U;
    }
    else
    {
        TMA_REG = 0xC0;
        TAC_REG = 0x4U;    
    }
    set_interrupts(TIM_IFLAG | VBL_IFLAG);
}

void playGameMoveSound()
{
    NR10_REG = 0x00;
    NR11_REG = 0x40;
    NR12_REG = 0x63;
    NR13_REG = 0x06;
    NR14_REG = 0x87;
}

void playErrorSound()
{
    NR10_REG = 0x19;
    NR11_REG = 0xC2;
    NR12_REG = 0x93;
    NR13_REG = 0x73;
    NR14_REG = 0xC6; 
}

void playMenuSelectSound()
{

    NR10_REG = 0x00;
    NR11_REG = 0x00;
    NR12_REG = 0x63;
    NR13_REG = 0x9E;
    NR14_REG = 0x87;     
}

void playMenuBackSound()
{
    NR10_REG = 0x00;
    NR11_REG = 0x81;
    NR12_REG = 0x63;
    NR13_REG = 0xE8;
    NR14_REG = 0x83;       
}

void playMenuAcknowlege()
{
    NR10_REG = 0x57;
    NR11_REG = 0x03;
    NR12_REG = 0x5e;
    NR13_REG = 0xd0;
    NR14_REG = 0x87;     
}

void playGameAction()
{
    NR10_REG = 0x00;
    NR11_REG = 0x80;
    NR12_REG = 0x63;
    NR13_REG = 0x6f;
    NR14_REG = 0x86;     
}

//set the fade flag to fade in or not and increase the fadecounter to startvalue
void startfade(uint8_t afadein, uint8_t ForTitle)
{
    if(_cpu == CGB_TYPE)
    {
        //6 steps on color gameboy to go from default pallete to black and vice versa (goes from 0 to fadesteps)
        fadeSteps = 5;
        //skips per fadestep to prolong fade effect
        startFade = 7;
        //counts from 1 to startfade above then increases fadestep 
        fadeCounter = 0;
        //fade step we are at (will go until fadestep > fadesteps)
        fadeStep = 0;

    }
    else
    {
        //how long takes each step
        fadeStep = 6;
        //4 steps on dmg gameboy to go from default pallete to black and vice versa
        fadeSteps = 4;
        startFade = fadeSteps * fadeStep;
        fadeCounter = startFade;
    }
    
    fadein = afadein;

    //set temporary palette based on title or gameplay (screen is only faded on 2 places)
    if (ForTitle)
    {
        memcpy(toPallette, bkgPaletteTitle, sizeof(toPallette));   
    }
    else
    {
        memcpy(toPallette, bkgPaletteGame, sizeof(toPallette));
    }
}

//based on zgdb source
uint16_t fadeBlack(uint8_t step, uint16_t color) {
    return RGB(
        PAL_RED(color) >> step,
        PAL_GREEN(color) >> step,
        PAL_BLUE(color) >> step
    );
}

//fade to color from black
void fadeInCGB() {
    uint8_t c;
    uint16_t palette[4];

    for(c = 0; c != 4; c++) 
    {
        palette[c] = fadeBlack(fadeSteps - fadeStep, toPallette[c]);
    }
    set_bkg_palette(0, 1, palette);
}

//fade to black from color
void fadeOutCGB() {
    uint8_t c;
    uint16_t palette[4];

    for(c = 0; c != 4; c++) {
        palette[c] = fadeBlack(fadeStep, toPallette[c]);
    }
    set_bkg_palette(0, 1, palette);
}

//fade function for color gameboy need to set fadecounter first using above function
uint8_t fadecgb()
{
    //fadestep out of range return true
    if (fadeStep > fadeSteps)
    {
        return 1;
    }
    else
    {
        //fade based on what we need todo (in/out)
        if(fadein)
        {
            fadeInCGB();
        }
        else
        {
            fadeOutCGB();
        }
            
        //inc fadecounter 
        fadeCounter++;
        //if fadecounter reaches skip limit 
        if(fadeCounter == startFade)
        {
            //increase fade step & reset fadecounter
            fadeStep++;
            fadeCounter = 1;
        }
        return 0;
    }
}

//simple fade function for DMG gameboy (to BLACK) need to set fadecounter first using above function
uint8_t fadegb()
{
    if (fadeCounter == 0)
    {
        return 1;
    }
    else
    {
        if(fadeCounter == startFade)
        {
            if (fadein)
            {
                BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
            }
            else
            {    
                BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            }
        }
        else
        {
            if(fadeCounter == startFade-fadeStep)
            {
                if (fadein)
                {
                    BGP_REG =  DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
                }
                else
                {
                    BGP_REG =  DMG_PALETTE(DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
                }
            }
            else
            {
                if (fadeCounter == startFade-fadeStep-fadeStep)
                {
                    if (fadein)
                    {
                        BGP_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
                    }
                    else
                    {    
                        BGP_REG =  DMG_PALETTE(DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK, DMG_BLACK);
                    }
                }
                else
                {
                    if(fadeCounter == startFade-fadeStep-fadeStep-fadeStep)
                    {
                        if (fadein)
                        {
                            BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
                        }
                        else
                        {    
                            BGP_REG =  DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
                        }
                    }
                }
            }
        }
    }
    fadeCounter--;
    return 0;
}

uint8_t fade()
{
    if(_cpu == CGB_TYPE)
    {
        return fadecgb();
    }
    else
    {
        return fadegb();
    }
}

//generates a lookup table containing per "tile" value the possible x & y position
//based on the board size but only regenerate it if the board size had changed
void generateLookupTable(uint8_t width, uint8_t height)
{
    if ((width == prevBoardWidth) && (height == prevBoardHeight))
    {
        return;
    }

    i = 0;
    for (y=0; y != height; y++)
    {
        for (x=0; x != width; x++)
        {
            lookUpTable[i+x].x = x;
            lookUpTable[i+x].y = y;
        }
        i += width;
    }
    //remember current board size
    prevBoardWidth = width;
    prevBoardHeight = height;
}

//print a number on levelselect or game screen (used to display levelnr & moves)
//again based on asci codes and adjust to tiles in the tileset
void printNumber(uint8_t ax, uint8_t ay, uint16_t aNumber, uint8_t maxDigits, uint8_t emptyBlock)
{
    memset(number, 0, 10);
    sprintf(number, "%d", aNumber);
    for (i=0; i < maxDigits; i++)
    {
        if (number[i] != 0)
        {
           // if(get_bkg_tile_xy(ax + i, ay) != number[i] + 32)
           // {
                set_bkg_tile_xy(ax + i, ay, number[i] + 32);
           // }
        }
        else
        {
           // if(get_bkg_tile_xy(ax + i, ay) != emptyBlock)
           // {
                set_bkg_tile_xy(ax + i, ay, emptyBlock);
           // }
        }      
    }
}

//print a message on the title screen on ax,ay, the tileset from titlescreen contains an alphabet
//and we use the asci codes to adapt the value to the tilenr in the tileset
void printTitle(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t emptyBlock)
{
    memset(titleMessage, 0, 20);
    sprintf(titleMessage, "%s", amsg);
    for (i=0; i < 20; i++)
    {
        if (titleMessage[i] == ' ')
        {
          //  if(get_bkg_tile_xy(ax + i, ay) != emptyBlock)
          //  {
                set_bkg_tile_xy(ax + i, ay, emptyBlock);
          //  }
        }
        else
        {
            if (titleMessage[i] != 0)
            {
            //    if(get_bkg_tile_xy(ax + i, ay) != titleMessage[i] + 34)
            //    {
                    set_bkg_tile_xy(ax + i, ay, titleMessage[i] + 34);
            //    }
            }
            else
            {
                break;
            }
        }    
    }
}

//print a message on the title screen on ax,ay, the tileset from titlescreen contains an alphabet
//and we use the asci codes to adapt the value to the tilenr in the tileset
void printLevelSelectGame(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t emptyBlock)
{
    memset(titleMessage, 0, 20);
    sprintf(titleMessage, "%s", amsg);
    for (i=0; i < 20; i++)
    {
        if (titleMessage[i] == ' ')
        {
            //if(get_bkg_tile_xy(ax + i, ay) != emptyBlock)
            //{
                set_bkg_tile_xy(ax + i, ay,emptyBlock);
            //}
        }
        else
        {
            //A-Z
            if ((titleMessage[i] > 64) && (titleMessage[i] < 91))
            {
                //if(get_bkg_tile_xy(ax + i, ay) != titleMessage[i] + 25)
                //{
                    set_bkg_tile_xy(ax + i, ay, titleMessage[i] + 25);
                //}
            }   
            else
            {
                if (titleMessage[i] == ':')
                {
                    set_bkg_tile_xy(ax + i, ay, 116);
                }  
                else
                {
                    if (titleMessage[i] == 'a')
                    {
                        set_bkg_tile_xy(ax + i, ay, 119);
                    }
                    else
                    {
                        if (titleMessage[i] == 'b')
                        {
                            set_bkg_tile_xy(ax + i, ay, 117);
                        } 
                        else
                        {
                            if(titleMessage[i] == 0)
                            {
                                break;
                            }
                            else
                            {
                                //0-9
                                if ((titleMessage[i] > 46) && (titleMessage[i] < 58))
                                {
                                    set_bkg_tile_xy(ax + i, ay, titleMessage[i] + 32);
                                }
                            } 
                        }
                    }
                }
            }
        }    
    }
}

void printCongratsScreen(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t emptyBlock)
{
    memset(titleMessage, 0, 20);
    sprintf(titleMessage, "%s", amsg);
    for (i=0; i < 20; i++)
    {
        if (titleMessage[i] == ' ')
        {
            //if(get_bkg_tile_xy(ax + i, ay) != emptyBlock)
            //{
                set_bkg_tile_xy(ax + i, ay,emptyBlock);
            //}
        }
        else
        {
            if (titleMessage[i] != 0)
            {
            //    if(get_bkg_tile_xy(ax + i, ay) != titleMessage[i])
            //    {
                    set_bkg_tile_xy(ax + i, ay, titleMessage[i]);
            //    }
            }
            else
            {
                break;
            }
        }
    }
}

void performantdelay(uint8_t numloops) 
{
    for(i = 0; i != numloops; i++) 
    {
        wait_vbl_done();
    }     
}

void moveBlockDown(uint8_t aTile)
{
    tmp = level[aTile + boardSize - boardWidth]; 
    for (i= boardSize - boardWidth; i != 0 ; i -= boardWidth)
    {
        level[aTile + i] = level[aTile + i -boardWidth];
    }
    level[aTile] = tmp;
}

void moveBlockUp(uint8_t aTile)
{
    tmp = level[aTile - boardSize + boardWidth]; 
    for (i= boardSize - boardWidth; i != 0; i -= boardWidth)
    {
        level[aTile - i] = level[aTile - i + boardWidth];
    }
    level[aTile] = tmp;
}

void moveBlockRight(uint8_t aTile)
{
    tmp = level[aTile + boardWidth - 1]; 
    //for (i= 0; i != boardWidth -1 ; i++)
    //{
    //    level[aTile + boardWidth - 1 - i] = level[aTile + boardWidth - 1 -i - 1];
    //}
    memmove(&level[aTile+1], &level[aTile] , boardWidth -1);
    level[aTile] = tmp;
}

void moveBlockLeft(uint8_t aTile)
{
    tmp = level[aTile - boardWidth + 1]; 
  //  for (i= 0; i != boardWidth - 1; i++)
   // {   
    //    level[aTile - boardWidth + 1 +i] = level[aTile - boardWidth + 1 +i + 1];
   // }
    memmove(&level[aTile - boardWidth + 1], &level[aTile - boardWidth + 1 + 1], boardWidth -1);
    level[aTile] = tmp;
}

//rotates a tile by change the tilenr in the level
//there are 16 tiles per set and there are 3 sets no water, water filled, and special start tiles
void rotateBlock(uint8_t aTile)
{
    switch (level[aTile])
    {
        case 1:
        case 17:
        case 33:
            level[aTile] = 2;
            break;
        case 2:
        case 18:
        case 34:
            level[aTile] = 4;
            break;
        case 3:
        case 19:
        case 35:
            level[aTile] = 6;
            break;
        case 4:
        case 20:
        case 36:
            level[aTile] = 8;
            break;
        case 5:
        case 21:
        case 37:
            level[aTile] = 10;
            break;
        case 6:
        case 22:
        case 38:
            level[aTile] = 12;
            break;
        case 7:
        case 23:
        case 39:
            level[aTile] = 14;
            break;
        case 8:
        case 24:
        case 40:
            level[aTile] = 1;
            break;
        case 9:
        case 25:
        case 41:
            level[aTile] = 3;
            break;
        case 10:
        case 26:
        case 42:
            level[aTile] = 5;
            break;
        case 11:
        case 27:
        case 43:
            level[aTile] = 7;
            break;
        case 12:
        case 28:
        case 44:
            level[aTile] = 9;
            break;
        case 13:
        case 29:
        case 45:
            level[aTile] = 11;
            break;
        case 14:
        case 30:
        case 46:
            level[aTile] = 13;
            break;      
        default:
            break;    
    }
}

void shuffleSlide(uint8_t aTile)
{
    rnd = (uint8_t)rand();
    rnd &=3;
    if (rnd == 0)
    {
        moveBlockUp(lookUpTable[aTile].x +  boardSize - boardWidth);
    }
    else
    {
        if (rnd == 1)
        {
            moveBlockDown(lookUpTable[aTile].x);
        }
        else
        {
            if(rnd == 2)
            {
                moveBlockLeft(boardWidth - 1 + aTile - lookUpTable[aTile].x);
            }
            else
            {
                moveBlockRight(aTile - lookUpTable[aTile].x);     
            }
        }
    }
}

void shuffleRotate(uint8_t aTile)
{
    rnd = (uint8_t)rand();
    //4 directions so normally modulus 4
    //but 4 is power of 2 so we can and value with denominator - 1
    //4-1 = 3 so and with 3
    rnd &= 3;
    for (i = 0; i < rnd; i++)
    {
        rotateBlock(aTile);
    }
}

void shuffleLevel()
{
    //return;
    j = 0;
    while(j < boardSize)
    {
        if(gameMode == gmRotate)
        {
            shuffleRotate(j);
            j++;
        }
        else
        {
            if(gameMode == gmSlide)
            {
                shuffleSlide(j);
                //for speed up it should be fine as all slide levels are uneven in width / height (except random)
                j+=2;
            }
            else
            {
                if(gameMode == gmRotateSlide)
                {
                    rnd = (uint8_t)rand();
                    rnd = rnd & 1;
                    if(rnd == 0)
                    {
                        shuffleSlide(j);
                        //for speed up
                        j+=2;
                    }
                    else
                    {
                        shuffleRotate(j);
                        j++;
                    }
                }
            }
        }
    }
}

void updateConnected()
{
    uint8_t currentPoint;
    //reset all tiles to default not filled one
    for (i= 0; i != boardSize; i++)
    {
        if (level[i] > 31)
        {
            level[i] -= 32;
        }
        else
        {
            if (level[i] > 15)
            {
                level[i] -= 16;
            }
        }
    } 

    //start with start tile
    cc = 1;
    cellStack[cc] = startPos;
    currentPoint = cellStack[cc];
    while(cc > 0)
    {
        //if tile is bigger then 15 we already handled this one, continue with next one
        if (level[currentPoint] > 15)
        {
            if(cc > 0)
            {
                cc--;
                continue;
            }
        }
        
        //if tile has passage to the north and north neigbour passage to the south 
        if ((lookUpTable[currentPoint].y > 0) && (!(level[currentPoint] & 1)) &&
            (((level[currentPoint - boardWidth] < 16) && (!(level[currentPoint - boardWidth] & 4)) ) || 
            ((level[currentPoint - boardWidth] > 15) && (!((level[currentPoint - boardWidth] - 16) & 4)))))
        {   
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }

            //add neighbour to cellstack of to handle tiles
            if (level[currentPoint - boardWidth] < 16)
            {
                cc++;
                cellStack[cc] = currentPoint - boardWidth;
            }
        }       
        
        //if tile has passage to the east and east neigbour passage to the west 
        if  ((lookUpTable[currentPoint].x  + 1 < boardWidth) && (!(level[currentPoint] & 2)) &&
            (((level[currentPoint + 1] < 16) && (!(level[currentPoint + 1] & 8))) || 
            ((level[currentPoint + 1] > 15) && (!((level[currentPoint + 1] - 16) & 8)))))
        {
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }

            //add neighbour to cellstack of to handle tiles
            if (level[currentPoint + 1] < 16)
            {
                cc++;
                cellStack[cc] = currentPoint + 1;
            }

        }       
        
        //if tile has passage to the south and south neigbour passage to the north 
        if ((lookUpTable[currentPoint].y + 1 < boardHeight) && (!(level[currentPoint] & 4 )) && 
           (((level[currentPoint + boardWidth] < 16) && (!(level[currentPoint + boardWidth] & 1))) || 
           ((level[currentPoint + boardWidth] > 15) && (!((level[currentPoint + boardWidth] - 16) & 1)))))
        {
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }

            //add neighbour to cellstack of to handle tiles
            if (level[currentPoint + boardWidth] < 16)
            {
                cc++;
                cellStack[cc] = currentPoint + boardWidth; 
            }
        }       

        //if tile has passage to the west and west neigbour passage to the east 
        if  ((lookUpTable[currentPoint].x > 0) && (!(level[currentPoint] & 8)) &&
            (((level[currentPoint - 1] < 16) && (!(level[currentPoint - 1] & 2))) ||
            ((level[currentPoint - 1] > 15) && (!((level[currentPoint - 1] - 16) & 2)))))
        {
            //adapt tile to filled tile
            if(level[currentPoint] < 16)
            { 
                level[currentPoint] += 16;
            }
            
            //add neighbour to cellstack of to handle tiles
            if(level[currentPoint - 1] < 16)
            {
                cc++;
                cellStack[cc] = currentPoint - 1;
            }
        }  

        //take tile from the cellstack
        currentPoint = cellStack[cc];
        if (cc > 0)
        {
            cc--;
        }  
    }

    //add start pos special tile
    if (level[startPos] > 15)
    {
        level[startPos] += 16;
    }
    else 
    {
        if (level[startPos] < 16)
        {
            level[startPos] += 32;
        }
    }

}

void generateLevel()
{
    
    cc = 0;
    //generate a lookup table so we don't have to use modulus or divide constantly
    generateLookupTable(boardWidth, boardHeight);
    
    //intial all walls value in every room we will remove bits of this value to remove walls  
    memset(level, 0xfu, boardSize);
  
    currentPoint = 0;
    visitedRooms = 1;
    while (visitedRooms != boardSize)
    {
        neighboursFound = 0;
        //tile has neighbour to the right which we did not handle yet
        if ((level[currentPoint+1] == 0xfu) && (lookUpTable[currentPoint].x + 1 < boardWidth))
        {
            neighbours[neighboursFound] = currentPoint + 1;
            neighboursFound++;
        }
    
        //tile has neighbour to the left which we did not handle yet
        if ((level[currentPoint-1] == 0xfu) && (lookUpTable[currentPoint].x > 0))
        {
            neighbours[neighboursFound] = currentPoint -1;
            neighboursFound++;
        }

        //tile has neighbour the north which we did not handle yet
        if ((level[currentPoint - boardWidth] == 0xfu) && (lookUpTable[currentPoint].y > 0))
        {
            neighbours[neighboursFound] = currentPoint - boardWidth;
            neighboursFound++;
        }

        //tile has neighbour the south which we did not handle yet
        if ((level[currentPoint + boardWidth] == 0xfu) && (lookUpTable[currentPoint].y + 1 < boardHeight))
        {
            neighbours[neighboursFound] = currentPoint + boardWidth;
            neighboursFound++;
        }

        //if we had any unhandled neighbours
        if (neighboursFound != 0)
        {
            //take a random neighbour
            if (neighboursFound == 1)
            {
                rnd = 0;
            }
            else
            {
                rnd = (uint8_t)rand();
                if ((neighboursFound == 2) || (neighboursFound == 4))
                {
                    rnd &= neighboursFound -1;
                }
                else //only for 3 neighbours
                {
                    rnd = rnd >> 4;
                    if (rnd == 0)
                    {
                        rnd = 2;
                    }
                    else
                    {
                       rnd = rnd >> 2;
                       if(rnd == 0)
                       {
                           rnd = 1;
                       }
                       else
                       {
                           rnd = 0;
                       }
                    }
                }
            }
            selectedNeighbour = neighbours[rnd];
            //tile has neighbour to the east
            if( lookUpTable[selectedNeighbour].x > lookUpTable[currentPoint].x)
            {
                //remove west wall neighbour
                level[selectedNeighbour] &= ~(8);
                //remove east wall tile
                level[currentPoint] &= ~(2);
            }
            else // tile has neighbour to the west
            {
                if(lookUpTable[selectedNeighbour].x < lookUpTable[currentPoint].x)
                {
                    //remove east wall neighbour
                    level[selectedNeighbour] &= ~(2);
                    //remove west wall tile
                    level[currentPoint] &= ~(8);
                }
                else // tile has neighbour to the north
                {
                    if(lookUpTable[selectedNeighbour].y < lookUpTable[currentPoint].y)
                    {
                        //remove south wall neighbour
                        level[selectedNeighbour] &= ~(4);
                        //remove north wall tile
                        level[currentPoint] &= ~(1);
                    }
                    else // tile has neighbour to the south
                    {
                        if(lookUpTable[selectedNeighbour].y > lookUpTable[currentPoint].y)
                        {
                            //remove north wall neighbour
                            level[selectedNeighbour] &= ~(1);
                            //remove south wall tile
                            level[currentPoint] &= ~(4);
                        }
                    }
                }
            }
            
            //add tile to the cellstack
            if(neighboursFound > 1)
            {
                cellStack[cc] = currentPoint;
                cc++;
            } 
            //set tile to the neighbour   
            currentPoint = selectedNeighbour;
            visitedRooms++;           
        }
        else //no neighbours take a tile from the list
        {
            cc--;
            currentPoint = cellStack[cc];
        }
    }
}

//when all board tiles are not below 16, the level is cleared
//as there are 16 tiles per tilegroup (no water, water, special start with water) 
uint8_t isLevelDone()
{
    for (i=0; i != boardSize; i++)
    {
        if(level[i] < 16)
        {
            return 0;
        }
    }
    return 1;
}

void updateSwitches() 
{
	HIDE_WIN;
	SHOW_BKG;
}

void updateCursorFrame()
{
    uint8_t tmpFrame;
    cursorFrameCount++;
    if (cursorFrameCount >= MAXCURSORFRAMECOUNT)
    {
        cursorFrame++;
        cursorFrameCount = 0;
        if (cursorFrame >= CURSORANIMCOUNT)
        {
            cursorFrame = 0;
        }
        tmpFrame = (cursorFrame * 4);
       
        //cursor 0
        set_sprite_tile(0, 8 + tmpFrame);
        set_sprite_tile(1, 9 + tmpFrame);
        set_sprite_tile(2, 10 + tmpFrame);
        set_sprite_tile(3, 11 + tmpFrame);
        
        //cursor 1
        set_sprite_tile(4, 8 + tmpFrame);
        set_sprite_tile(5, 9 + tmpFrame);
        set_sprite_tile(6, 10 + tmpFrame);
        set_sprite_tile(7, 11 + tmpFrame);
       
        //cursor 2
        set_sprite_tile(8, 8 + tmpFrame);
        set_sprite_tile(9, 9 + tmpFrame);
        set_sprite_tile(10, 10 + tmpFrame);
        set_sprite_tile(11, 11 + tmpFrame);
       
        //cursor 3
        set_sprite_tile(12, 8 + tmpFrame);
        set_sprite_tile(13, 9 + tmpFrame);
        set_sprite_tile(14, 10 + tmpFrame);
        set_sprite_tile(15, 11 + tmpFrame);
    }
}

void hideCursors()
{
    //HIDE CURSOR SPRITES
    //cursor 0
    move_sprite(0, 0, 0);
    move_sprite(1, 0, 0);
    move_sprite(2, 0, 0);
    move_sprite(3, 0, 0);
    
    //cursor 1
    move_sprite(4, 0, 0);
    move_sprite(5, 0, 0);
    move_sprite(6, 0, 0);
    move_sprite(7, 0, 0);
    
    //cursor 2
    move_sprite(8, 0, 0);
    move_sprite(9, 0, 0);
    move_sprite(10, 0, 0);
    move_sprite(11, 0, 0);
    
    //cursor 3
    move_sprite(12, 0, 0);
    move_sprite(13, 0, 0);
    move_sprite(14, 0, 0);
    move_sprite(15, 0, 0);
    HIDE_SPRITES;
}

void showCursors()
{
    SHOW_SPRITES;
}

void setCursorPos(uint8_t cursorNr, uint8_t xPos, uint8_t yPos)
{
    if (cursorNr > 3)
    {
        return;
    }
    move_sprite((cursorNr<<2) + 0, 8 + ((xPos) << 3), 16 + ((yPos-1) << 3));
    move_sprite((cursorNr<<2) + 1, 8 + ((xPos+1) << 3), 16 + ((yPos) << 3));
    move_sprite((cursorNr<<2) + 2, 8 + ((xPos) << 3), 16 + ((yPos+1) << 3));
    move_sprite((cursorNr<<2) + 3, 8 + ((xPos-1) << 3), 16 + ((yPos) << 3)); 
}

void initCursors()
{
    set_sprite_data(0, 24, selector);
    
    //cursor 0
    set_sprite_tile(0, 8);
    set_sprite_tile(1, 9);
    set_sprite_tile(2, 10);
    set_sprite_tile(3, 11);

    //cursor 1
    set_sprite_tile(4, 8);
    set_sprite_tile(5, 9);
    set_sprite_tile(6, 10);
    set_sprite_tile(7, 11);

    //cursor 2
    set_sprite_tile(8, 8);
    set_sprite_tile(9, 9);
    set_sprite_tile(10, 10);
    set_sprite_tile(11, 11);

    //cursor 3
    set_sprite_tile(12, 8);
    set_sprite_tile(13, 9);
    set_sprite_tile(14, 10);
    set_sprite_tile(15, 11);
    
    cursorFrameCount = 0;
    cursorFrame = 0;
}

void initLevel(uint16_t aRandomSeed)
{
    levelDone = 0;
    moves = 0;
    if(difficulty != diffRandom)
    { 
        //use level number + fixed value based on difficulty as seed for the random function
        //this makes sure every level from a difficulty will remain the same
        initrand((uint16_t)selectedLevel + ((uint16_t)difficulty * (uint16_t)500) + ((uint16_t)gameMode * (uint16_t)50));
    }
    else
    {
        initrand(aRandomSeed); 
    }
    //set boardsize and max level based on difficulty
    switch (difficulty)
    {
        case diffVeryEasy:
            boardWidth = 6;
            boardHeight = 6;
            maxLevel = levelCount;
            break;
        case diffEasy:
            boardWidth = 8;
            boardHeight = 8;
            maxLevel = levelCount;
            break;
        case diffNormal:
            boardWidth = 10;
            boardHeight = 10;
            maxLevel = levelCount;
            break;
        case diffHard:
            boardWidth = 12;
            boardHeight = 12;
            maxLevel = levelCount;
            break;
        case diffVeryHard:
            boardWidth = 14;
            boardHeight = 14;
            maxLevel = levelCount;
            break;    
        case diffRandom:
            rnd = (uint8_t)rand();
            boardWidth = 6 + (rnd % (maxBoardWidth -5));  
            rnd = (uint8_t)rand();
            boardHeight = 6 + (rnd % (maxBoardHeight -5));
            maxLevel = 0; //special value with random
            break;
    }
    //add space for arrows based on same posadd value (1 or 0 depending if sliding is allowed)
    boardWidth -= posAdd + posAdd + posAdd;
    boardHeight -= posAdd + posAdd + posAdd;

    boardSize = boardWidth * boardHeight;
    //BGB_PROFILE_BEGIN("");
    //generate the level
    generateLevel();
    //BGB_PROFILE_END("generateLevel: ");
    //startpoint of of level in center of screen
    boardX = (maxBoardBgWidth - boardWidth) >> 1;
    boardY = (maxBoardBgHeight  - boardHeight) >> 1;
    startPos = (boardWidth >> 1) + (boardHeight >> 1) * (boardWidth);
    //startpoint of tile with water and our cursor
    selectionX = boardWidth >> 1;
    selectionY = boardHeight >> 1;
    setCursorPos(0, boardX + selectionX, boardY + selectionY);

    //level is currently the solution so we still need to shuffle it
    shuffleLevel();
    //update possibly connected tiles already starting from startpoint
    updateConnected();
}

void updateBackgroundLevelSelect() 
{
    //clearbit will clear the background tiles
    //this is only needed for random selection and initial call to set the tiles
    //otherwise the screen acted weird for some reason causing a glitch
    if(clearbit)
    {
        //only redraw changed tiles
        i16 = 0;
        for (y = 0; y != gameBackgroundHeight; y++)
        {
            for (x = 0; x != gameBackgroundWidth; x++)
            {   
                //to prevent flickering. If i draw background + something else over it (like level or so), flickering might occur    
                if ((((x < boardX -posAdd) || (x > boardX + boardWidth - 1 + posAdd) ||
                    (y < boardY -posAdd) || (y > boardY + boardHeight - 1 + posAdd))) &&
                    //locked & unlocked keyword
                    (! ((y==17) && (x < 0 + 8))) &&
                    
                    //LEVEL:[level nr]
                    (! ((y==16) && (x < 8))) &&
                    //B:BACK
                    (! ((y==16) && (x >=12) && (x < 12+6))) &&
                    //A:PLAY
                    (! ((y==17) && (x >=12) && (x < 12+6))))


                {
                   // if (get_bkg_tile_xy(x,y) != levelSelectBackground[i16])
                   // {
                        set_bkg_tile_xy(x, y, gameBackground[i16]);
                   // }
                }
                i16++;
            }
        }

        //LEVEL:
        printLevelSelectGame(0, 16, "LEVEL:", 61);
        
        //[LEVEL NR] 2 chars
        printNumber(6, 16, selectedLevel, 2, 61);
        
        //B:BACK
        printLevelSelectGame(12, 16, "b:BACK", 61);
        
        //A:PLAY
        printLevelSelectGame(12, 17, "a:PLAY", 61);
        
        //Locked & Unlocked keywoard
        if(LevelLocks[gameMode][difficulty][selectedLevel-1] == 0)
        {
            printLevelSelectGame(0, 17, "LOCKED  ", 79);
        }
        else
        {
            if(LevelLocks[gameMode][difficulty][selectedLevel-1] == 1)
            {
                printLevelSelectGame(0, 17, "UNLOCKED", 79);
            }
        }
        
        //Draw arrows for vertical / horizontal movement
        if(gameMode != gmRotate)
        {

            //corners to empty
            set_bkg_tile_xy(boardX -1, boardY -1, 0x31);
            set_bkg_tile_xy(boardX + boardWidth, boardY -1, 0x31);
            set_bkg_tile_xy(boardX + boardWidth, boardY + boardHeight, 0x31);
            set_bkg_tile_xy(boardX -1, boardY + boardHeight, 0x31);

            for (x = 0; x != boardWidth; x++)
            {
                set_bkg_tile_xy(boardX + x, boardY -1, arrowDown);
                set_bkg_tile_xy(boardX + x, boardY + boardHeight, arrowUp);
            }

            for (y = 0; y != boardHeight; y++)
            {
                set_bkg_tile_xy(boardX - 1, boardY + y, arrowRight);
                set_bkg_tile_xy(boardX + boardWidth, boardY + y, arrowLeft);
            }
        }
    }

    if (redrawLevelbit || clearbit)
    {
        //only update what changed
        // i16 = 0; 
        // for (y = 0; y < boardHeight; y++)
        // {
        //     for(x = 0; x <boardWidth; x++)
        //     {
        //         if (get_bkg_tile_xy(boardX +x, boardY + y) != level[i16 + x])
        //         {
        //             set_bkg_tile_xy(boardX +x , boardY + y, level[i16 + x]);
        //         }
        //     }
        //     i16+=boardWidth;
        // }
        set_bkg_tiles(boardX, boardY, boardWidth, boardHeight, level);
        clearbit = 0;
        redrawLevelbit = 0;
    }
}

void initLevelSelect()
{
    set_bkg_data(0, 128, Blocks);
    //need to clear background & it wil also draw the level
    clearbit = 1;
    redrawLevelbit = 1;
    SelectMusic(musTitle, 1);
}

void levelSelect()
{
    uint8_t delay;
    delay = 1;
    initLevelSelect();
    while (gameState == gsLevelSelect)
    {        
        updateBackgroundLevelSelect();   

        //if fading (from white) wait till fade is done
        while (!fade())
        {
            performantdelay(1);
        }
        
        prevJoyPad = joyPad;
        joyPad = joypad();
        if ((joyPad & J_B) && (!(prevJoyPad & J_B)))
        {
            playMenuBackSound();
            gameState = gsTitle;
            //set fadeout & wait for it to be done and set flag to start fadein
            startfade(FADEOUT, 0);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 1);
        }
        if (((joyPad & J_A) && (!(prevJoyPad & J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            if(LevelLocks[gameMode][difficulty][selectedLevel-1] == 1)
            {
                gameState = gsGame;
                playMenuAcknowlege();
            }
            else
            {
                playErrorSound();
            }
        }
        if ((joyPad & J_LEFT) && (delay == 0))
        {
            if (difficulty == diffRandom)
            {
                //ned new seed based on time
                playMenuSelectSound();
                randomSeed = sys_time;
                initLevel(randomSeed);
                //redraw background + level as size can differ
                clearbit = 1;
            }
            else
            {
                if (selectedLevel > 1)
                {
                    playMenuSelectSound();
                    selectedLevel--;
                    initLevel(randomSeed);
                    //no need to use clearbit (redraw background)
                    //as level dimensions remain the same
                    redrawLevelbit = 1;
                }
            }
            if(LevelLocks[gameMode][difficulty][selectedLevel-1] == 0)
            {
                printLevelSelectGame(0, 17, "LOCKED  ", 79);
            }
            else
            {
                if(LevelLocks[gameMode][difficulty][selectedLevel-1] == 1)
                {
                    printLevelSelectGame(0, 17, "UNLOCKED", 79);
                }
            }
            printNumber(6, 16, selectedLevel, 2, 61);
            delay = 6;
        }
        if ((joyPad & J_RIGHT) && (delay == 0))
        {
            if (difficulty == diffRandom)
            {
                playMenuSelectSound();
                //ned new seed based on time
                randomSeed = sys_time;
                initLevel(randomSeed);
                clearbit = 1;
            }
            else
            {
                if (selectedLevel < maxLevel)
                {
                    playMenuSelectSound();
                    selectedLevel++;
                    initLevel(randomSeed);
                    redrawLevelbit = 1;
                }
            }
            if(LevelLocks[gameMode][difficulty][selectedLevel-1] == 0)
            {
                printLevelSelectGame(0, 17, "LOCKED  ", 79);
            }
            else
            {
                if(LevelLocks[gameMode][difficulty][selectedLevel-1] == 1)
                {
                    printLevelSelectGame(0, 17, "UNLOCKED", 79);
                }
            }
            printNumber(6, 16, selectedLevel, 2, 61);
            delay = 6;
        }
        updateSwitches();
        performantdelay(1);
        if (delay != 0)
        {
            delay--;
        }
    }
}

void updateBackgroundGame() 
{
    //background
    if(clearbit)
    {
        i16 = 0;
        for (y = 0; y != gameBackgroundHeight; y++)
        {
            for (x = 0; x != gameBackgroundWidth; x++)
            {       
                if ((((x < boardX -posAdd) || (x > boardX + boardWidth - 1 + posAdd) ||
                    (y < boardY -posAdd) || (y > boardY + boardHeight - 1 + posAdd))) &&
                     //LEVEL:XX (XX = 2 char levelnr)
                    (! ((y==16) && (x < 8))) &&
                    //B:BACK
                    (! ((y==16) && (x >11) && (x < 12+6))) && 
                    //A:XXXXXX (XXXXXX="ROTATE" or XXXXXX="SLIDE " or XXXXXX="ROSLID")
                    (! ((y==17) && (x >11) && (x < 12+8))) && 
                    //MOVES:XXXXX (XXXXX 5 char moves)
                    (! ((y==17) && (x < 6 + 5))))

                {

                    //if (get_bkg_tile_xy(x,y) != gameBackground[i16])
                    //{
                        set_bkg_tile_xy(x, y, gameBackground[i16]);
                    //}
                }
                i16++;
            }
        }

         //LEVEL:
        printLevelSelectGame(0, 16, "LEVEL:", 61);
        
        //[LEVEL NR] 2 chars
        printNumber(6, 16, selectedLevel, 2, 61);
        
        //B:BACK
        printLevelSelectGame(12, 16, "b:BACK", 61);
        
        //MOVES:
        printLevelSelectGame(0, 17, "MOVES:", 61);

        //A:XXXXXX (XXXXXX="ROTATE" or XXXXXX="SLIDE " or XXXXXX="ROSLID")
        if(gameMode == gmRotate)
        {
            printLevelSelectGame(12, 17, "a:ROTATE", 61);
        }
        else
        {
            if(gameMode == gmSlide)
            {
                printLevelSelectGame(12, 17, "a:SLIDE ", 61);
            }
            else
            {
                if(gameMode == gmRotateSlide)
                {
                    printLevelSelectGame(12, 17, "a:ROSLID", 61);
                }
            }
        }
        //Draw arrows for vertical / horizontal movement
        if(gameMode != gmRotate)
        {
            //corners to empty
            set_bkg_tile_xy(boardX -1, boardY -1, 0x31);
            set_bkg_tile_xy(boardX + boardWidth, boardY -1, 0x31);
            set_bkg_tile_xy(boardX + boardWidth, boardY + boardHeight, 0x31);
            set_bkg_tile_xy(boardX -1, boardY + boardHeight, 0x31);
            
            for (x = 0; x != boardWidth; x++)
            {
                set_bkg_tile_xy(boardX + x, boardY -1, arrowDown);
                set_bkg_tile_xy(boardX + x, boardY + boardHeight, arrowUp);
            }

            for (y = 0; y != boardHeight; y++)
            {
                set_bkg_tile_xy(boardX - 1, boardY + y, arrowRight);
                set_bkg_tile_xy(boardX + boardWidth, boardY + y, arrowLeft);
            }
        }
    }
    //level
    if(redrawLevelbit || clearbit)
    {
        // i16 = 0; 
        // for (y = 0; y < boardHeight; y++)
        // {
        //     for(x = 0; x <boardWidth; x++)
        //     {
        //         if (get_bkg_tile_xy(boardX +x, boardY + y) != level[i16 + x])
        //         {
        //             set_bkg_tile_xy(boardX +x , boardY + y, level[i16 + x]);
        //         }
        //     }
        //     i16+=boardWidth;
        // }
        set_bkg_tiles(boardX, boardY, boardWidth, boardHeight, level);
        //can put this inside here as well as it most probably changed also when level changed
        printNumber(6,17, moves, 5, 61);
        
        //clearbit is reset here otherwise too early because of "if" clearbit above
        redrawLevelbit = 0;
        clearbit = 0;
    }
    //level done
    if(redrawLevelDoneBit)
    {
        set_bkg_tiles(5, lookUpTable[startPos].y + boardY, 10, 1, leveldone_map);
        redrawLevelDoneBit = 0;
    }

}

void initGame()
{
    SelectMusic(musGame, 1);
    //set background tiles
    set_bkg_data(0, 128, Blocks);
    //set sprite for selector / cursor
    initCursors();
    setCursorPos(0, boardX + selectionX, boardY + selectionY);
    showCursors();

    clearbit = 1;
    redrawLevelbit = 1;
    redrawLevelDoneBit = 0;

}

void game()
{
    initGame();
    uint8_t delay;
    delay = 1; 
    while(gameState == gsGame)
    {       
        updateBackgroundGame();
        updateCursorFrame();

        prevJoyPad = joyPad;
        joyPad = joypad();
        if ((joyPad & J_DOWN) && (delay == 0))
        {
            if(!levelDone)
            {
                playGameMoveSound();
                //if not touching border on bottom
                if (selectionY + 1 < boardHeight + posAdd)
                {
                    selectionY += 1;
                }
                else
                //set to border on top
                {
                    selectionY = -posAdd;
                }
                setCursorPos(0, boardX + selectionX, boardY + selectionY);
                delay = 10;
            }
        } 
        if ((joyPad & J_UP) && (delay == 0))
        {
            if (!levelDone)
            {
                //if not touching border on top
                playGameMoveSound();
                if (selectionY -1 >= -posAdd)
                {
                    selectionY -= 1;
                }
                else
                //set to border on bottom
                {
                    selectionY = boardHeight -1 +posAdd;
                }
                setCursorPos(0, boardX + selectionX, boardY + selectionY);
                delay = 10;
            }
        }
        if ((joyPad & J_RIGHT) && (delay == 0))
        {
            if (!levelDone)
            {
                playGameMoveSound();
                //if not touching border on right                
                if(selectionX + 1 < boardWidth + posAdd)
                {
                    selectionX += 1;
                }
                else
                //set to border on left
                {
                    selectionX = -posAdd;
                }
                setCursorPos(0, boardX + selectionX, boardY + selectionY);
                delay = 10;
            }
        }
        if ((joyPad & J_LEFT) && (delay == 0))
        {
            if(!levelDone)
            {
                playGameMoveSound();
                //if not touching border on left
                if( selectionX -1 >= -posAdd)
                {
                    selectionX -= 1;
           
                }
                //set to border on right
                else
                {
                    selectionX = boardWidth -1 + posAdd;
                }
                setCursorPos(0, boardX + selectionX, boardY + selectionY);
                delay = 10;
            }
        }
        if ((joyPad & J_A) && (!(prevJoyPad & J_A)))
        {
            if(!levelDone)
            {
                if ((selectionX > -1) && (selectionX < boardWidth) &&
                    (selectionY > -1) && (selectionY < boardHeight))
                {   
                    if (gameMode != gmSlide)
                    {
                        rotateBlock((uint8_t)selectionX + ((uint8_t)selectionY * boardWidth));
                        moves++;
                        redrawLevelbit = 1;
                        playGameAction();
                    }
                    else
                    {
                         playErrorSound();
                    }
                }
                else
                {
                    if ((selectionX > -1) && (selectionX < boardWidth))
                    {
                        if (selectionY == -1)
                        {
                            moveBlockDown((uint8_t)selectionX + ((uint8_t)(selectionY+1) * boardWidth));
                            moves++;
                            redrawLevelbit = 1;
                             playGameAction();
                        }
                        else
                        {
                            if (selectionY == boardHeight)
                            {
                                moveBlockUp((uint8_t)selectionX + ((uint8_t)(selectionY-1) * boardWidth));
                                moves++;
                                redrawLevelbit = 1;
                                 playGameAction();
                            }
                        }
                    }
                    else
                    {
                        if ((selectionY > -1) && (selectionY < boardHeight))    
                        {
                            if (selectionX == -1)
                            {
                                moveBlockRight((uint8_t)(selectionX + 1) + ((uint8_t)selectionY * boardWidth));
                                moves++;
                                redrawLevelbit = 1;
                                 playGameAction();
                            }
                            else
                            {
                                if (selectionX == boardWidth)
                                {
                                    moveBlockLeft( (uint8_t)(selectionX - 1) + ((uint8_t)selectionY * boardWidth));
                                    moves++;
                                    redrawLevelbit = 1;
                                     playGameAction();
                                }
                            }
                        }
                        else
                        {
                            playErrorSound();
                        }
                    }
                }
                updateConnected();
                levelDone = isLevelDone();
                if(levelDone)
                {
                    //update level one last time so we are at final state
                    //as it won't be updated anymore as long as level done is displayed
                    //1 forces level to be drawn (only) one last time the other call uses levelDone                  
                    redrawLevelbit = 1;
                    redrawLevelDoneBit = 1;
                    updateBackgroundGame();
                    //need to wait until its actually drawn on screen.
                    performantdelay(1);
                    SelectMusic(musLevelClear, 0);
                    //hide cursor it's only sprite we use
                    hideCursors();
                }
            }
            else
            {
                //goto next level
                if (difficulty == diffRandom)
                {
                    //ned new seed based on time
                    randomSeed = sys_time;
                    initLevel(randomSeed);
                    //redraw level + background 
                    //otherwise with random if level is smaller than previous one the level is not cleared
                    //completely
                    clearbit = 1;  
                    redrawLevelbit = 1;
                    SelectMusic(musGame, 1);
                    //show cursor again (it's actually to early but i'm not fixing that)
                    showCursors();
                }
                else
                {   
                    //goto next level if any                     
                    if (selectedLevel < maxLevel)
                    {
                        selectedLevel++;
                        LevelLocks[gameMode][difficulty][selectedLevel-1] = 1;
                        initLevel(randomSeed);
                        //redraw levelnr + level + background
                        clearbit = 1;
                        SelectMusic(musGame, 1);
                        //show cursor again (it's actually to early but i'm not fixing that)
                        showCursors();
                    }
                    else //Goto some congrats screen
                    {
                        gameState = gsLevelsCleared;
                        startfade(FADEOUT, 0);
                        while(!fade())
                        {
                            performantdelay(1);
                        }
                        startfade(FADEIN, 1); 
                    }
                }
            }
        }
        if(joyPad & J_B)
        {
            if(!levelDone)
            {
                hideCursors();
                playMenuBackSound();
                gameState = gsLevelSelect;                
                //need to reset the level to initial state when going back to level selector
                //could not find a better way unfortunatly
                //also we do not want to reset the randomseed used for random level generating
                //or a new level would have been created when going back we only want the level
                //with random to change when pressing left and right in the level selector
                //this way it stays consistent with the normal levels
                //and the player can replay the level if he wants to
                initLevel(randomSeed);
            }
        }
        updateSwitches();
        performantdelay(1);
        if (delay != 0)
        {
            delay--;
        }
    }
}

void updateBackgroundTitleScreen()
{
    //if background needs clearing draw background and menu
    if(clearbit)
    {
        set_bkg_tiles(0, 0, titlescreenMapWidth, titlescreenMapHeight, titlescreen_map);
        
        if (titleStep == tsMainMenu)
        {
            printTitle(6,8, "MAIN MENU", 98);
            printTitle(8,10, "START", 98);
            printTitle(8,11, "HELP", 98);
        }
        else
        {
            if (titleStep == tsDifficulty)
            {
                printTitle(6,8, "VERY EASY", 98);
                printTitle(6,9, "EASY", 98);
                printTitle(6,10, "NORMAL", 98);
                printTitle(6,11, "HARD", 98);
                printTitle(6,12, "VERY HARD", 98);
                printTitle(6,13, "RANDOM", 98);
            }
            else
            {
                if (titleStep == tsGameMode)
                {
                    if (mainMenu == mmStartGame)
                    {
                        printTitle(4,8, "SELECT  MODE", 98);
                    }
                    else
                    {
                        printTitle(6,8, "MODE HELP", 98);
                    }
                    printTitle(7,10, "ROTATE", 98);
                    printTitle(7,11, "SLIDE", 98);
                    printTitle(7,12, "ROSLID", 98);

                }
            }
        }
    }
    
    if(clearbit || redrawLevelbit)
    {
        //set menu tile
        if (titleStep == tsMainMenu)
        {
            //clear menu selector
            for (y = mmStartGame ; y < mmCount; y++)
            {
                if (y != mainMenu)
                {
                    printTitle(7, 10 + y, " ", 98);
                }
            }

            set_bkg_tile_xy(7, 10 + mainMenu, leftMenu); 
        }
        else
        {
            //set menu tile
            if (titleStep == tsGameMode)
            {
                //clear menu selector
                for (y = gmRotate; y < gmCount; y++)
                {
                    if (y != gameMode)
                    {
                        printTitle(6, 10 + y, " ", 98);
                    }
                }

                set_bkg_tile_xy(6, 10 + gameMode, leftMenu); 
            }
            else
            if(titleStep == tsDifficulty)
            {
                //clear menu selector
                for (y = diffVeryEasy; y < diffCount; y++)
                {
                    if (y != difficulty)
                    {
                        printTitle(5, 8 + y, " ", 98);
                    }
                }

                set_bkg_tile_xy(5, 8 + difficulty, leftMenu);
            }
        }
        redrawLevelbit = 0;
        clearbit = 0;        
    }

}

void initTitleScreen()
{
    set_bkg_data(0, 128, titlescreen_data);
    clearbit = 1;
    SelectMusic(musTitle, 1);
}

void titleScreen()
{
    initTitleScreen();
    
    while (gameState == gsTitle)
    {
        updateBackgroundTitleScreen();
        //wait on fadein        
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }

        prevJoyPad = joyPad;
        joyPad = joypad();
        if ((joyPad & J_UP) && (!(prevJoyPad & J_UP)))
        {
            if (titleStep == tsMainMenu)
            {
                if(mainMenu > mmStartGame)
                {
                    playMenuSelectSound();
                    mainMenu--;
                    redrawLevelbit = 1;
                }
            } 
            else
            {
                if (titleStep == tsGameMode)
                {
                    if(gameMode > gmRotate)
                    {
                        playMenuSelectSound();
                        gameMode--;
                        redrawLevelbit = 1;
                    }
                } 
                else
                {
                    if(titleStep == tsDifficulty)
                    {
                        if(difficulty > diffVeryEasy)
                        {
                            playMenuSelectSound();
                            difficulty--;
                            redrawLevelbit = 1;
                        }
                    }
                }
            }
        }
        if ((joyPad & J_DOWN) && (!(prevJoyPad & J_DOWN)))
        {
            if (titleStep == tsMainMenu)
            {
                if(mainMenu < mmHelp)
                {
                    playMenuSelectSound();
                    mainMenu++;
                    redrawLevelbit = 1;
                }
            }
            else
            {
                if (titleStep == tsGameMode)
                {
                    if(gameMode < gmRotateSlide)
                    {
                        playMenuSelectSound();
                        gameMode++;
                        redrawLevelbit = 1;
                    }
                } 
                else
                {
                    if(difficulty < diffRandom)
                    {
                        playMenuSelectSound();
                        difficulty++;
                        redrawLevelbit = 1;
                    }
                }
            }
        }
        if ((joyPad & J_B) && (!(prevJoyPad && J_B)))
        {
            if (titleStep > tsMainMenu)
            {
                titleStep--;
                clearbit = 1;
                playMenuBackSound();
            }
        }
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            if(mainMenu == mmHelp)
            {
                if (titleStep < tsGameMode)
                {
                    titleStep++;
                    clearbit = 1;
                }
                else
                {
                    switch (gameMode)
                    {
                        case gmRotate:
                            gameState = gsHelpRotate;
                            break;
                        case gmSlide:
                            gameState = gsHelpSlide;
                            break;
                        case gmRotateSlide:
                            gameState = gsHelpRotateSlide;
                            break; 
                    }
                    startfade(FADEOUT, 1);
                    while(!fade())
                    {
                        performantdelay(1);
                    }
                    startfade(FADEIN, 0);
                }
            }
            if(mainMenu == mmStartGame)
            {
            
                if (titleStep < tsDifficulty)
                {
                    titleStep++;
                    clearbit = 1;
                }
                else
                {
                    if (difficulty == diffRandom)
                    {
                        selectedLevel = 1;
                    }
                    else
                    //select last unlocked level
                    {
                        for (i = 0; i < levelCount; i++)
                        {
                            if(LevelLocks[gameMode][difficulty][i] == 1)
                            {
                                selectedLevel = i + 1;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    gameState = gsLevelSelect;
                    startfade(FADEOUT, 1);
                    while(!fade())
                    {
                        performantdelay(1);
                    }
                    startfade(FADEIN, 0);
                } 
            }  
        }
        updateSwitches(); 
        performantdelay(1);    
    } 
}

void initLevelsCleared()
{
    set_bkg_data(0, 91, congratsScreenTiles);
    set_bkg_tiles(0, 0, congratsMapWidth, congratsMapHeight, congratsMap);
    if(difficulty == diffVeryEasy)
    {
        printCongratsScreen(1, 7, "VERY EASY  LEVELS", 0);
    }
    if(difficulty == diffEasy)
    {
        printCongratsScreen(3, 7, "EASY LEVELS", 0);
    }
    if(difficulty == diffNormal)
    {
        printCongratsScreen(3, 7, "NORMAL LEVELS", 0);
    }
    if(difficulty == diffHard)
    {
        printCongratsScreen(3, 7, "HARD LEVELS", 0);
    }
    if(difficulty == diffVeryHard)
    {
        printCongratsScreen(1, 7, "VERY HARD  LEVELS", 0);
    }
    SelectMusic(musAllLevelsClear, 1);
}

void levelsCleared()
{
    initLevelsCleared();
   
    while (gameState == gsLevelsCleared)
    {
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }
        prevJoyPad = joyPad;
        joyPad = joypad();
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) || 
            ((joyPad & J_B) && (!(prevJoyPad && J_B))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            titleStep = tsMainMenu;
            gameState = gsTitle;
            startfade(FADEOUT, 1);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 1);
        }
        updateSwitches(); 
        performantdelay(1);    
    }
}

void initHelpRotateSlide()
{
    set_bkg_data(0, 128, Blocks);
    set_bkg_tiles(0, 0, helpRotateSlideMapWidth, helpRotateSlideMapHeight, helpRotateSlideMap);
    SelectMusic(musTitle, 1);
}

void helpRotateSlide()
{
    initHelpRotateSlide();
   
    while (gameState == gsHelpRotateSlide)
    {
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }
        prevJoyPad = joyPad;
        joyPad = joypad();
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))

        {
            playMenuAcknowlege();
            gameState = gsHelpRotateSlide2;
            startfade(FADEOUT, 0);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 0);
        }
        updateSwitches(); 
        performantdelay(1);    
    }
}

void initHelpRotateSlide2()
{
    set_bkg_data(0, 128, Blocks);
    set_bkg_tiles(0, 0, helpRotateSlide2MapWidth, helpRotateSlide2MapHeight, helpRotateSlide2Map);
    SelectMusic(musTitle, 1);
    cursorFrameCount = 0;

    //DRAW CURSOR SPRITES
    initCursors();

    setCursorPos(0, 1, 5);
    setCursorPos(1, 14, 5);
    setCursorPos(2, 2, 12);
    setCursorPos(3, 15, 12);

    showCursors();
}

void helpRotateSlide2()
{
    initHelpRotateSlide2();
   
    while (gameState == gsHelpRotateSlide2)
    {
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }
        updateCursorFrame();
        prevJoyPad = joyPad;
        joyPad = joypad();
        if (((joyPad & J_A) && (!(prevJoyPad && J_A)))  ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            gameState = gsTitle;
            startfade(FADEOUT, 0);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 1);
        }
        updateSwitches(); 
        performantdelay(1);    
    }

    hideCursors();
}

void initHelpRotate()
{
    set_bkg_data(0, 128, Blocks);
    set_bkg_tiles(0, 0, helpRotateMapWidth, helpRotateMapHeight, helpRotateMap);
    SelectMusic(musTitle, 1);
}

void helpRotate()
{
    initHelpRotate();
   
    while (gameState == gsHelpRotate)
    {
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }
        prevJoyPad = joyPad;
        joyPad = joypad();
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            gameState = gsHelpRotate2;
            startfade(FADEOUT, 0);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 0);
        }
        updateSwitches(); 
        performantdelay(1);    
    }
}

void initHelpRotate2()
{
    set_bkg_data(0, 128, Blocks);
    set_bkg_tiles(0, 0, helpRotate2MapWidth, helpRotate2MapHeight, helpRotate2Map);
    SelectMusic(musTitle, 1);

    //DRAW CURSOR SPRITES
    initCursors();
    setCursorPos(0, 2, 7);
    setCursorPos(1, 15, 7);
    showCursors();
}

void helpRotate2()
{
    initHelpRotate2();
   
    while (gameState == gsHelpRotate2)
    {
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }

       updateCursorFrame();

        prevJoyPad = joyPad;
        joyPad = joypad();
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            gameState = gsTitle;
            startfade(FADEOUT, 0);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 1);
        }
        updateSwitches(); 
        performantdelay(1);    
    }

    hideCursors();
}


void initHelpSlide()
{
    set_bkg_data(0, 128, Blocks);
    set_bkg_tiles(0, 0, helpSlideMapWidth, helpSlideMapHeight, helpSlideMap);
    SelectMusic(musTitle, 1);
}

void helpSlide()
{
    initHelpSlide();
   
    while (gameState == gsHelpSlide)
    {
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }
        prevJoyPad = joyPad;
        joyPad = joypad();
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            gameState = gsHelpSlide2;
            startfade(FADEOUT, 0);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 0);
        }
        updateSwitches(); 
        performantdelay(1);    
    }
}

void initHelpSlide2()
{
    set_bkg_data(0, 128, Blocks);
    set_bkg_tiles(0, 0, helpSlide2MapWidth, helpSlide2MapHeight, helpSlide2Map);
    SelectMusic(musTitle, 1);
    
    //DRAW CURSOR SPRITES
    initCursors();

    setCursorPos(0, 1, 8);
    setCursorPos(1, 14, 8);

    showCursors();
}

void helpSlide2()
{
    initHelpSlide2();
   
    while (gameState == gsHelpSlide2)
    {
        while (!fade())
        {
            SHOW_BKG;
            performantdelay(1);
        }
        
       updateCursorFrame();
        
        prevJoyPad = joyPad;
        joyPad = joypad();
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            gameState = gsTitle;
            startfade(FADEOUT, 0);
            while(!fade())
            {
                performantdelay(1);
            }
            startfade(FADEIN, 1);
        }
        updateSwitches(); 
        performantdelay(1);    
    }
    
    hideCursors();
}

//intialisation of game & global variables
void init() 
{
    if (_cpu == CGB_TYPE) 
    {
        cpu_fast();
        set_bkg_palette(0, 1, blackPallette);
        set_sprite_palette(0, 1, sprPalette);
    }
    else
    {
        BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
    }
    DISPLAY_ON;
    ENABLE_RAM_MBC1;
    prevBoardHeight = 0;
    prevBoardWidth = 0;
    maxcc = 0;
    fadeCounter = 0;
    fadein = 1;
    difficulty = diffNormal;
    selectedLevel = 1;
    mainMenu = mmStartGame;
    gameState = gsTitle;
    titleStep = tsMainMenu;
    gameMode = gmRotate;
    joyPad = joypad();
    prevJoyPad = joyPad;
    startfade(FADEIN, 1);
    initSound();
    initMusic();
    initSaveState();
 }

//entrypoint
void main() 
{
    init();
    while(1)
    {
        //gamestate handling
        switch (gameState)
        {
            case gsTitle:
                titleScreen();
                if (gameMode == gmRotate)
                {
                    posAdd = 0;
                }
                else
                {
                    posAdd = 1;
                }
                //set randomseet to systime here
                //it will be reused all the time
                //with the level generating
                //but not when going back from
                //level playing to level selector
                //when calling init level there
                randomSeed = sys_time;
                initLevel(randomSeed);                
                break;
            case gsLevelSelect:
                levelSelect();
                break;
            case gsGame:
                game();
                break;
            case gsLevelsCleared:
                levelsCleared();
                break;
            case gsHelpSlide:
                helpSlide();
                break;
            case gsHelpSlide2:
                helpSlide2();
                break;
            case gsHelpRotateSlide:
                helpRotateSlide();
                break;
            case gsHelpRotateSlide2:
                helpRotateSlide2();
                break;
            case gsHelpRotate:
                helpRotate();
                break;
            case gsHelpRotate2:
                helpRotate2();
                break;
        }        
    }

    
}