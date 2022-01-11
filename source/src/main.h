#ifndef MAIN_H
#define MAIN_H

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
#define mmCount 2

#define tsMainMenu 0
#define tsGameMode 1
#define tsDifficulty 2

#define levelCount 25

#define arrowDown 122
#define arrowUp	120
#define arrowLeft 123
#define arrowRight 121
#define leftMenu 97

typedef struct point
{
    uint8_t x;
    uint8_t y;
}point;

#endif