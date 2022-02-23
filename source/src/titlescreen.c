#pragma bank 255

#include <gbdk/platform.h>
#include <gbdk/gbdecompress.h>
#include <stdint.h>
#include <time.h>

#include "../res/titletiles.h"
#include "../res/titlescreenmap.h"
#include "commonvars.h"
#include "helperfuncs.h"
#include "graphics.h"
#include "sound.h"
#include "printfuncs.h"
#include "savestate.h"

BANKREF(TITLESCREEN)

void updateBackgroundTitleScreen() NONBANKED
{
    //if background needs clearing draw background and menu
    if(clearbit)
    {
        pushBank();
        SWITCH_ROM(BANK(titlescreenmap));
        set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, titlescreenMapWidth, titlescreenMapHeight, titlescreenMap);
        popBank();

        if (titleStep == tsMainMenu)
        {
            printTitle(6 + SCREENSTARTX, 8 + SCREENSTARTY, "MAIN MENU", 9, 0);
            printTitle(7 + SCREENSTARTX, 10 + SCREENSTARTY, "START", 5, 0);
            printTitle(7 + SCREENSTARTX, 11 + SCREENSTARTY, "HELP",4, 0);
            printTitle(7 + SCREENSTARTX, 12 + SCREENSTARTY, "OPTIONS", 7, 0);
        }
        else
        {
            if (titleStep == tsDifficulty)
            {
                printTitle(6 + SCREENSTARTX, 8 + SCREENSTARTY, "VERY EASY", 9, 0);
                printTitle(6 + SCREENSTARTX, 9 + SCREENSTARTY, "EASY", 4, 0);
                printTitle(6 + SCREENSTARTX, 10 + SCREENSTARTY, "NORMAL", 6, 0);
                printTitle(6 + SCREENSTARTX, 11 + SCREENSTARTY, "HARD", 4, 0);
                printTitle(6 + SCREENSTARTX, 12 + SCREENSTARTY, "VERY HARD", 9, 0);
                printTitle(6 + SCREENSTARTX, 13 + SCREENSTARTY, "RANDOM", 6, 0);
            }
            else
            {
                if (titleStep == tsGameMode)
                {
                    if (mainMenu == mmStartGame)
                    {
                        printTitle(4 + SCREENSTARTX, 8 + SCREENSTARTY, "SELECT  MODE", 12, 0);
                    }
                    else
                    {
                        printTitle(6 + SCREENSTARTX, 8 + SCREENSTARTY, "MODE HELP", 9, 0);
                    }
                    printTitle(7 + SCREENSTARTX, 10 + SCREENSTARTY, "ROTATE", 7, 0);
                    printTitle(7 + SCREENSTARTX, 11 + SCREENSTARTY, "SLIDE", 5, 0);
                    printTitle(7 + SCREENSTARTX, 12 + SCREENSTARTY, "ROSLID", 6, 0);

                }
                else
                {
                    if (titleStep == tsOptions)
                    {
                        printTitle(7 + SCREENSTARTX, 8 + SCREENSTARTY, "OPTIONS", 7, 0);
                    }
                }
            }
        }
    }
    
    if(clearbit || redrawLevelbit)
    {
        if(titleStep == tsOptions)
        {
            if(isMusicOn())
            {
                printTitle(6 + SCREENSTARTX, 10 + SCREENSTARTY, "MUSIC ON ", 9, 0);
            }
            else
            {
                printTitle(6 + SCREENSTARTX, 10 + SCREENSTARTY, "MUSIC OFF", 9, 0);
            }

            if(isSoundOn())
            {
                printTitle(6 + SCREENSTARTX, 11 + SCREENSTARTY, "SOUND ON ",9, 0);
            }
            else
            {
                printTitle(6 + SCREENSTARTX, 11 + SCREENSTARTY, "SOUND OFF",9, 0);
            }
        }
        //set menu tile
        if (titleStep == tsMainMenu)
        {
            //clear menu selector
            for (y = mmStartGame ; y < mmCount; y++)
            {
                if (y != mainMenu)
                {
                    printTitle(6 + SCREENSTARTX, 10 + y + SCREENSTARTY, " ", 1, 0);
                }
            }

            set_bkg_tile_xy(6 + SCREENSTARTX, 10 + mainMenu + SCREENSTARTY, leftMenu); 
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
                        printTitle(6 + SCREENSTARTX, 10 + y + SCREENSTARTY, " ", 1, 0);
                    }
                }

                set_bkg_tile_xy(6 + SCREENSTARTX, 10 + gameMode + SCREENSTARTY, leftMenu); 
            }
            else
            {
                if(titleStep == tsDifficulty)
                {
                    //clear menu selector
                    for (y = diffVeryEasy; y < diffCount; y++)
                    {
                        if (y != difficulty)
                        {
                            printTitle(5 + SCREENSTARTX, 8 + y + SCREENSTARTY, " ", 1, 0);
                        }
                    }

                    set_bkg_tile_xy(5 + SCREENSTARTX, 8 + difficulty + SCREENSTARTY, leftMenu);
                }
                else
                {
                    if(titleStep == tsOptions)
                    {
                        //clear menu selector
                        for (y = opMusic; y < opCount; y++)
                        {
                            if (y != option)
                            {
                                printTitle(5 + SCREENSTARTX, 10 + y + SCREENSTARTY, " ", 1, 0);
                            }
                        }

                        set_bkg_tile_xy(5 + SCREENSTARTX, 10 + option + SCREENSTARTY, leftMenu);
                    }    
                }
            }
        }
        redrawLevelbit = 0;
        clearbit = 0;        
    }

}

