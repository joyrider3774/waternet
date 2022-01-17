#include <stdint.h>
#include <stdio.h>
#include <gbdk/platform.h>
#include <rand.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "sound.h"
#include "cursor.h"
#include "graphics.h"
#include "printfuncs.h"

#include "savestate.h"

#include "../res/gamebackgroundmap.h"
#include "../res/titlescreenmap.h"
#include "../res/congratsscreenmap.h"
#include "../res/helprotatemap.h"
#include "../res/helprotate2map.h"
#include "../res/helprotateslidemap.h"
#include "../res/helprotateslide2map.h"
#include "../res/helpslidemap.h"
#include "../res/helpslide2map.h"

#include "../res/blocktiles.h"
#include "../res/titletiles.h"
#include "../res/congratsscreentiles.h"

uint8_t startPos, menuPos, 
        maxLevel, selectedLevel, boardX, boardY, joyPad, difficulty, 
        gameState, boardWidth, boardHeight, boardSize, prevBoardWidth, prevBoardHeight,
        i, j, x, y, rnd, index, cc, maxcc, clearbit, redrawLevelbit, levelDone,
        prevJoyPad, titleStep, gameMode, posAdd, redrawLevelDoneBit,
        tmp, tmp2, tmp3, tmp4, tmp5, tmp6, neighboursFound, selectedNeighbour, currentPoint, visitedRooms,  mainMenu,
        option, paused, wasMusicOn, wasSoundOn, realPause;
        
int16_t selectionX, selectionY,i16;
uint16_t rnd16, randomSeed, moves;
uint8_t neighbours[4];
uint8_t cellStack[maxBoardSize+1];
point lookUpTable[maxBoardSize];
unsigned char level[maxBoardSize];


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
            tmp = i+x;
            lookUpTable[tmp].x = x;
            lookUpTable[tmp].y = y;
        }
        i += width;
    }
    //remember current board size
    prevBoardWidth = width;
    prevBoardHeight = height;
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

void handleConnectPoint(uint8_t currentPoint)
{
    if ((lookUpTable[currentPoint].y > 0) && (!(level[currentPoint] & 1)))
        {
            tmp = currentPoint - boardWidth;
            if (((level[tmp] < 16) && (!(level[tmp] & 4)) ) || 
            ((level[tmp] > 15) && (!((level[tmp] - 16) & 4))))
            {
                //adapt tile to filled tile
                if(level[currentPoint] < 16)
                { 
                    level[currentPoint] += 16;
                }

                //add neighbour to cellstack of to handle tiles
                if (level[tmp] < 16)
                {
                    cellStack[cc++] = tmp;
                }
            }       
            
        }
        //if tile has passage to the east and east neigbour passage to the west 
        if  ((lookUpTable[currentPoint].x  + 1 < boardWidth) && (!(level[currentPoint] & 2)))
        {
            tmp = currentPoint + 1;
            if (((level[tmp] < 16) && (!(level[tmp] & 8))) || 
            ((level[tmp] > 15) && (!((level[tmp] - 16) & 8))))
            {
                //adapt tile to filled tile
                if(level[currentPoint] < 16)
                { 
                    level[currentPoint] += 16;
                }

                //add neighbour to cellstack of to handle tiles
                if (level[tmp] < 16)
                {
                    cellStack[cc++] = tmp;
                }

            }
        }      
                
        //if tile has passage to the south and south neigbour passage to the north 
        if ((lookUpTable[currentPoint].y + 1 < boardHeight) && (!(level[currentPoint] & 4 )))
        {
            tmp = currentPoint + boardWidth;
            if (((level[tmp] < 16) && (!(level[tmp] & 1))) || 
            ((level[tmp] > 15) && (!((level[tmp] - 16) & 1))))
            {
                //adapt tile to filled tile
                if(level[currentPoint] < 16)
                { 
                    level[currentPoint] += 16;
                }

                //add neighbour to cellstack of to handle tiles
                if (level[tmp] < 16)
                {
                    cellStack[cc++] = tmp; 
                }
            } 
        }

        //if tile has passage to the west and west neigbour passage to the east 
        if  ((lookUpTable[currentPoint].x > 0) && (!(level[currentPoint] & 8)))
        {
            tmp = currentPoint - 1;
            if (((level[tmp] < 16) && (!(level[tmp] & 2))) ||
            ((level[tmp] > 15) && (!((level[tmp] - 16) & 2))))
            {
                //adapt tile to filled tile
                if(level[currentPoint] < 16)
                { 
                    level[currentPoint] += 16;
                }

                //add neighbour to cellstack of to handle tiles
                if(level[tmp] < 16)
                {
                    cellStack[cc++] = tmp;
                }
            }  
        }

}

