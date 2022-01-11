#include <gbdk/platform.h>
#include "main.h"

#if defined(NINTENDO)
uint8_t AT(0xA000) header[20];
uint8_t AT(0xA014)LevelLocks[gmCount][diffCount][levelCount];
#elif defined(SEGA)
uint8_t AT(0x8000) header[20];
uint8_t AT(0x8014)LevelLocks[gmCount][diffCount][levelCount];
#endif
