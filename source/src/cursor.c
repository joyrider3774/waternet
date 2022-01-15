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
        set_sprite_tile(4, 25 + tmpFrame);
        set_sprite_tile(5, 26 + tmpFrame);
        set_sprite_tile(6, 27 + tmpFrame);
        set_sprite_tile(7, 28 + tmpFrame);
        
        //cursor 1
        set_sprite_tile(8, 8 + tmpFrame);
        set_sprite_tile(9, 9 + tmpFrame);
        set_sprite_tile(10, 10 + tmpFrame);
        set_sprite_tile(11, 11 + tmpFrame);
        set_sprite_tile(12, 25 + tmpFrame);
        set_sprite_tile(13, 26 + tmpFrame);
        set_sprite_tile(14, 27 + tmpFrame);
        set_sprite_tile(15, 28 + tmpFrame);
       
        //cursor 2
        set_sprite_tile(16, 8 + tmpFrame);
        set_sprite_tile(17, 9 + tmpFrame);
        set_sprite_tile(18, 10 + tmpFrame);
        set_sprite_tile(19, 11 + tmpFrame);
        set_sprite_tile(20, 25 + tmpFrame);
        set_sprite_tile(21, 26 + tmpFrame);
        set_sprite_tile(22, 27 + tmpFrame);
        set_sprite_tile(23, 28 + tmpFrame);
       
        //cursor 3
        set_sprite_tile(24, 8 + tmpFrame);
        set_sprite_tile(25, 9 + tmpFrame);
        set_sprite_tile(26, 10 + tmpFrame);
        set_sprite_tile(27, 11 + tmpFrame);
        set_sprite_tile(28, 25 + tmpFrame);
        set_sprite_tile(29, 26 + tmpFrame);
        set_sprite_tile(30, 27 + tmpFrame);
        set_sprite_tile(31, 28 + tmpFrame);
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
    move_sprite((cursorNr<<3) + 0, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos - 1 + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<3) + 1, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<3) + 2, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + 1 + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<3) + 3, DEVICE_SPRITE_PX_OFFSET_X + ((xPos - 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + SCREENSTARTY) << 3)); 
    //corners
    move_sprite((cursorNr<<3) + 4, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos - 1 + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<3) + 5, DEVICE_SPRITE_PX_OFFSET_X + ((xPos + 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + 1 + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<3) + 6, DEVICE_SPRITE_PX_OFFSET_X + ((xPos - 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos - 1 + SCREENSTARTY) << 3));
    move_sprite((cursorNr<<3) + 7, DEVICE_SPRITE_PX_OFFSET_X + ((xPos - 1 + SCREENSTARTX) << 3), DEVICE_SPRITE_PX_OFFSET_Y + ((yPos + 1 + SCREENSTARTY) << 3)); 
}

void initCursors()
{
    set_sprite_palette(0, 1, sprPalette);
    set_sprite_data(0, 41, selectorTiles);
    
    //cursor 0
    set_sprite_tile(0, 8);
    set_sprite_tile(1, 9);
    set_sprite_tile(2, 10);
    set_sprite_tile(3, 11);
    set_sprite_tile(4, 25);
    set_sprite_tile(5, 26);
    set_sprite_tile(6, 27);
    set_sprite_tile(7, 28);

    //cursor 1
    set_sprite_tile(8, 8);
    set_sprite_tile(9, 9);
    set_sprite_tile(10, 10);
    set_sprite_tile(11, 11);
    set_sprite_tile(12, 25);
    set_sprite_tile(13, 26);
    set_sprite_tile(14, 27);
    set_sprite_tile(15, 28);

    //cursor 2
    set_sprite_tile(16, 8);
    set_sprite_tile(17, 9);
    set_sprite_tile(18, 10);
    set_sprite_tile(19, 11);
    set_sprite_tile(20, 25);
    set_sprite_tile(21, 26);
    set_sprite_tile(22, 27);
    set_sprite_tile(23, 28);

    //cursor 3
    set_sprite_tile(24, 8);
    set_sprite_tile(25, 9);
    set_sprite_tile(26, 10);
    set_sprite_tile(27, 11);
    set_sprite_tile(28, 25);
    set_sprite_tile(29, 26);
    set_sprite_tile(30, 27);
    set_sprite_tile(31, 28);

    cursorFrameCount = 0;
    cursorFrame = 0;
}
