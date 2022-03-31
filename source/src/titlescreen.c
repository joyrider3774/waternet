#include <gbdk/platform.h>
#include <gbdk/gbdecompress.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "../res/titletiles.h"
#include "../res/titlescreenmap.h"
#include "commonvars.h"
#include "helperfuncs.h"
#include "graphics.h"
#include "sound.h"
#include "printfuncs.h"
#include "savestate.h"

uint8_t passX, passY;

void updateBackgroundTitleScreen()
{
    //if background needs clearing draw background and menu
    if(clearbit)
    {
        set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, titlescreenMapWidth, titlescreenMapHeight, titlescreenMap);

        if (titleStep == tsMainMenu)
        {
            printTitle(6 + SCREENSTARTX, 8 + SCREENSTARTY,  "MAIN MENU", 9, 0);
            printTitle(7 + SCREENSTARTX, 10 + SCREENSTARTY, "START", 5, 0);
            printTitle(7 + SCREENSTARTX, 11 + SCREENSTARTY, "HELP",4, 0);
            printTitle(7 + SCREENSTARTX, 12 + SCREENSTARTY, "OPTIONS", 7, 0);
            printTitle(7 + SCREENSTARTX, 13 + SCREENSTARTY, "PASSWORD", 8, 0);
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
                    else
                    {
                        if (titleStep == tsPassword)
                        {
                            printTitle(6 + SCREENSTARTX, 8 + SCREENSTARTY, "PASSWORD", 8, 0);
                        }
                    }
                }               
            }
        }
    }
    
    if(clearbit || redrawLevelbit)
    {
        if(titleStep == tsPassword)
        {
            unsigned char passwordSplit[8];
            memcpy(passwordSplit, statePassword, 8);
            printTitle(6 + SCREENSTARTX, 10 + SCREENSTARTY, passwordSplit, 8, 0);
            memcpy(passwordSplit, statePassword+8, 8);
            printTitle(6 + SCREENSTARTX, 12 + SCREENSTARTY, passwordSplit, 8, 0);
            printTitle(6 + SCREENSTARTX, 11 + SCREENSTARTY, "        ", 8, 0);
            printTitle(6 + SCREENSTARTX, 13 + SCREENSTARTY, "        ", 8, 0);
            printTitle(6 + passX + SCREENSTARTX, 11 + (passY << 1) + SCREENSTARTY, "^", 1, 0);
        }
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

void initTitleScreen()
{
    EncryptState(statePassword);
    passX = 0;
    passY = 0;

    set_bkg_data(0, gb_decompress(titleTiles, tileBuffer) >> 4, tileBuffer);
    
    clearBackgroundLayer(0);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, titlescreenMapWidth, titlescreenMapHeight, titlescreenMap);
    
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
        if ((joyPad & J_LEFT) && (!(prevJoyPad & J_LEFT)))
        {
            if(titleStep == tsPassword)
            {
                if(passX == 0)
                {
                    if(passY == 0)
                    {
                        passY = maxPasswordY;
                    } 
                    else
                    {
                        passY--;
                    }
                    passX = maxPasswordX;
                }
                else
                {
                    passX--;
                }
                redrawLevelbit = 1;
            }
        }
        if ((joyPad & J_RIGHT) && (!(prevJoyPad & J_RIGHT)))
        {
            if(titleStep == tsPassword)
            {
                passX++;
                if(passX > maxPasswordX)
                {
                    passY++;
                    if(passY > maxPasswordY)
                    {
                        passY = 0;
                    }
                    passX =  0;
                }
                redrawLevelbit = 1;
            }
        }
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
                        else
                        {
                            if(titleStep == tsPassword)
                            {   
                                statePassword[passY * (maxPasswordX + 1) + passX]++;
                                if (statePassword[passY * (maxPasswordX + 1) + passX] > 90) //Z
                                {
                                    statePassword[passY * (maxPasswordX + 1) + passX] = 65; //A
                                }                                                             
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
                        else
                        {
                            if(titleStep == tsPassword)
                            {                                
                                statePassword[passY * (maxPasswordX + 1) + passX]--;
                                if (statePassword[passY * (maxPasswordX + 1) + passX] < 65) //A
                                {
                                    statePassword[passY * (maxPasswordX + 1) + passX] = 90; //Z
                                }
                                redrawLevelbit = 1;
                            }
                        }
                    }
                }
            }
        }
        if ((joyPad & J_B) && (!(prevJoyPad && J_B)))
        {
            if ((titleStep == tsOptions) || (titleStep == tsPassword))
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
            if (mainMenu == mmPassword)
            {
                if(titleStep != tsPassword)
                {
                    EncryptState(statePassword);
                    passX = 0;
                    passY = 0;
                    titleStep = tsPassword;
                    clearbit = 1;
                }
                else
                {
                    if (DecryptState(statePassword) == 1)
                    {
                        playMenuAcknowlege();
                        titleStep = tsMainMenu;
                        clearbit = 1;
                    }
                    else
                    {
                        playErrorSound();
                    }
                }
            }
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
                            redrawLevelbit = 1;
                            break;
                        case opSound:
                            setSoundOn(!isSoundOn());;
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
