#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "main.h"

void initSaveState();

uint8_t lastUnlockedLevel(uint8_t mode, uint8_t diff);
uint8_t levelUnlocked(uint8_t mode, uint8_t diff, uint8_t level);
uint8_t isMusicOnSaveState();
uint8_t isSoundOnSaveState();
void unlockLevel(uint8_t mode, uint8_t diff, uint8_t level);
void setMusicOnSaveState(uint8_t value);
void setSoundOnSaveState(uint8_t value);
#endif