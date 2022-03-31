#include <gbdk/platform.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "../res/gamebackgroundmap.h"
#include "level.h"
#include "commonvars.h"
#include "helperfuncs.h"
#include "graphics.h"
#include "sound.h"
#include "printfuncs.h"
#include "cursor.h"
#include "savestate.h"

void updateBackgroundGame() NONBANKED
{
    //background
    if(clearbit)
    {
        i16 = 0;
        for (y = 0; y != gameBackgroundMapHeight; y++)
        {
            for (x = 0; x != gameBackgroundMapWidth; x++)
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

                    //if (get_bkg_tile_xy(x,y) != gameBackgroundMap[i16])
                    //{
                        
                        set_bkg_tile_xy(x + SCREENSTARTX, y + SCREENSTARTY, gameBackgroundMap[i16]);                        
                    //}
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
        
        //MOVES:
        printLevelSelectGame(0 + SCREENSTARTX, 17 + SCREENSTARTY, "MOVES:", 6, 61);

        //A:XXXXXX (XXXXXX="ROTATE" or XXXXXX="SLIDE " or XXXXXX="ROSLID")
        if(gameMode == gmRotate)
        {
            printLevelSelectGame(12 + SCREENSTARTX, 17 + SCREENSTARTY, "a:ROTATE", 8, 61);
        }
        else
        {
            if(gameMode == gmSlide)
            {
                printLevelSelectGame(12 + SCREENSTARTX, 17 + SCREENSTARTY, "a:SLIDE ", 8, 61);
            }
            else
            {
                if(gameMode == gmRotateSlide)
                {
                    printLevelSelectGame(12 + SCREENSTARTX, 17 + SCREENSTARTY, "a:ROSLID", 8, 61);
                }
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
        set_bkg_tiles(boardX + SCREENSTARTX, boardY + SCREENSTARTY, boardWidth, boardHeight, level);
        //can put this inside here as well as it most probably changed also when level changed
        printNumber(6 + SCREENSTARTX,17 + SCREENSTARTY, moves, 5, 61);
        
        //clearbit is reset here otherwise too early because of "if" clearbit above
        redrawLevelbit = 0;
        clearbit = 0;
    }
    //level done
    if(redrawLevelDoneBit)
    {
        if( difficulty == diffRandom)
        {
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 2 + SCREENSTARTY, "[************]", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 1 + SCREENSTARTY, "| LEVEL DONE +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1)     + SCREENSTARTY ,"|            +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 1 + SCREENSTARTY ,"| a CONTINUE +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 2 + SCREENSTARTY, "<############>", 14, 61);
        }
        else
        {
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 5 + SCREENSTARTY, "[************]", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 4 + SCREENSTARTY, "| LEVEL DONE +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 3 + SCREENSTARTY ,"|            +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 2 + SCREENSTARTY ,"| PASSWORD:  +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 1 + SCREENSTARTY ,"|            +", 14, 61);
            unsigned char passwordSplit[14];
            memset(passwordSplit, ' ', 14);
            passwordSplit[0] = '|';
            passwordSplit[13] = '+';
            memcpy(passwordSplit+2, statePassword, 8);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1)     + SCREENSTARTY , passwordSplit,   14, 61);
            memcpy(passwordSplit+2, statePassword+8, 8);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 1 + SCREENSTARTY , passwordSplit,   14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 2 + SCREENSTARTY ,"|            +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 3 + SCREENSTARTY ,"| a CONTINUE +", 14, 61);
            printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 4 + SCREENSTARTY, "<############>", 14, 61);
            
        }
        redrawLevelDoneBit = 0;
    }

}

void initGame() NONBANKED
{
    SelectMusic(musGame, 1);
    //set background tiles
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    //set sprite for selector / cursor
    initCursors();
    setCursorPos(0, boardX + selectionX, boardY + selectionY);
    showCursors();

    clearbit = 1;
    redrawLevelbit = 1;
    redrawLevelDoneBit = 0;

}

