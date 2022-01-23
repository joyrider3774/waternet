#ifndef level_h
#define level_h

void moveBlockDown(uint8_t aTile);
void moveBlockUp(uint8_t aTile);
void moveBlockRight(uint8_t aTile);
void moveBlockLeft(uint8_t aTile);
void rotateBlock(uint8_t aTile);
void updateConnected();
void initLevel(uint16_t aRandomSeed);
uint8_t isLevelDone();

#endif