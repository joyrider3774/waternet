#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#define musGame 1
#define musTitle 2
#define musLevelClear 3
#define musAllLevelsClear 4

#define PSG_VOLUME_MAX 0x0
#define PSG_VOLUME_OFF 0xf

#define SFX_SUSTAIN 3

extern uint8_t sound_on;
extern uint8_t music_on;

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
void processSoundSega();
void stopSoundSega();

#endif