void doPause(uint8_t isRealPause) NONBANKED
{
    paused = 1;
    wasSoundOn = isSoundOn();
    wasMusicOn = isMusicOn();
    setMusicOn(0);
    setSoundOn(0);
    realPause = isRealPause;
    hideCursors();
    if (realPause)
    {
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 3 + SCREENSTARTY, "[*********]", 11, 61);
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 2 + SCREENSTARTY, "|  PAUSE  +", 11, 61); 
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 1 + SCREENSTARTY, "|         +", 11, 61); 
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + SCREENSTARTY,     "|a PLAY   +", 11, 61);
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 1 + SCREENSTARTY, "|b TO QUIT+", 11, 61);
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 2 + SCREENSTARTY, "<#########>", 11, 61);

    }    
    else
    {
        printLevelSelectGame(((maxBoardBgWidth - 15) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 3 + SCREENSTARTY, "[**************]", 16, 61);
        printLevelSelectGame(((maxBoardBgWidth - 15) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 2 + SCREENSTARTY, "|PLEASE CONFIRM+", 16, 61); 
        printLevelSelectGame(((maxBoardBgWidth - 15) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 1 + SCREENSTARTY, "|              +", 16, 61); 
        printLevelSelectGame(((maxBoardBgWidth - 15) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + SCREENSTARTY,     "|   a PLAY     +", 16, 61);
        printLevelSelectGame(((maxBoardBgWidth - 15) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 1 + SCREENSTARTY, "|   b TO QUIT  +", 16, 61);
        printLevelSelectGame(((maxBoardBgWidth - 15) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 2 + SCREENSTARTY, "<##############>", 16, 61);
    }
    
    
}

void doUnPause() NONBANKED
{
    paused = 0;
    setMusicOn(wasMusicOn);
    setSoundOn(wasSoundOn);
    clearbit = 1;
    setCursorPos(0, boardX + selectionX, boardY + selectionY);
    showCursors();
}

void game() NONBANKED
{
    initGame();
    uint8_t delay;
    delay = 1; 
    while(gameState == gsGame)
    {       
        updateBackgroundGame();
        while(!fade())
        {
            performantdelay(1);
        }

        updateCursorFrame();
        prevJoyPad = joyPad;
        joyPad = joypad();
        if ((joyPad & J_DOWN) && (delay == 0))
        {
            if(!levelDone && !paused)
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
            if (!levelDone && !paused)
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
            if (!levelDone && !paused)
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
            if(!levelDone && !paused)
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
            if(paused)
            {
                doUnPause();
                playMenuAcknowlege();
            }
            else
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

                        //inc level, unlock level and set new password
                        selectedLevel++;
                        unlockLevel(gameMode, difficulty, selectedLevel-1);
                        EncryptState(statePassword);

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
                        randomSeed = clock();
                        initLevel(randomSeed);
                        //redraw level + background 
                        //otherwise with random if level is smaller than previous one the level is not cleared
                        //completely
                        clearbit = 1;  
                        redrawLevelbit = 1;
                        SelectMusic(musGame, 1);
                        //show cursor again (it's actually to early but i'm not fixing that)
                        setCursorPos(0, boardX + selectionX, boardY + selectionY);
                        showCursors();
                    }
                    else
                    {   
                        //goto next level if any
                        if (selectedLevel < maxLevel)
                        {                            
                            initLevel(randomSeed);
                            //redraw levelnr + level + background
                            clearbit = 1;
                            SelectMusic(musGame, 1);
                            //show cursor again (it's actually to early but i'm not fixing that)
                            setCursorPos(0, boardX + selectionX, boardY + selectionY);
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
        }
        //master system has no start button it's mapped to A and it conflicts
        #ifndef MASTERSYSTEM
        if((joyPad & J_START) && (!(prevJoyPad & J_START)))
        {
            if(!levelDone)
            {
                if(!paused)
                {
                    playMenuBackSound();
                    doPause(1);
                }
                else
                {
                    doUnPause();
                    playMenuAcknowlege();
                }
            }
        }
        #endif
        if((joyPad & J_B) && (!(prevJoyPad & J_B))) 
        {
            if(!levelDone)
            {
                if(!paused)
                {
                    playMenuBackSound();
                    doPause(0);
                }
                else
                {
                    //need to enable early again to play backsound
                    //normally unpause does it but we only unpause
                    //after fade
                    setSoundOn(wasSoundOn);
                    hideCursors();
                    playMenuBackSound();
                    gameState = gsLevelSelect;
                    startfade(FADEOUT, 0);
                    while(!fade())
                    {
                        performantdelay(1);
                    }
                    startfade(FADEIN, 0);
                    doUnPause();
                    //unpause sets cursor visible !
                    hideCursors();
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
        }
        updateSwitches();
        performantdelay(1);
        if (delay != 0)
        {
            delay--;
        }
    }
}
