#include <gbdk/platform.h>

#include "../res/helprotatemap.h"
#include "../res/helprotate2map.h"
#include "../res/helprotateslidemap.h"
#include "../res/helprotateslide2map.h"
#include "../res/helpslidemap.h"
#include "../res/helpslide2map.h"

#include "commonvars.h"
#include "helperfuncs.h"
#include "cursor.h"
#include "graphics.h"
#include "sound.h"

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

void helpSlide2() NONBANKED
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