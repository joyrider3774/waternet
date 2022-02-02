#include <gbdk/platform.h>
#include <stdint.h>
#include <stdio.h>
#include <rand.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "commonvars.h"
#include "levelselect.h"
#include "sound.h"
#include "cursor.h"
#include "graphics.h"
#include "printfuncs.h"
#include "helpscreens.h"
#include "game.h"
#include "titlescreen.h"
#include "levelscleared.h"
#include "savestate.h"
#include "level.h"
#include "helperfuncs.h"

#ifdef NINTENDO
#include "sgb_border.h"
#include "../res/gb_border.h"

#ifdef BATTERYLESSSAVE
#include "../flasher/flasher.h"
#endif

#endif


//intialisation of game & global variables
void init() 
{
    sgb_enabled = 0;
    initBankStack();
    #ifdef NINTENDO
    if(_cpu == CGB_TYPE)
    {
        cpu_fast();
    }
    #endif

#ifdef BATTERYLESSSAVE
#ifndef MEGADUCK
    ENABLE_RAM;
    restore_sram();
    DISABLE_RAM;
#endif
#endif

    DISPLAY_ON;
    
    #ifdef GAMEBOY
        //graphics data is in bank 1
        SWITCH_ROM(1);
        // this delay is required for PAL SNES
        for (i = 4; i != 0; i--)
        {
            wait_vbl_done();
        }
        sgb_enabled = sgb_check();
        set_sgb_border(gb_border_tiles, sizeof(gb_border_tiles), gb_border_map, sizeof(gb_border_map), gb_border_palettes, sizeof(gb_border_palettes));
    #endif
    setBlackPalette();
    prevBoardHeight = 0;
    prevBoardWidth = 0;
    maxcc = 0;
    option = 0;
    paused = 0;
    difficulty = diffNormal;
    selectedLevel = 1;
    mainMenu = mmStartGame;
    gameState = gsTitle;
    titleStep = tsMainMenu;
    gameMode = gmRotate;
    joyPad = joypad();
    prevJoyPad = joyPad;
    initSaveState();
    setMusicOn(isMusicOnSaveState());
    setSoundOn(isSoundOnSaveState());
    startfade(FADEIN, 1);
    initSound();
    initMusic();
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
                randomSeed = clock();
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