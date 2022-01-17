#ifndef PRINTFUNCS_H
#define PRINTFUNCS_H

void printNumber(uint8_t ax, uint8_t ay, uint16_t aNumber, uint8_t maxDigits, uint8_t emptyBlock);
void printTitle(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t length, uint8_t emptyBlock);
void printLevelSelectGame(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t length, uint8_t emptyBlock);
void printCongratsScreen(uint8_t ax, uint8_t ay, unsigned char amsg[], uint8_t length, uint8_t emptyBlock);

#endif