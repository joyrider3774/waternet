#include <stdint.h>
#include <gbdk/platform.h>
#include <gb/sgb.h>
#include <string.h>
#include "graphics.h"
#include "commonvars.h"


uint8_t fadein, fadeCounter,fadeStep, startFade, fadeSteps;
palette_color_t  toPallette[4];


uint16_t sprPalette[4] = {
    RGB_LIGHTGRAY, RGB_WHITE, RGB_WHITE, RGB_BLACK
};

const palette_color_t blackPallette[4] = {
	0, 0, 0, 0
};

const palette_color_t bkgPaletteGame[4] = {
    RGB8(128u,128u,128u), RGB8(0,0,128u), RGB8(0,0,255u), RGB8(0,0,64u)
};

const palette_color_t bkgPaletteTitle[4] = {
    RGB8(128u,128u,128u), RGB8(0,0,255u), RGB8(0,0,128u), RGB8(0,0,64u)
};

const palette_color_t bkgSgbPaletteGame[4] = {
    RGB8(128u,128u,128u), RGB8(28u,90u,143u), RGB8(55u,129u,190u), RGB8(11u,47u,79u)
};

const palette_color_t bkgSgbPaletteTitle[4] = {
    RGB8(128u,128u,128u), RGB8(55u,129u,190u), RGB8(28u,90u,143u), RGB8(11u,47u,79u)
};

void clearBackgroundLayer(uint8_t tile)
{
    fill_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, tile);
}

void setBlackPalette()
{
    #ifdef NINTENDO
    if ((_cpu == CGB_TYPE) || sgb_enabled)  
    {
    #endif
        if(sgb_enabled)
        {
            set_sgb_palette(SGB_PAL_01, blackPallette);
        }
        set_bkg_palette(0, 1, blackPallette);
        set_sprite_palette(0, 1, blackPallette);
    #ifdef NINTENDO
    }
    else
    {
        BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
    }
    #endif
}

void set_sgb_palette_colors(uint8_t pal, uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4)
{
    #ifdef NINTENDO
    if(sgb_enabled)
    {
        struct {
            uint8_t cmd;
            uint16_t pal0[4];
            uint16_t pal1[3];
            uint8_t unused;
        } SGB_PAL_PACKET = {
            .cmd = (pal << 3) | 1,
            .pal0 = {c1, c2, c3, c4},
            .pal1 = {0, 0, 0},
            .unused = 0 
        };
        sgb_transfer((void *)&SGB_PAL_PACKET);
    }
    #endif
}

void set_sgb_palette(uint8_t pal, palette_color_t palette[4])
{
    #ifdef NINTENDO
    set_sgb_palette_colors(pal, palette[0], palette[1], palette[2], palette[3]);
    #endif
}

//set the fade flag to fade in or not and increase the fadecounter to startvalue
void startfade(uint8_t afadein, uint8_t ForTitle)
{
    #ifdef NINTENDO
    if((_cpu == CGB_TYPE) || sgb_enabled)
    {
    #endif

        //6 steps on color gameboy to go from default pallete to black and vice versa (goes from 0 to fadesteps)
        #ifdef NINTENDO
        fadeSteps = 5;
        //skips per fadestep to prolong fade effect
        if(_cpu == CGB_TYPE)
        {
            startFade = 7;
        }
        else
        {
            startFade = 3;
        }
        #endif
        #ifdef GAMEGEAR
        fadeSteps = 4;
         //skips per fadestep to prolong fade effect
        startFade = 8;
        #endif
        #ifdef MASTERSYSTEM
        fadeSteps = 2;
         //skips per fadestep to prolong fade effect
        startFade = 10;
        #endif

        //counts from 1 to startfade above then increases fadestep 
        fadeCounter = 1;
        //fade step we are at (will go until fadestep > fadesteps)
        fadeStep = 0;
    #ifdef NINTENDO
    }
    else
    {
        //how long takes each step
        fadeStep = 6;
        //4 steps on dmg gameboy to go from default pallete to black and vice versa
        fadeSteps = 4;
        startFade = fadeSteps * fadeStep;
        fadeCounter = startFade;
    }
    #endif
    
    fadein = afadein;

    uint8_t sgbpal;
    sgbpal = 0;
    #ifdef SEGA
    sgbpal = 1;
    #endif
    sgbpal = sgbpal | sgb_enabled;
    //set temporary palette based on title or gameplay (screen is only faded on 2 places)
    if (ForTitle)
    {
        if (sgbpal)
        {
            memcpy(toPallette, bkgSgbPaletteTitle, sizeof(toPallette));
        }
        else
        {
            memcpy(toPallette, bkgPaletteTitle, sizeof(toPallette));
        }
    }
    else
    {
        if (sgbpal)
        {
            memcpy(toPallette, bkgSgbPaletteGame, sizeof(toPallette));
        }
        else
        {
            memcpy(toPallette, bkgPaletteGame, sizeof(toPallette));
        }
    }
}

