#include <stdint.h>
#include <gbdk/platform.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "printfuncs.h"
#include "helperfuncs.h"

uint8_t c;
unsigned char number[10];

void printDebug(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t length, uint8_t emptyBlock, uint16_t displayTime)
{
#ifdef GAMEBOY 
    palette_color_t pal[4] = {RGB8(255,255,255), RGB8(192,192,192),RGB8(64,64,64), RGB8(0,0,0)};
    int bpal[8];
    unsigned char buf[16*128];
    unsigned char buf2[20*18];
    int b, b2;
    
    //Backup palettes & set default one
    if(_cpu == DMG_TYPE)
    {
        b = BGP_REG;        
        BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    }
    else
    {
        wait_vbl_done();
        b2 = BCPS_REG;
        for(uint8_t u= 0; u <8; u++)
        {
            BCPS_REG = u;        
            bpal[u] = BCPD_REG;
        }
        BCPS_REG = b2;
        wait_vbl_done();
        set_bkg_palette(0,1,pal);  
    }

    //backup background data & tiles
    wait_vbl_done();
    get_bkg_data(0, 128, buf);
    wait_vbl_done();
    get_bkg_tiles(0,0,20,18, buf2);
#endif
    //clear screen
    fill_bkg_rect(0,0,20,18, 61);
    
    //set background tiles
    setBlockTilesAsBackground();
    
    //print debug text
    printLevelSelectGame(ax, ay, amsg, length, emptyBlock);
    
    //Display delay
    for(uint16_t u = displayTime; u != 0; u--)
    {
        wait_vbl_done();
    }
#ifdef GAMEBOY
    //restore background data
    set_bkg_data(0, 128, buf);
    wait_vbl_done();
    set_bkg_tiles(0,0,20,18, buf2);
    
    //restore palletes
    if(_cpu == DMG_TYPE)
    {
        BGP_REG = b;
    }
    else
    {
        wait_vbl_done();
        b2 = BCPS_REG;
        for(uint8_t u= 0; u <8; u++)
        {
            BCPS_REG = u;        
            BCPD_REG = bpal[u];
        }
        BCPS_REG = b2;
    }
#endif    
}

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
            if (amsg[c] == '^')
            {
                set_bkg_tile_xy(ax + c, ay, 126);
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