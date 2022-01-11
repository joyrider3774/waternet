#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "main.h"

void initSaveState();

uint8_t lastUnlockedLevel(uint8_t mode, uint8_t diff);
uint8_t levelUnlocked(uint8_t mode, uint8_t diff, uint8_t level);
void unlockLevel(uint8_t mode, uint8_t diff, uint8_t level);

#endif