#ifndef GRAPHICS_H
#define GRAPHICS_H

#ifdef MASTERSYSTEM
    #define PAL_RED(C)   (((C)     ) & 0x3)
    #define PAL_GREEN(C) (((C) >> 2) & 0x3)
    #define PAL_BLUE(C)  (((C) >> 4) & 0x3)
#endif

#ifdef GAMEGEAR
    #define PAL_RED(C)   (((C)     ) & 0xF)
    #define PAL_GREEN(C) (((C) >> 4) & 0xF)
    #define PAL_BLUE(C)  (((C) >> 8) & 0xF)
#endif

#ifdef NINTENDO
    #define PAL_RED(C)   (((C)      ) & 0x1F)
    #define PAL_GREEN(C) (((C) >>  5) & 0x1F)
    #define PAL_BLUE(C)  (((C) >> 10) & 0x1F)
#endif

#define FADEIN 1
#define FADEOUT 0

void startfade(uint8_t afadein, uint8_t ForTitle);
uint8_t fade();
void setBlackPalette();

#endif