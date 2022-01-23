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
#endif

//intialisation of game & global variables
void init() 
{
    #ifdef NINTENDO
    if(_cpu == CGB_TYPE)
    {
        cpu_fast();
    }
    #endif
    
    setBlackPalette();
    DISPLAY_ON;
    #ifdef GAMEBOY
        pushBank();
        SWITCH_ROM(1);
        set_sgb_border(gb_border_tiles, sizeof(gb_border_tiles), gb_border_map, sizeof(gb_border_map), gb_border_palettes, sizeof(gb_border_palettes));
        popBank();
    #endif

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
                pushBank();
                SWITCH_ROM(BANK(TITLESCREEN));
                titleScreen();
                popBank();
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
                pushBank();
                SWITCH_ROM(BANK(LEVELSELECT));
                levelSelect();
                popBank();
                break;
            case gsGame:
                pushBank();
                SWITCH_ROM(BANK(GAME));
                game();
                popBank();
                break;
            case gsLevelsCleared:
                pushBank();
                SWITCH_ROM(BANK(LEVELSCLEARED));
                levelsCleared();
                popBank();
                break;
            case gsHelpSlide:
                pushBank();
                SWITCH_ROM(BANK(HELPSCREENS));
                helpSlide();
                popBank();
                break;
            case gsHelpSlide2:
                pushBank();
                SWITCH_ROM(BANK(HELPSCREENS));
                helpSlide2();
                popBank();
                break;
            case gsHelpRotateSlide:
                pushBank();
                SWITCH_ROM(BANK(HELPSCREENS));
                helpRotateSlide();
                popBank();
                break;
            case gsHelpRotateSlide2:
                pushBank();
                SWITCH_ROM(BANK(HELPSCREENS));
                helpRotateSlide2();
                popBank();
                break;
            case gsHelpRotate:
                pushBank();
                SWITCH_ROM(BANK(HELPSCREENS));
                helpRotate();
                popBank();
                break;
            case gsHelpRotate2:
                pushBank();
                SWITCH_ROM(BANK(HELPSCREENS));
                helpRotate2();
                popBank();
                break;
        }        
    }
}