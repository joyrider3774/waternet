#include <gbdk/platform.h>
#include <gbdk/gbdecompress.h>
#include "../res/blocktiles.h"
#include "../res/segabuttontiles.h"

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
    set_bkg_data(0, gb_decompress(blockTiles, tileBuffer) >> 4, tileBuffer);
    popBank();
    //Assign 1 & 2 instead A & B for SEGA
    #ifdef SEGA
    pushBank();
    SWITCH_ROM(BANK(segabuttontiles));
    set_bkg_data(117, gb_decompress(segaButtonTiles, tileBuffer) >> 4, tileBuffer);
    //Assign 1 & 2 instead A & B for SEGA
    popBank();
    #endif

}

inline void performantdelay(uint8_t numloops) 
{
    for(i = 0; i != numloops; i++) 
    {
        wait_vbl_done();
    }     
}