void updateConnected()
{
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
    handleConnectPoint(startPos);
    while(--cc > 0)
    {
        //if tile is bigger then 15 we already handled this one, continue with next one
        if ((level[cellStack[cc]] < 16))
        {
            handleConnectPoint(cellStack[cc]);
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
        tmp = currentPoint+1; 
        //tile has neighbour to the right which we did not handle yet
        if ((level[tmp] == 0xfu) && (lookUpTable[currentPoint].x + 1 < boardWidth))
        {
            neighbours[neighboursFound++] = tmp;
        }
    
        tmp = currentPoint-1; 
        //tile has neighbour to the left which we did not handle yet
        if ((level[tmp] == 0xfu) && (lookUpTable[currentPoint].x > 0))
        {
            neighbours[neighboursFound++] = tmp;
        }

        tmp = currentPoint - boardWidth; 
        //tile has neighbour the north which we did not handle yet
        if ((level[tmp] == 0xfu) && (lookUpTable[currentPoint].y > 0))
        {
            neighbours[neighboursFound++] = tmp;
        }

        tmp = currentPoint + boardWidth; 
        //tile has neighbour the south which we did not handle yet
        if ((level[tmp] == 0xfu) && (lookUpTable[currentPoint].y + 1 < boardHeight))
        {
            neighbours[neighboursFound++] = tmp;
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
            tmp = lookUpTable[selectedNeighbour].x;
            tmp3 = lookUpTable[currentPoint].x;
            //tile has neighbour to the east
            if(tmp > tmp3)
            {
                //remove west wall neighbour
                level[selectedNeighbour] &= ~(8);
                //remove east wall tile
                level[currentPoint] &= ~(2);
            }
            else // tile has neighbour to the west
            {
                if(tmp < tmp3)
                {
                    //remove east wall neighbour
                    level[selectedNeighbour] &= ~(2);
                    //remove west wall tile
                    level[currentPoint] &= ~(8);
                }
                else // tile has neighbour to the north
                {
                    tmp2 = lookUpTable[selectedNeighbour].y;
                    tmp4 = lookUpTable[currentPoint].y;
                    if(tmp2 < tmp4)
                    {
                        //remove south wall neighbour
                        level[selectedNeighbour] &= ~(4);
                        //remove north wall tile
                        level[currentPoint] &= ~(1);
                    }
                    else // tile has neighbour to the south
                    {
                        if(tmp2 > tmp4)
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
                cellStack[cc++] = currentPoint;
            } 
            //set tile to the neighbour   
            currentPoint = selectedNeighbour;
            visitedRooms++;
        }
        else //no neighbours take a tile from the list
        {
            currentPoint = cellStack[--cc];
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

    //level is currently the solution so we still need to shuffle it
    shuffleLevel();
    //update possibly connected tiles already starting from startpoint
    updateConnected();
}

void setBlockTilesAsBackground()
{
    set_bkg_data(0, 128, blockTiles);
    //Assign 1 & 2 instead A & B for SEGA
    #ifdef SEGA
    set_bkg_data(117,1, blockTiles+(125*16));
    set_bkg_data(119,1, blockTiles+(124*16));
    #endif
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
    uint8_t delay, tmpUnlocked;
    delay = 1;
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
        if ((joyPad & J_LEFT) && (delay == 0))
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
            delay = 6;
        }
        if ((joyPad & J_RIGHT) && (delay == 0))
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
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 2 + SCREENSTARTY, "[************]", 14, 61);
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) - 1 + SCREENSTARTY, "| LEVEL DONE +", 14, 61);
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1)     + SCREENSTARTY ,"|            +", 14, 61);
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 1 + SCREENSTARTY ,"| a CONTINUE +", 14, 61);
        printLevelSelectGame(((maxBoardBgWidth - 12) >> 1) + SCREENSTARTX, (maxBoardBgHeight >> 1) + 2 + SCREENSTARTY, "<############>", 14, 61);
        redrawLevelDoneBit = 0;
    }

}

