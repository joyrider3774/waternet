#ifndef helper_funcs_h
#define helper_funcs_h

#include <stdint.h>

void pushBank();
void popBank();
void setBlockTilesAsBackground();
void performantdelay(uint8_t numloops);
void updateSwitches();
void initBankStack();
#endif