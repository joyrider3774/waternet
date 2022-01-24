#include <gbdk/platform.h>
#include <stdint.h>

#include "commonvars.h"
#include "printfuncs.h"
#include "graphics.h"
#include "helperfuncs.h"
#include "sound.h"
#include "level.h"

#include "../res/congratsscreentiles.h"
#include "../res/congratsscreenmap.h"

BANKREF(LEVELSCLEARED)

void initLevelsCleared() NONBANKED
{
    pushBank();
    SWITCH_ROM(BANK(congratsscreentiles));
    set_bkg_data(0, 91, congratsScreenTiles);
    popBank();
    clearBackgroundLayer(0);
    pushBank();
    SWITCH_ROM(BANK(congratsscreenmap));
    set_bkg_tiles(SCREENSTARTX, SCREENSTARTY, congratsMapWidth, congratsMapHeight, congratsMap);
    popBank();
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

void levelsCleared() BANKED
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