void initTitleScreen() NONBANKED
{
    pushBank();
    SWITCH_ROM(BANK(titletiles));
    set_bkg_data(0, gb_decompress(titleTiles, tileBuffer) >> 4, tileBuffer);
    popBank();
    
    clearBackgroundLayer(0);
    
    pushBank();
    SWITCH_ROM(BANK(titlescreenmap));
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, titlescreenMapWidth, titlescreenMapHeight, titlescreenMap);
    popBank();
    
    clearbit = 1;
    SelectMusic(musTitle, 1);
}

void titleScreen() BANKED
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
                    else
                    {
                        if(titleStep == tsOptions)
                        {
                            if(option > opMusic)
                            {
                                playMenuSelectSound();
                                option--;
                                redrawLevelbit = 1;
                            }
                        }
                    }
                }
            }
        }
        if ((joyPad & J_DOWN) && (!(prevJoyPad & J_DOWN)))
        {
            if (titleStep == tsMainMenu)
            {
                if(mainMenu < mmCount-1)
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
                    if(gameMode < gmCount-1)
                    {
                        playMenuSelectSound();
                        gameMode++;
                        redrawLevelbit = 1;
                    }
                } 
                else
                {
                    if(titleStep == tsDifficulty)
                    {
                        if(difficulty < diffCount-1)
                        {
                            playMenuSelectSound();
                            difficulty++;
                            redrawLevelbit = 1;
                        }
                    }
                    else
                    {
                        if(titleStep == tsOptions)
                        {
                            if(option < opCount-1)
                            {
                                playMenuSelectSound();
                                option++;
                                redrawLevelbit = 1;
                            }
                        }
                    }
                }
            }
        }
        if ((joyPad & J_B) && (!(prevJoyPad && J_B)))
        {
            if (titleStep == tsOptions)
            {
                titleStep = tsMainMenu;
                clearbit = 1;
                playMenuBackSound();
            }
            else
            {
                if (titleStep > tsMainMenu)
                {
                    titleStep--;
                    clearbit = 1;
                    playMenuBackSound();
                }
            }
        }
        if (((joyPad & J_A) && (!(prevJoyPad && J_A))) ||
            ((joyPad & J_START) && (!(prevJoyPad && J_START))))
        {
            playMenuAcknowlege();
            if (mainMenu == mmOptions)
            {
                if(titleStep != tsOptions)
                {
                    titleStep = tsOptions;
                    clearbit = 1;
                }
                else
                {
                    switch(option)
                    {
                        case opMusic:
                            setMusicOn(!isMusicOn());
                            setMusicOnSaveState(isMusicOn());
                            redrawLevelbit = 1;
                            break;
                        case opSound:
                            setSoundOn(!isSoundOn());;
                            setSoundOnSaveState(isSoundOn());
                            redrawLevelbit = 1;
                            break;
                    }
                }
            }

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
                    {
                        selectedLevel = lastUnlockedLevel(gameMode, difficulty);
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
