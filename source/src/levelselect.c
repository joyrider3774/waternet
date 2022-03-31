#include <gbdk/platform.h>
#include <time.h>

#include "../res/gamebackgroundmap.h"

#include "commonvars.h"
#include "helperfuncs.h"
#include "printfuncs.h"
#include "graphics.h"
#include "savestate.h"
#include "sound.h"
#include "level.h"

//NONBANKED because of gameBackgroundMap
void updateBackgroundLevelSelect() 
{
    //clearbit will clear the background tiles
    //this is only needed for random selection and initial call to set the tiles
    //otherwise the screen acted weird for some reason causing a glitch
    if(clearbit)
    {
        //only redraw changed tiles
        i16 = 0;
        for (y = 0; y != gameBackgroundMapHeight; y++)
        {
            for (x = 0; x != gameBackgroundMapWidth; x++)
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
                        
                        set_bkg_tile_xy(x + SCREENSTARTX, y + SCREENSTARTY, gameBackgroundMap[i16]);                        
                   // }
                }
                i16++;
            }
        }
        //LEVEL:
        printLevelSelectGame(0 + SCREENSTARTX, 16 + SCREENSTARTY, "LEVEL:", 6, 61);
        
        //[LEVEL NR] 2 chars
        printNumber(6 + SCREENSTARTX, 16 + SCREENSTARTY, selectedLevel, 2, 61);
        
        //B:BACK
        printLevelSelectGame(12 + SCREENSTARTX, 16 + SCREENSTARTY, "b:BACK", 6, 61);
        
        //A:PLAY
        printLevelSelectGame(12 + SCREENSTARTX, 17 + SCREENSTARTY, "a:PLAY", 6, 61);
        
        //Locked & Unlocked keywoard
        uint8_t tmpUnlocked = levelUnlocked(gameMode, difficulty, selectedLevel -1);
        if(!tmpUnlocked)
        {
            printLevelSelectGame(0 + SCREENSTARTX, 17 + SCREENSTARTY, "LOCKED  ", 8, 79);
        }
        else
        {
            if(tmpUnlocked)
            {
                printLevelSelectGame(0 + SCREENSTARTX, 17 + SCREENSTARTY, "UNLOCKED", 8, 79);
            }
        }
        
        //Draw arrows for vertical / horizontal movement
        if(gameMode != gmRotate)
        {

            //corners to empty
            set_bkg_tile_xy(boardX -1 + SCREENSTARTX, boardY -1 + SCREENSTARTY, 0x31);
            set_bkg_tile_xy(boardX + boardWidth + SCREENSTARTX, boardY -1 + SCREENSTARTY, 0x31);
            set_bkg_tile_xy(boardX + boardWidth + SCREENSTARTX, boardY + boardHeight + SCREENSTARTY, 0x31);
            set_bkg_tile_xy(boardX -1 + SCREENSTARTX, boardY + boardHeight + SCREENSTARTY, 0x31);

            for (x = 0; x != boardWidth; x++)
            {
                set_bkg_tile_xy(boardX + x + SCREENSTARTX, boardY -1 + SCREENSTARTY, arrowDown);
                set_bkg_tile_xy(boardX + x + SCREENSTARTX, boardY + boardHeight + SCREENSTARTY, arrowUp);
            }

            for (y = 0; y != boardHeight; y++)
            {
                set_bkg_tile_xy(boardX - 1 + SCREENSTARTX, boardY + y + SCREENSTARTY, arrowRight);
                set_bkg_tile_xy(boardX + boardWidth + SCREENSTARTX, boardY + y + SCREENSTARTY, arrowLeft);
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
        set_bkg_tiles(boardX + SCREENSTARTX, boardY + SCREENSTARTY, boardWidth, boardHeight, level);
        clearbit = 0;
        redrawLevelbit = 0;
    }
}

//NONBANKED because setBlockTilesAsBackground
void initLevelSelect()
{
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    //need to clear background & it wil also draw the level
    clearbit = 1;
    redrawLevelbit = 1;
    SelectMusic(musTitle, 1);
}

void levelSelect()
{
    uint8_t tmpUnlocked;
    tmpUnlocked = levelUnlocked(gameMode, difficulty, selectedLevel -1);
    initLevelSelect();
    while (gameState == gsLevelSelect)
    {
        updateBackgroundLevelSelect();

        //if fading wait till fade is done
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
            if(tmpUnlocked)
            {
                gameState = gsGame;
                playMenuAcknowlege();
                startfade(FADEOUT, 0);
                while(!fade())
                {
                    performantdelay(1);
                }
                startfade(FADEIN, 0);
            }
            else
            {
                playErrorSound();
            }
        }
        if ((joyPad & J_LEFT) && (!(prevJoyPad & J_LEFT)))
        {
            if (difficulty == diffRandom)
            {
                //ned new seed based on time
                playMenuSelectSound();
                randomSeed = clock();
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
            tmpUnlocked = levelUnlocked(gameMode, difficulty, selectedLevel -1);
            if(tmpUnlocked == 0)
            {
                printLevelSelectGame(0 + SCREENSTARTX, 17 + SCREENSTARTY, "LOCKED  ", 8, 79);
            }
            else
            {
                if(tmpUnlocked == 1)
                {
                    printLevelSelectGame(0 + SCREENSTARTX, 17 + SCREENSTARTY, "UNLOCKED", 8, 79);
                }
            }
            printNumber(6 + SCREENSTARTX, 16 + SCREENSTARTY, selectedLevel, 2, 61);
        }
        if ((joyPad & J_SELECT) && (!(prevJoyPad & J_SELECT)))
        {
            unsigned char tmpbuf[18];
            EncryptState(tmpbuf);
            printLevelSelectGame(0, 0, tmpbuf, 18, 61);
        }
        if ((joyPad & J_RIGHT) && (!(prevJoyPad & J_RIGHT)))
        {
            if (difficulty == diffRandom)
            {
                playMenuSelectSound();
                //ned new seed based on time
                randomSeed = clock();
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
            tmpUnlocked = levelUnlocked(gameMode, difficulty, selectedLevel -1);
            if(tmpUnlocked == 0)
            {
                printLevelSelectGame(0 + SCREENSTARTX, 17 + SCREENSTARTY, "LOCKED  ", 8, 79);
            }
            else
            {
                if(tmpUnlocked == 1)
                {
                    printLevelSelectGame(0 + SCREENSTARTX, 17 + SCREENSTARTY, "UNLOCKED", 8, 79);
                }
            }
            printNumber(6 + SCREENSTARTX, 16 + SCREENSTARTY, selectedLevel, 2, 61);
        }
        updateSwitches();
        performantdelay(1);
    }
}
