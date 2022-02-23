#include "commonvars.h"

#include <stdint.h>

uint8_t startPos, menuPos, 
        maxLevel, selectedLevel, boardX, boardY, joyPad, difficulty, 
        gameState, boardWidth, boardHeight, boardSize, prevBoardWidth, prevBoardHeight,
        i, j, x, y, rnd, index, cc, maxcc, clearbit, redrawLevelbit, levelDone,
        prevJoyPad, titleStep, gameMode, posAdd, redrawLevelDoneBit,
        tmp, tmp2, tmp3, tmp4, tmp5, tmp6, neighboursFound, selectedNeighbour, currentPoint, visitedRooms,  mainMenu,
        option, paused, wasMusicOn, wasSoundOn, realPause, lookUpX, lookUpY, savedBank, sgb_enabled;
        
int16_t selectionX, selectionY, i16;
uint16_t randomSeed, moves;
uint8_t neighbours[4];
uint8_t cellStack[maxBoardSize+1];
point lookUpTable[maxBoardSize];
unsigned char level[maxBoardSize];
uint8_t tileBuffer[4096];