void initGame()
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

void doPause(uint8_t isRealPause)
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

void doUnPause()
{
    paused = 0;
    setMusicOn(wasMusicOn);
    setSoundOn(wasSoundOn);
    clearbit = 1;
    setCursorPos(0, boardX + selectionX, boardY + selectionY);
    showCursors();
}

void game()
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
                            selectedLevel++;
                            unlockLevel(gameMode, difficulty, selectedLevel-1);
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

void updateBackgroundTitleScreen()
{
    //if background needs clearing draw background and menu
    if(clearbit)
    {
        set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, titlescreenMapWidth, titlescreenMapHeight, titlescreenMap);
        
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

void initTitleScreen()
{
    set_bkg_data(0, 126, titleTiles);
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

void initLevelsCleared()
{
    set_bkg_data(0, 91, congratsScreenTiles);
    clearBackgroundLayer(0);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, congratsMapWidth, congratsMapHeight, congratsMap);
    if(difficulty == diffVeryEasy)
    {
        printCongratsScreen(1 + SCREENSTARTX, 7 + SCREENSTARTY, "VERY EASY  LEVELS", 17, 0);
    }
    if(difficulty == diffEasy)
    {
        printCongratsScreen(4 + SCREENSTARTX, 7 + SCREENSTARTY, "EASY LEVELS",11, 0);
    }
    if(difficulty == diffNormal)
    {
        printCongratsScreen(3 + SCREENSTARTX, 7 + SCREENSTARTY, "NORMAL LEVELS",13, 0);
    }
    if(difficulty == diffHard)
    {
        printCongratsScreen(4 + SCREENSTARTX, 7 + SCREENSTARTY, "HARD LEVELS",11, 0);
    }
    if(difficulty == diffVeryHard)
    {
        printCongratsScreen(1 + SCREENSTARTX, 7 + SCREENSTARTY, "VERY HARD  LEVELS",17, 0);
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
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, helpRotateSlideMapWidth, helpRotateSlideMapHeight, helpRotateSlideMap);
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
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, helpRotateSlide2MapWidth, helpRotateSlide2MapHeight, helpRotateSlide2Map);
    SelectMusic(musTitle, 1);

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
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, helpRotateMapWidth, helpRotateMapHeight, helpRotateMap);
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
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, helpRotate2MapWidth, helpRotate2MapHeight, helpRotate2Map);
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
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, helpSlideMapWidth, helpSlideMapHeight, helpSlideMap);
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
    setBlockTilesAsBackground();
    clearBackgroundLayer(48);
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, helpSlide2MapWidth, helpSlide2MapHeight, helpSlide2Map);
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
    #ifdef NINTENDO
    if(_cpu == CGB_TYPE)
    {
        cpu_fast();
    }
    #endif
    setBlackPalette();
    DISPLAY_ON;
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
    setMusicOn(isMusicOnSaveState());
    setSoundOn(isSoundOnSaveState());
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