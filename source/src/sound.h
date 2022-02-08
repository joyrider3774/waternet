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
void playNintendoCh1(int REG_10, int REG_11, int REG_12, int REG_13, int REG_14);
void playNintendoCh2(int REG_21, int REG_22, int REG_23, int REG_24);
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