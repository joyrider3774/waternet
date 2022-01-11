#include <gbdk/platform.h>
#include <string.h>
#include "main.h"


#if defined(NINTENDO)
uint8_t AT(0xA000) header[20];
uint8_t AT(0xA014) levelLocks[gmCount][diffCount][levelCount];
#elif defined(SEGA)
uint8_t AT(0x8000) header[20];
uint8_t AT(0x8014) levelLocks[gmCount][diffCount][levelCount];
#endif


void initSaveState()
{
    uint8_t i, j, y;
    #ifndef MEGADUCK
    if (strcmp(header, "waternet_001") != 0)
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
        strcpy(header, "waternet_001");
    }
    #endif
}

void unlockLevel(uint8_t mode, uint8_t diff, uint8_t level)
{
    #ifndef MEGADUCK
    ENABLE_RAM;
    levelLocks[mode][diff][level] = 1;
    DISABLE_RAM;
    #endif
}

uint8_t levelUnlocked(uint8_t mode, uint8_t diff, uint8_t level)
{
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
    uint8_t tmp, level;
    tmp = 1;
   
    #ifndef MEGADUCK
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