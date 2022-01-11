#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#define musGame 1
#define musTitle 2
#define musLevelClear 3
#define musAllLevelsClear 4

void initSound();

void SelectMusic(uint8_t musicFile, uint8_t loop);
void playNote();
void musicTimer();
void initMusic();

void playGameAction();
void playMenuAcknowlege();
void playMenuBackSound();
void playMenuSelectSound();
void playErrorSound();
void playGameMoveSound();

#endif