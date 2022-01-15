#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "main.h"

void initSaveState();

uint8_t lastUnlockedLevel(uint8_t mode, uint8_t diff);
uint8_t levelUnlocked(uint8_t mode, uint8_t diff, uint8_t level);
uint8_t isMusicOn();
uint8_t isSoundOn();
void unlockLevel(uint8_t mode, uint8_t diff, uint8_t level);
void setMusicOn(uint8_t value);
void setSoundOn(uint8_t value);
#endif