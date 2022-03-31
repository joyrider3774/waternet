#include <gbdk/platform.h>
#include <string.h>
#include <rand.h>
#include "commonvars.h"
#include "savestate.h"

uint8_t levelLocks[gmCount][diffCount][levelCount];
uint8_t valXorA, val1, val2, val3, val4, val5, val6, val7,
 val8, val9, val10, val11, val12, val13, val14, val15, valRandom, valCheckSum;
const unsigned char key[15] = {3,1,5,8,9,4,15,4,5,7,9,11,4,13,6};

void initSaveState()
{
  unlockLevels(gmRotate, diffVeryEasy, 1);
  unlockLevels(gmRotate, diffEasy, 1);
  unlockLevels(gmRotate, diffNormal, 1);
  unlockLevels(gmRotate, diffHard, 1);
  unlockLevels(gmRotate, diffVeryHard, 1);

  unlockLevels(gmSlide, diffVeryEasy, 1);
  unlockLevels(gmSlide, diffEasy, 1);
  unlockLevels(gmSlide, diffNormal, 1);
  unlockLevels(gmSlide, diffHard, 1);
  unlockLevels(gmSlide, diffVeryHard, 1);

  unlockLevels(gmRotateSlide, diffVeryEasy, 1);
  unlockLevels(gmRotateSlide, diffEasy, 1);
  unlockLevels(gmRotateSlide, diffNormal, 1); 
  unlockLevels(gmRotateSlide, diffHard, 1);;
  unlockLevels(gmRotateSlide, diffVeryHard, 1);
}

uint8_t DecryptState(unsigned char *tmpbuf)
{
    val1 = tmpbuf[0] - 65;
    val2 = tmpbuf[1] - 68;
    val3 = tmpbuf[2] - 65;
    val4 = tmpbuf[3] - 74;
    val5 = tmpbuf[4] - 72;
    val6 = tmpbuf[5] - 65;
    val7 = tmpbuf[6] - 67;
    val8 = tmpbuf[7] - 71;
    val9 = tmpbuf[8] - 68;
    val10 = tmpbuf[9] - 65;
    val11 = tmpbuf[10] - 73;
    val12 = tmpbuf[11] - 66;
    val13 = tmpbuf[12] - 65;
    val14 = tmpbuf[13] - 75;
    val15 = tmpbuf[14] - 70;
    valCheckSum = tmpbuf[15] - 65;

    if (valCheckSum > 15) 
        return 0;

    uint8_t tmp = 12u ^ val1 ^ val2 ^ val3 ^ val4 ^ val5 ^ val6 ^ val7 ^ val8 ^ val9 ^ 
        val10 ^ val11 ^ val12 ^ val13 ^ val14 ^ val15;
    if (tmp != valCheckSum)
        return 0;

    val15 = val15 ^ val14;
    val14 = val14 ^ val13;
    val13 = val13 ^ val12;
    val12 = val12 ^ val11;
    val11 = val11 ^ val10;
    val10 = val10 ^ val9;
    val9 = val9 ^ val8;
    val8 = val8 ^ val7;
    val7 = val7 ^ val6;
    val6 = val6 ^ val5;
    val5 = val5 ^ val4;
    val4 = val4 ^ val3;
    val3 = val3 ^ val2;
    val2 = val2 ^ val1;
    val1 = val1 ^ val15;

    val15 = (val15 ^ val14) ^ key[1];
    val14 = (val14 ^ val13) ^ key[2];
    val13 = (val13 ^ val12) ^ key[3];
    val12 = (val12 ^ val11) ^ key[4];
    val11 = (val11 ^ val10) ^ key[5];
    val10 = (val10 ^ val9) ^ key[6];
    val9 = (val9 ^ val8) ^ key[7];
    val8 = (val8 ^ val7) ^ key[8];
    val7 = (val7 ^ val6) ^ key[9];
    val6 = (val6 ^ val5) ^ key[10];
    val5 = (val5 ^ val4) ^ key[11];
    val4 = (val4 ^ val3) ^ key[12];
    val3 = (val3 ^ val2) ^ key[13];
    val2 = (val2 ^ val1) ^ key[14];
    val1 = (val1 ^ key[0]);

    //cant be higher than 15 levels
    if ((val1 > 15) || (val2 > 15) || (val3 > 15) || (val4 > 15) ||  (val5 > 15) ||
      (val6 > 15) || (val7 > 15) || (val8 > 15) || (val9 > 15) ||
      (val10 > 15) || (val11 > 15) || (val12 > 15) || (val3 > 15) ||
      (val14 > 15) || (val15 > 15))
      return 0;

    //and can't be 0 as 1st level is always unlocked
    if ((val1 == 0) || (val2 == 0) || (val3 == 0) || (val4 == 0) || (val5 == 0) ||
      (val6 == 0) || (val7 == 0) || (val8 == 0) || (val9 == 0) ||
      (val10 == 0) || (val11 == 0) || (val12 == 0) || (val3 == 0) ||
      (val14 == 0) || (val15 == 0))
      return 0;
        

    unlockLevels(gmRotate, diffVeryEasy, val1);
    unlockLevels(gmRotate, diffEasy, val2);
    unlockLevels(gmRotate, diffNormal, val3);
    unlockLevels(gmRotate, diffHard, val4);
    unlockLevels(gmRotate, diffVeryHard, val5);

    unlockLevels(gmSlide, diffVeryEasy, val6);
    unlockLevels(gmSlide, diffEasy, val7);
    unlockLevels(gmSlide, diffNormal, val8);
    unlockLevels(gmSlide, diffHard, val9);
    unlockLevels(gmSlide, diffVeryHard, val10);

    unlockLevels(gmRotateSlide, diffVeryEasy, val11);
    unlockLevels(gmRotateSlide, diffEasy, val12);
    unlockLevels(gmRotateSlide, diffNormal, val13);
    unlockLevels(gmRotateSlide, diffHard, val14);
    unlockLevels(gmRotateSlide, diffVeryHard, val15);

    return 1;
}

