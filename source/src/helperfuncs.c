#include <gbdk/platform.h>

#include "../res/blocktiles.h"

#include "helperfuncs.h"
#include "commonvars.h"

int16_t prevBank;

void initBankStack()
{
    prevBank = 0;
}

void updateSwitches() 
{
	HIDE_WIN;
	SHOW_BKG;
}

void pushBank()
{
    prevBank = _current_bank;
}

void popBank()
{
    SWITCH_ROM(prevBank);
}

void setBlockTilesAsBackground()
{
    pushBank();
    SWITCH_ROM(BANK(blocktiles));
    set_bkg_data(0, 128, blockTiles);
    //Assign 1 & 2 instead A & B for SEGA
    #ifdef SEGA
    set_bkg_data(117,1, blockTiles+(125*16));
    set_bkg_data(119,1, blockTiles+(124*16));
    #endif
    popBank();
}

inline void performantdelay(uint8_t numloops) 
{
    for(i = 0; i != numloops; i++) 
    {
        wait_vbl_done();
    }     
}