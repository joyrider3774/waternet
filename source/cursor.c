#include <stdint.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include "cursor.h"
#include "tiles/selector.h"

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
    move_sprite(0, 0, 0);
    move_sprite(1, 0, 0);
    move_sprite(2, 0, 0);
    move_sprite(3, 0, 0);
    
    //cursor 1
    move_sprite(4, 0, 0);
    move_sprite(5, 0, 0);
    move_sprite(6, 0, 0);
    move_sprite(7, 0, 0);
    
    //cursor 2
    move_sprite(8, 0, 0);
    move_sprite(9, 0, 0);
    move_sprite(10, 0, 0);
    move_sprite(11, 0, 0);
    
    //cursor 3
    move_sprite(12, 0, 0);
    move_sprite(13, 0, 0);
    move_sprite(14, 0, 0);
    move_sprite(15, 0, 0);
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
    move_sprite((cursorNr<<2) + 0, 8 + ((xPos) << 3), 16 + ((yPos-1) << 3));
    move_sprite((cursorNr<<2) + 1, 8 + ((xPos+1) << 3), 16 + ((yPos) << 3));
    move_sprite((cursorNr<<2) + 2, 8 + ((xPos) << 3), 16 + ((yPos+1) << 3));
    move_sprite((cursorNr<<2) + 3, 8 + ((xPos-1) << 3), 16 + ((yPos) << 3)); 
}

void initCursors()
{
    set_sprite_palette(0, 1, sprPalette);
    set_sprite_data(0, 24, selector);
    
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
