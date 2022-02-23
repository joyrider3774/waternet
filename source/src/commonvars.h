#ifndef commonvars_h
#define commonvars_h

#include <stdint.h>

#define maxBoardWidth 18
#define maxBoardHeight 14
#define maxBoardBgWidth 20
#define maxBoardBgHeight 16
#define maxBoardSize 252 //w*h

#define gsGame 0
#define gsTitle 1
#define gsLevelSelect 2
#define gsLevelsCleared 3
#define gsHelpRotate 4
#define gsHelpRotate2 5
#define gsHelpRotateSlide 6
#define gsHelpRotateSlide2 7
#define gsHelpSlide 8
#define gsHelpSlide2 9

#define diffVeryEasy 0
#define diffEasy 1
#define diffNormal 2
#define diffHard 3
#define diffVeryHard 4
#define diffRandom 5
#define diffCount 6

#define gmRotate 0
#define gmSlide 1
#define gmRotateSlide 2
#define gmCount 3

#define mmStartGame 0
#define mmHelp 1
#define mmOptions 2
#define mmCount 3

#define opMusic 0
#define opSound 1
#define opCount 2

#define tsMainMenu 0
#define tsGameMode 1
#define tsDifficulty 2
#define tsOptions 3

#define levelCount 25

#define arrowDown 122
#define arrowUp	120
#define arrowLeft 123
#define arrowRight 121
#define leftMenu 97

//only SMS has different screen res 
//it is used to center the screen
#ifdef MASTERSYSTEM
#define SCREENSTARTX 6
#define SCREENSTARTY 3
#else
#define SCREENSTARTX 0
#define SCREENSTARTY 0
#endif

typedef struct point
{
    uint8_t x;
    uint8_t y;
}point;


extern uint8_t startPos, menuPos, 
        maxLevel, selectedLevel, boardX, boardY, joyPad, difficulty, 
        gameState, boardWidth, boardHeight, boardSize, prevBoardWidth, prevBoardHeight,
        i, j, x, y, rnd, index, cc, maxcc, clearbit, redrawLevelbit, levelDone,
        prevJoyPad, titleStep, gameMode, posAdd, redrawLevelDoneBit,
        tmp, tmp2, tmp3, tmp4, tmp5, tmp6, neighboursFound, selectedNeighbour, currentPoint, visitedRooms,  mainMenu,
        option, paused, wasMusicOn, wasSoundOn, realPause, lookUpX, lookUpY, savedBank, sgb_enabled;
        
extern int16_t selectionX, selectionY, i16;
extern uint16_t randomSeed, moves;
extern uint8_t neighbours[4];
extern uint8_t cellStack[maxBoardSize+1];
extern point lookUpTable[maxBoardSize];
extern unsigned char level[maxBoardSize];
extern uint8_t tileBuffer[4096];
#endif