//based on zgdb source
palette_color_t fadeBlack(uint8_t step, palette_color_t color) 
{
    return RGB(
        PAL_RED(color) >> (step),
        PAL_GREEN(color) >> (step),
        PAL_BLUE(color) >> (step)
    );
}

//fade to color from black
void fadeInCGB() 
{
    uint8_t c;
    palette_color_t palette[4];

    for(c = 0; c != 4; c++) 
    {
        palette[c] = fadeBlack(fadeSteps - fadeStep, toPallette[c]);
    }
    set_bkg_palette(0, 1, palette);
    if(sgb_enabled)
    {
        set_sgb_palette(SGB_PAL_01, palette);
    }
    
    //first color of sprite pallete (2nd pallete) on sms is used for
    //border colors by default, but cursor only uses last color (black)
    sprPalette[0] = palette[0];
    //selector color
    sprPalette[3] = palette[3];
    set_sprite_palette(0, 1, sprPalette);
}

//fade to black from color
void fadeOutCGB()
{
    uint8_t c;
    palette_color_t palette[4];

    for(c = 0; c != 4; c++) {
        palette[c] = fadeBlack(fadeStep, toPallette[c]);
    }
    set_bkg_palette(0, 1, palette);
    if(sgb_enabled)
    {
        set_sgb_palette(SGB_PAL_01, palette);
    }
    
    //first color of sprite pallete (2nd pallete) on sms is used for
    //border colors by default, but cursor only uses last color (black)
    sprPalette[0] = palette[0];
    //selector color
    sprPalette[3] = palette[3];
    set_sprite_palette(0, 1, sprPalette);
}

//fade function for color gameboy need to set fadecounter first using above function
uint8_t fadecgb()
{
    //fadestep out of range return true
    if (fadeStep > fadeSteps)
    {
        return 1;
    }
    else
    {
         //fade based on what we need todo (in/out)
        if(fadeCounter == 1)
        {
            if(fadein)
            {
                fadeInCGB();
            }
            else
            {
                fadeOutCGB();
            }
        }

        //inc fadecounter 
        fadeCounter++;
        //if fadecounter reaches skip limit 
        if(fadeCounter == startFade)
        {
            //increase fade step & reset fadecounter
            fadeStep++;
            fadeCounter = 1;
        }
        return 0;
    }
}

//simple fade function for DMG gameboy (to BLACK) need to set fadecounter first using above function
uint8_t fadegb()
{
    #ifdef NINTENDO
    if (fadeCounter == 0)
    {
        return 1;
    }
    else
    {
        if(fadeCounter == startFade)
        {
            if (fadein)
            {
                BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
            }
            else
            {    
                BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
            }
        }
        else
        {
            if(fadeCounter == startFade-fadeStep)
            {
                if (fadein)
                {
                    BGP_REG =  DMG_PALETTE(DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_DARK_GRAY, DMG_BLACK);
                }
                else
                {
                    BGP_REG =  DMG_PALETTE(DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK);
                }
            }
            else
            {
                if (fadeCounter == startFade-fadeStep-fadeStep)
                {
                    if (fadein)
                    {
                        BGP_REG = DMG_PALETTE(DMG_LITE_GRAY, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
                    }
                    else
                    {    
                        BGP_REG =  DMG_PALETTE(DMG_DARK_GRAY, DMG_BLACK, DMG_BLACK, DMG_BLACK);
                    }
                }
                else
                {
                    if(fadeCounter == startFade-fadeStep-fadeStep-fadeStep)
                    {
                        if (fadein)
                        {
                            BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
                        }
                        else
                        {    
                            BGP_REG =  DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
                        }
                    }
                }
            }
        }
    }
    fadeCounter--;
    return 0;
    #endif
    #ifdef SEGA
    return 1;
    #endif
}

uint8_t fade()
{
    
    #ifdef NINTENDO
    if((_cpu == CGB_TYPE) || sgb_enabled)
    {
    #endif
        return fadecgb();
    #ifdef NINTENDO
    }
    else
    {
        return fadegb();
    }
    #endif
}
