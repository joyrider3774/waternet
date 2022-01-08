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

#define tsGameMode 0
#define tsDifficulty 1

#define FADEIN 1
#define FADEOUT 0


#define MAXCURSORFRAMECOUNT 5
#define CURSORANIMCOUNT 4

#define levelCount 25

#define arrowDown 122
#define arrowUp	120
#define arrowLeft 123
#define arrowRight 121
#define leftMenu 96

typedef struct point
{
    uint8_t x;
    uint8_t y;
}point;

#endif