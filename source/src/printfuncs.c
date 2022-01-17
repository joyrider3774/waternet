#include <stdint.h>
#include <gbdk/platform.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "printfuncs.h"

uint8_t c;
unsigned char number[10];

//print a number on levelselect or game screen (used to display levelnr & moves)
//again based on asci codes and adjust to tiles in the tileset
void printNumber(uint8_t ax, uint8_t ay, uint16_t aNumber, uint8_t maxDigits, uint8_t emptyBlock)
{
    uitoa(aNumber, number, 10);
    //sprintf(number,"%u",aNumber);
    for (c=0; c < maxDigits; c++)
    {
        if (number[c] == 0)
        {
            break;
        }
        set_bkg_tile_xy(ax + c, ay, number[c] + 32);
    }
    for( ; c < maxDigits; c++)
    {
         set_bkg_tile_xy(ax + c, ay, emptyBlock);
    }
}

//print a message on the title screen on ax,ay, the tileset from titlescreen contains an alphabet
//and we use the asci codes to adapt the value to the tilenr in the tileset
void printTitle(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t length, uint8_t emptyBlock)
{
    for (c=0; c < length; c++)
    {
        if (amsg[c] == ' ')
        {
            set_bkg_tile_xy(ax + c, ay, emptyBlock);
        }
        else
        {
            if (amsg[c] != 0)
            {
                set_bkg_tile_xy(ax + c, ay, amsg[c] + 34);
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
void printLevelSelectGame(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t length, uint8_t emptyBlock)
{
    for (c=0; c < length; c++)
    {
        if (amsg[c] == ' ')
        {
            set_bkg_tile_xy(ax + c, ay,emptyBlock);
        }
        else
        {
            //A-Z
            if ((amsg[c] > 64) && (amsg[c] < 91))
            {
                set_bkg_tile_xy(ax + c, ay, amsg[c] + 25);
            }   
            else
            {
                if (amsg[c] == '[')
                {
                    set_bkg_tile_xy(ax + c, ay, 70);
                }  
                else
                if (amsg[c] == ']')
                {
                    set_bkg_tile_xy(ax + c, ay, 64);
                }  
                else
                if (amsg[c] == '<')
                {
                    set_bkg_tile_xy(ax + c, ay, 73);
                }  
                else
                if (amsg[c] == '>')
                {
                    set_bkg_tile_xy(ax + c, ay, 67);
                }  
                else
                if (amsg[c] == '+')
                {
                    set_bkg_tile_xy(ax + c, ay, 63);
                }  
                else
                if (amsg[c] == '*')
                {
                    set_bkg_tile_xy(ax + c, ay, 62);
                }  
                else
                if (amsg[c] == '|')
                {
                    set_bkg_tile_xy(ax + c, ay, 69);
                }  
                else
                if (amsg[c] == '#')
                {
                    set_bkg_tile_xy(ax + c, ay, 65);
                }  
                else
                if (amsg[c] == ':')
                {
                    set_bkg_tile_xy(ax + c, ay, 116);
                }  
                else
                {
                    if (amsg[c] == 'a')
                    {
                        set_bkg_tile_xy(ax + c, ay, 119);
                    }
                    else
                    {
                        if (amsg[c] == 'b')
                        {
                            set_bkg_tile_xy(ax + c, ay, 117);
                        } 
                        else
                        {
                            if(amsg[c] == 0)
                            {
                                break;
                            }
                            else
                            {
                                //0-9
                                if ((amsg[c] > 46) && (amsg[c] < 58))
                                {
                                    set_bkg_tile_xy(ax + c, ay, amsg[c] + 32);
                                }
                            } 
                        }
                    }
                }
            }
        }    
    }
}

void printCongratsScreen(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t length, uint8_t emptyBlock)
{
    for (c=0; c < length; c++)
    {
        if (amsg[c] == ' ')
        {
            set_bkg_tile_xy(ax + c, ay,emptyBlock);
        }
        else
        {
            if (amsg[c] != 0)
            {
                set_bkg_tile_xy(ax + c, ay, amsg[c]);
            }
            else
            {
                break;
            }
        }
    }
}