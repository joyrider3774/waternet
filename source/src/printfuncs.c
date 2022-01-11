#include <stdint.h>
#include <gbdk/platform.h>
#include <string.h>
#include <stdio.h>

#include "printfuncs.h"

uint8_t c;
unsigned char number[10];
unsigned char titleMessage[20];

//print a number on levelselect or game screen (used to display levelnr & moves)
//again based on asci codes and adjust to tiles in the tileset
void printNumber(uint8_t ax, uint8_t ay, uint16_t aNumber, uint8_t maxDigits, uint8_t emptyBlock)
{
    memset(number, 0, 10);
    sprintf(number, "%d", aNumber);
    for (c=0; c < maxDigits; c++)
    {
        if (number[c] != 0)
        {
            set_bkg_tile_xy(ax + c, ay, number[c] + 32);
        }
        else
        {
            set_bkg_tile_xy(ax + c, ay, emptyBlock);
        }      
    }
}

//print a message on the title screen on ax,ay, the tileset from titlescreen contains an alphabet
//and we use the asci codes to adapt the value to the tilenr in the tileset
void printTitle(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t emptyBlock)
{
    memset(titleMessage, 0, 20);
    sprintf(titleMessage, "%s", amsg);
    for (c=0; c < 20; c++)
    {
        if (titleMessage[c] == ' ')
        {
            set_bkg_tile_xy(ax + c, ay, emptyBlock);
        }
        else
        {
            if (titleMessage[c] != 0)
            {
                set_bkg_tile_xy(ax + c, ay, titleMessage[c] + 34);
            }
            else
            {
                break;
            }
        }    
    }
}

//print a message on the title screen on ax,ay, the tileset from titlescreen contains an alphabet
//and we use the asci codes to adapt the value to the tilenr in the tileset
void printLevelSelectGame(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t emptyBlock)
{
    memset(titleMessage, 0, 20);
    sprintf(titleMessage, "%s", amsg);
    for (c=0; c < 20; c++)
    {
        if (titleMessage[c] == ' ')
        {
            set_bkg_tile_xy(ax + c, ay,emptyBlock);
        }
        else
        {
            //A-Z
            if ((titleMessage[c] > 64) && (titleMessage[c] < 91))
            {
                set_bkg_tile_xy(ax + c, ay, titleMessage[c] + 25);
            }   
            else
            {
                if (titleMessage[c] == ':')
                {
                    set_bkg_tile_xy(ax + c, ay, 116);
                }  
                else
                {
                    if (titleMessage[c] == 'a')
                    {
                        set_bkg_tile_xy(ax + c, ay, 119);
                    }
                    else
                    {
                        if (titleMessage[c] == 'b')
                        {
                            set_bkg_tile_xy(ax + c, ay, 117);
                        } 
                        else
                        {
                            if(titleMessage[c] == 0)
                            {
                                break;
                            }
                            else
                            {
                                //0-9
                                if ((titleMessage[c] > 46) && (titleMessage[c] < 58))
                                {
                                    set_bkg_tile_xy(ax + c, ay, titleMessage[c] + 32);
                                }
                            } 
                        }
                    }
                }
            }
        }    
    }
}

void printCongratsScreen(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t emptyBlock)
{
    memset(titleMessage, 0, 20);
    sprintf(titleMessage, "%s", amsg);
    for (c=0; c < 20; c++)
    {
        if (titleMessage[c] == ' ')
        {
            set_bkg_tile_xy(ax + c, ay,emptyBlock);
        }
        else
        {
            if (titleMessage[c] != 0)
            {
                set_bkg_tile_xy(ax + c, ay, titleMessage[c]);
            }
            else
            {
                break;
            }
        }
    }
}