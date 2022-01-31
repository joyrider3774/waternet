#include <gbdk/platform.h>

#include <stdint.h>
#include <string.h>
#include "flasher.h"
#include "../src/printfuncs.h"

extern const uint8_t save_rom_bank;

void restore_sram_bank(uint8_t bank) NONBANKED {
    uint8_t _save = _current_bank;
    SWITCH_RAM(bank);
    SWITCH_ROM(save_rom_bank + (bank >> 1));
    memcpy((uint8_t *)0xA000, (uint8_t *)(0x4000 + ((bank & 1) << 13)), 0x2000);
    SWITCH_ROM(_save);
}

void restore_sram() {
    for (uint8_t i = 0; i < 1; i++)
        restore_sram_bank(i);
}

extern uint8_t erase_flash() OLDCALL;                 // erases FLASH sector: 64K or 4 banks
extern uint8_t save_sram_banks(uint8_t count) OLDCALL;  // copies up to count SRAM banks to FLASH

uint8_t save_sram() {
    if (!erase_flash()) return 0;
    uint8_t t = save_sram_banks(1);

#ifdef DEBUGFLASHSAVE
    if(t)
    {
        printDebug(0,0,"FLASH SAVE OK",13,61, 60*2);
    }
    else
    {
        printDebug(0,0,"FLASH SAVE NOT OK",17,61,60*2);
    }
#endif
    return t;
}