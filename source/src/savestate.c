#include <gbdk/platform.h>
#include <string.h>
#include "commonvars.h"


#if defined(NINTENDO)

#ifdef BATTERYLESSSAVE
#include "../flasher/flasher.h"
#endif

uint8_t AT(0xA000) header[20];
uint8_t AT(0xA014) levelLocks[gmCount][diffCount][levelCount];
uint8_t AT(0xA1D6) musicOn;
uint8_t AT(0xA1D7) soundOn;
#elif defined(SEGA)
uint8_t AT(0x8000) header[20];
uint8_t AT(0x8014) levelLocks[gmCount][diffCount][levelCount];
uint8_t AT(0x81D6) musicOn;
uint8_t AT(0x81D7) soundOn;
#endif


void initSaveState()
{
    #ifndef MEGADUCK
    uint8_t i, j, y;
    ENABLE_RAM;
    //only music option got added and this makes
    //sure that people who had the previous save state / rom
    //don't get their level unlocks overwritten
    //it only initialized the musicOn variable
    if (strcmp(header, "waternet_001") == 0)
    {
        musicOn = 1;
        strcpy(header, "waternet_002");
    }
    
    //another option added
    if (strcmp(header, "waternet_002") == 0)
    {
        soundOn = 1;
        strcpy(header, "waternet_003");
    }

    if (strcmp(header, "waternet_003") != 0)
    {
        for (j=0; j<gmCount; j++)
        {
            for (i=0; i<diffCount; i++)
            {
                for (y = 0; y<levelCount; y++)
                {
                    levelLocks[j][i][y] = 0;
                }
                //unlock first level
                levelLocks[j][i][0] = 1; 
            }
        }
        musicOn = 1;
        soundOn = 1;
        strcpy(header, "waternet_003");
    }
    
#ifdef BATTERYLESSSAVE
    save_sram();
#endif
    DISABLE_RAM;
    #endif
}

void setMusicOnSaveState(uint8_t value)
{
    value; //avoid megaduck warning
    #ifndef MEGADUCK
    ENABLE_RAM;
        musicOn = value;
#ifdef BATTERYLESSSAVE
    save_sram();
#endif
    DISABLE_RAM;
    #endif
}

uint8_t isMusicOnSaveState()
{
    uint8_t tmp;
    tmp = 1;

    #ifndef MEGADUCK
    ENABLE_RAM;
        tmp = musicOn;
    DISABLE_RAM;
    #endif
    return tmp;
}

void setSoundOnSaveState(uint8_t value)
{
    value; //avoid megaduck warning
    #ifndef MEGADUCK
    ENABLE_RAM;
        soundOn = value;
#ifdef BATTERYLESSSAVE
    save_sram();
#endif
    DISABLE_RAM;
    #endif
}

uint8_t isSoundOnSaveState()
{
    uint8_t tmp;
    tmp = 1;

    #ifndef MEGADUCK
    ENABLE_RAM;
        tmp = soundOn;
    DISABLE_RAM;
    #endif
    return tmp;
}

void unlockLevel(uint8_t mode, uint8_t diff, uint8_t level)
{
    mode; diff; level; //avoid megaduck warning
    #ifndef MEGADUCK
    ENABLE_RAM;
    levelLocks[mode][diff][level] = 1;
#ifdef BATTERYLESSSAVE
    save_sram();
#endif
    DISABLE_RAM;
    #endif
}

uint8_t levelUnlocked(uint8_t mode, uint8_t diff, uint8_t level)
{
    mode; diff; level; //avoid megaduck warning
    uint8_t tmp;
    tmp = 1;
    #ifndef MEGADUCK
    ENABLE_RAM;
    tmp = levelLocks[mode][diff][level];
    DISABLE_RAM;
    #endif

    return (tmp == 1);
}

uint8_t lastUnlockedLevel(uint8_t mode, uint8_t diff)
{
    mode; diff; //avoid megaduck warning
    uint8_t tmp;
    tmp = 1;
    #ifndef MEGADUCK
    uint8_t level;
    ENABLE_RAM;
    for (level = 0; level < levelCount; level++)
    {
        if(levelLocks[mode][diff][level] == 1)
        {
            tmp = level + 1;
        }
        else
        {
            break;
        }
    }
    DISABLE_RAM;
    #endif

    return tmp;
}