void EncryptState(unsigned char *tmpbuf)
{
    // i just want always same passwords
    val1 = lastUnlockedLevel(gmRotate, diffVeryEasy);
    val2 = lastUnlockedLevel(gmRotate, diffEasy);
    val3 = lastUnlockedLevel(gmRotate, diffNormal);
    val4 = lastUnlockedLevel(gmRotate, diffHard);
    val5 = lastUnlockedLevel(gmRotate, diffVeryHard);

    val6 = lastUnlockedLevel(gmSlide, diffVeryEasy);
    val7 = lastUnlockedLevel(gmSlide, diffEasy);
    val8 = lastUnlockedLevel(gmSlide, diffNormal);
    val9 = lastUnlockedLevel(gmSlide, diffHard);
    val10 = lastUnlockedLevel(gmSlide, diffVeryHard);

    val11 = lastUnlockedLevel(gmRotateSlide, diffVeryEasy);
    val12 = lastUnlockedLevel(gmRotateSlide, diffEasy);
    val13 = lastUnlockedLevel(gmRotateSlide, diffNormal);
    val14 = lastUnlockedLevel(gmRotateSlide, diffHard);
    val15 = lastUnlockedLevel(gmRotateSlide, diffVeryHard);

    val1 = (val1 ^ key[0]);
    val2 = (val2 ^ val1) ^ key[14];
    val3 = (val3 ^ val2) ^ key[13];
    val4 = (val4 ^ val3) ^ key[12];
    val5 = (val5 ^ val4) ^ key[11];

    val6 = (val6 ^ val5) ^ key[10];
    val7 = (val7 ^ val6) ^ key[9];
    val8 = (val8 ^ val7) ^ key[8];
    val9 = (val9 ^ val8) ^ key[7];
    val10 = (val10 ^ val9) ^ key[6];

    val11 = (val11 ^ val10) ^ key[5];
    val12 = (val12 ^ val11) ^ key[4];
    val13 = (val13 ^ val12) ^ key[3];
    val14 = (val14 ^ val13) ^ key[2];
    val15 = (val15 ^ val14) ^ key[1];

    val1 = val1 ^ val15;
    val2 = val2 ^ val1;
    val3 = val3 ^ val2;
    val4 = val4 ^ val3;
    val5 = val5 ^ val4;
    val6 = val6 ^ val5;
    val7 = val7 ^ val6;
    val8 = val8 ^ val7;
    val9 = val9 ^ val8;
    val10 = val10 ^ val9;
    val11 = val11 ^ val10;
    val12 = val12 ^ val11;
    val13 = val13 ^ val12;
    val14 = val14 ^ val13;
    val15 = val15 ^ val14;
    
    valCheckSum = 12u ^ val1 ^ val2 ^ val3 ^ val4 ^ val5 ^ val6 ^ val7 ^ val8 ^ val9 ^ val10 ^ val11 ^ val12 ^ val13 ^ val14 ^ val15;

    tmpbuf[0] = val1 + 65;
    tmpbuf[1] = val2 + 68; 
    tmpbuf[2] = val3 + 65;
    tmpbuf[3] = val4 + 74;
    tmpbuf[4] = val5 + 72;
    tmpbuf[5] = val6 + 65;
    tmpbuf[6] = val7 + 67;
    tmpbuf[7] = val8 + 71;
    tmpbuf[8] = val9 + 68;
    tmpbuf[9] = val10 + 65;
    tmpbuf[10] = val11 + 73;
    tmpbuf[11] = val12 + 66;
    tmpbuf[12] = val13 + 65;
    tmpbuf[13] = val14 + 75;
    tmpbuf[14] = val15 + 70; 
    tmpbuf[15] = valCheckSum + 65;
}

void unlockLevels(uint8_t mode, uint8_t diff, uint8_t nrOfLevels)
{
    uint8_t level;
    for (level = 0; level < levelCount; level++)
    {
        levelLocks[mode][diff][level] = level < nrOfLevels;
    }
}

void unlockLevel(uint8_t mode, uint8_t diff, uint8_t level)
{
    levelLocks[mode][diff][level] = 1;
}

uint8_t levelUnlocked(uint8_t mode, uint8_t diff, uint8_t level)
{
    return (levelLocks[mode][diff][level] == 1);
}

uint8_t lastUnlockedLevel(uint8_t mode, uint8_t diff)
{
    uint8_t tmp, level;
    tmp = 1;
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
    return tmp;
}