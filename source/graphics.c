#include <stdint.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <string.h>
#include "graphics.h"

uint8_t fadein, fadeCounter,fadeStep, startFade, fadeSteps;
uint16_t  toPallette[4];

const uint16_t blackPallette[4] = {
	RGB_BLACK, RGB_BLACK, RGB_BLACK, RGB_BLACK
};

const uint16_t bkgPaletteGame[4] = {
	RGB_LIGHTGRAY, RGB(0,0,15), RGB(0,0,31), RGB(0,0,7)
};

const uint16_t bkgPaletteTitle[4] = {
	RGB_LIGHTGRAY, RGB(0,0,31), RGB(0,0,15), RGB(0,0,7)
};

void setBlackPalette()
{
    if (_cpu == CGB_TYPE) 
    {
        cpu_fast();
        set_bkg_palette(0, 1, blackPallette);
    }
    else
    {
        BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
    }
}

//set the fade flag to fade in or not and increase the fadecounter to startvalue
void startfade(uint8_t afadein, uint8_t ForTitle)
{
    if(_cpu == CGB_TYPE)
    {
        //6 steps on color gameboy to go from default pallete to black and vice versa (goes from 0 to fadesteps)
        fadeSteps = 5;
        //skips per fadestep to prolong fade effect
        startFade = 7;
        //counts from 1 to startfade above then increases fadestep 
        fadeCounter = 0;
        //fade step we are at (will go until fadestep > fadesteps)
        fadeStep = 0;

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
    
    fadein = afadein;

    //set temporary palette based on title or gameplay (screen is only faded on 2 places)
    if (ForTitle)
    {
        memcpy(toPallette, bkgPaletteTitle, sizeof(toPallette));   
    }
    else
    {
        memcpy(toPallette, bkgPaletteGame, sizeof(toPallette));
    }
}

//based on zgdb source
uint16_t fadeBlack(uint8_t step, uint16_t color) 
{
    return RGB(
        PAL_RED(color) >> step,
        PAL_GREEN(color) >> step,
        PAL_BLUE(color) >> step
    );
}

//fade to color from black
void fadeInCGB() 
{
    uint8_t c;
    uint16_t palette[4];

    for(c = 0; c != 4; c++) 
    {
        palette[c] = fadeBlack(fadeSteps - fadeStep, toPallette[c]);
    }
    set_bkg_palette(0, 1, palette);
}

//fade to black from color
void fadeOutCGB()
{
    uint8_t c;
    uint16_t palette[4];

    for(c = 0; c != 4; c++) {
        palette[c] = fadeBlack(fadeStep, toPallette[c]);
    }
    set_bkg_palette(0, 1, palette);
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
        if(fadein)
        {
            fadeInCGB();
        }
        else
        {
            fadeOutCGB();
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
}

uint8_t fade()
{
    if(_cpu == CGB_TYPE)
    {
        return fadecgb();
    }
    else
    {
        return fadegb();
    }
}
