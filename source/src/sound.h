#ifndef SOUND_H
#define SOUND_H

#include <gbdk/platform.h>
#include <stdint.h>


#define musGame 1
#define musTitle 2
#define musLevelClear 3
#define musAllLevelsClear 4

#define PSG_VOLUME_MAX 0x0
#define PSG_VOLUME_OFF 0xf

#define SFX_SUSTAIN 3

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
void setMusicOn(uint8_t value);
void setSoundOn(uint8_t value);
uint8_t isMusicOn();
uint8_t isSoundOn();

#ifdef NINTENDO
void playNintendoCh1(uint8_t REG_10, uint8_t REG_11, uint8_t REG_12, uint8_t REG_13, uint8_t REG_14);
void playNintendoCh2(uint8_t REG_21, uint8_t REG_22, uint8_t REG_23, uint8_t REG_24);
#endif

#ifdef SEGA
void playSegaSfxTone(int val); 
void playSegaSfxNoise(int val);
void playSegaSfxToneCritical(int val); 
void playSegaSfxNoiseCritical(int val);
void playSegaMusTone(int val);
void SegaMusVolumeOff();
void SegaSfxVolumeOff();
void SegaNoiseVolumeOff();
void SegaNoiseVolumeMax();
void SegaSfxVolumeMax();
void SegaMusVolumeMax();
void SegaStopSound();
#endif

#endif