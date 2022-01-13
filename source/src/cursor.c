#include <stdint.h>
#include <gbdk/platform.h>
#include "cursor.h"
#include "main.h"
#include "../res/selectortiles.h"

uint8_t cursorFrameCount, cursorFrame;

const uint16_t sprPalette[4] = {
	RGB_LIGHTGRAY, RGB_WHITE, RGB_WHITE, RGB_BLACK
};

void updateCursorFrame()
{
    uint8_t tmpFrame;
    cursorFrameCount++;
    if (cursorFrameCount >= MAXCURSORFRAMECOUNT)
    {
        cursorFrame++;
        cursorFrameCount = 0;
        if (cursorFrame >= CURSORANIMCOUNT)
        {
            cursorFrame = 0;
        }
        tmpFrame = (cursorFrame * 4);
       
        //cursor 0
        set_sprite_tile(0, 8 + tmpFrame);
        set_sprite_tile(1, 9 + tmpFrame);
        set_sprite_tile(2, 10 + tmpFrame);
        set_sprite_tile(3, 11 + tmpFrame);
        
        //cursor 1
        set_sprite_tile(4, 8 + tmpFrame);
        set_sprite_tile(5, 9 + tmpFrame);
        set_sprite_tile(6, 10 + tmpFrame);
        set_sprite_tile(7, 11 + tmpFrame);
       
        //cursor 2
        set_sprite_tile(8, 8 + tmpFrame);
        set_sprite_tile(9, 9 + tmpFrame);
        set_sprite_tile(10, 10 + tmpFrame);
        set_sprite_tile(11, 11 + tmpFrame);
       
        //cursor 3
        set_sprite_tile(12, 8 + tmpFrame);
        set_sprite_tile(13, 9 + tmpFrame);
        set_sprite_tile(14, 10 + tmpFrame);
        set_sprite_tile(15, 11 + tmpFrame);
    }
}

void hideCursors()
{
    //HIDE CURSOR SPRITES
    //cursor 0
    setCursorPos(0, 0, DEVICE_SCREEN_HEIGHT+1);
    
    //cursor 1
    setCursorPos(1, 0, DEVICE_SCREEN_HEIGHT+1);
    
    //cursor 2
    setCursorPos(2, 0, DEVICE_SCREEN_HEIGHT+1);
    
    //cursor 3
    setCursorPos(3, 0, DEVICE_SCREEN_HEIGHT+1);
    HIDE_SPRITES;
}

void showCursors()
{
    SHOW_SPRITES;
}

void setCursorPos(uint8_t cursorNr, uint8_t xPos, uint8_t yPos)
{
    if (cursorNr > 3)
    {
        return;
    }
    move_sprite((cursorNr<<2) + 0, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos - 1 + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<2) + 1, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<2) + 2, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + 1 + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<2) + 3, DEVICE_SPRITE_PX_OFFSET_X + ((xPos - 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + SCREENSTARTY) << 3)); 
}

void initCursors()
{
    set_sprite_palette(0, 1, sprPalette);
    set_sprite_data(0, 24, selectorTiles);
    
    //cursor 0
    set_sprite_tile(0, 8);
    set_sprite_tile(1, 9);
    set_sprite_tile(2, 10);
    set_sprite_tile(3, 11);

    //cursor 1
    set_sprite_tile(4, 8);
    set_sprite_tile(5, 9);
    set_sprite_tile(6, 10);
    set_sprite_tile(7, 11);

    //cursor 2
    set_sprite_tile(8, 8);
    set_sprite_tile(9, 9);
    set_sprite_tile(10, 10);
    set_sprite_tile(11, 11);

    //cursor 3
    set_sprite_tile(12, 8);
    set_sprite_tile(13, 9);
    set_sprite_tile(14, 10);
    set_sprite_tile(15, 11);
    
    cursorFrameCount = 0;
    cursorFrame = 0;
}
