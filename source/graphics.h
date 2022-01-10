#ifndef GRAPHICS_H
#define GRAPHICS_H

#define PAL_RED(C)   (((C)      ) & 0x1F)
#define PAL_GREEN(C) (((C) >>  5) & 0x1F)
#define PAL_BLUE(C)  (((C) >> 10) & 0x1F)

#define FADEIN 1
#define FADEOUT 0

void startfade(uint8_t afadein, uint8_t ForTitle);
uint8_t fade();
void setBlackPalette();

#endif