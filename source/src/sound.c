#include <stdint.h>
#include <string.h>
#include <gbdk/platform.h>
#include "sound.h"


//I (joyrider3774) created the music in this tool : https://onlinesequencer.net
//and used this page to convert notes to frequencies for gameboy: http://www.devrs.com/gb/files/sndtab.html
//music uses channel 2 only using simple notes
//sound uses channel 1 only

//MUSIC PLAYING CONCEPT based on
//      Sheep it up! - A tiny GB/GBC game for the bitbitjam4 game jam
//      by Dr. Ludos (http://www.ludoscience.com)

uint8_t music_note, music_tempo, music_length, music_loop, prev_music, music_sustain_note, music_on, sound_on, sfx_sustain;
volatile uint8_t sfx_setting_note, sfx_sustain_zero_set, music_sustain_zero_set, selecting_music;
uint8_t musicArray[255];

//Here is kind of "quick'n dirty" custom music player made using solely the Channel 2 of the GB
//The main parameters of the channel are set manually, then this table defines "notes", as in "frequencies" to play that will go to the NR23 & NR24 registers
//Here is the "note table". Each note is defined by 4 hexa numbers, directly setting the values for NR21, NR22, NR23 and NR24 registers, in that order
const uint8_t music_notes_gb[44] ={
    0x82, 0x00, 0x00, 0x00, //for silence
    0x82, 0x77, 0x16, 0x84, //C  4
    0x82, 0x77, 0x50, 0x84, //C# 4
    0x82, 0x77, 0x83, 0x84, //D  4
    0x82, 0x77, 0xB5, 0x84, //D# 4
    0x82, 0x77, 0xE5, 0x84, //E  4
    0x82, 0x77, 0x2D, 0x87, //D# 6
    0x82, 0x77, 0x22, 0x87, //D 6
    0x82, 0x77, 0x39, 0x87, //E 6
    0x82, 0x77, 0x44, 0x87, //F 6
    0x82, 0x77, 0x15, 0x85, //E 4
};

const uint16_t music_notes_gg [11] ={
    0,  //for silence
    428, //C  4
    403, //C# 4
    381, //D  4
    359, //D# 4
    340, //E  4
    89, //D# 6
    95, //D 6
    84, //E 6
    80, //F 6
    320, //E 4
};

//helper defines
#define P   0
#define C4  1
#define C4S 2 
#define D4  3
#define D4S 4
#define E4  5
#define D6S 6
#define D6  7
#define E6  8
#define F6  9
#define F4  10
#define PAUSE 15
#define PAUSE2 6
#define PAUSE3 14
#define PAUSE4 13

//Here is the "partition" table. Each note is defined by two numbers in this array
//- The first number is the "note", an index to the "notes" tables array / 4 (as each note is defined by 4 entry in notes, we can use note 0,1,2,3... instead of note 0,4,8,12,16 - it's easier to read for us mere humans)
//- The second number is the "delay" after the next note : the player will wait XX cycles (max: 240 - 4 seconds (60 cycles per seconds) and play the next "note"
//Music Table Format: Note index in "music_notes_gb" / 4, Delay before next note

//https://onlinesequencer.net/2498607
const uint8_t music_levelsCleared[96] ={
    D6,PAUSE4,
    D6S,PAUSE4,
    D6,PAUSE4,
    P,PAUSE4,
    D6,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4,
    E6,PAUSE4,
    E6,PAUSE4,
    E6,PAUSE4,
    D6,PAUSE4,
    P,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    P,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4,
    E6,PAUSE4,
    P,PAUSE4,
    D6,PAUSE4,
    D6S,PAUSE4,
    D6,PAUSE4,
    E6,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    D6S,PAUSE4,
    D6,PAUSE4,
    D6,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    P,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    D6S,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    P,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4,
    D6S,PAUSE4,
    E6,PAUSE4,
    P,PAUSE4,
    F6,PAUSE4,
    E6,PAUSE4,
    F6,PAUSE4
};


//https://onlinesequencer.net/2484974
const uint8_t music_won[16] ={
  D6S, PAUSE2,
  D6,  PAUSE2,
  D6S, PAUSE2,
  E6 , PAUSE2,
  D6S, PAUSE2,
  E6 , PAUSE2,
  F6 , PAUSE2 << 2,
  P, PAUSE2  
};

//https://onlinesequencer.net/2485064
const uint8_t music_game[198] ={ 
    P, PAUSE3,
    C4, PAUSE3,
    C4S, PAUSE3,
    P, PAUSE3,
    C4, PAUSE3,
    C4S, PAUSE3,
    C4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    E4, PAUSE3,
    F4, PAUSE3,
    P, PAUSE3,

    C4, PAUSE3,
    C4S, PAUSE3,
    P, PAUSE3,
    C4, PAUSE3,
    C4S, PAUSE3,
    C4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    D4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    P, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    E4, PAUSE3,
    F4, PAUSE3,
    P, PAUSE3,

    D4, PAUSE3,
    C4, PAUSE3,
    E4, PAUSE3,
    D4, PAUSE3,
    P,  PAUSE3,
    E4, PAUSE3,
    P,  PAUSE3,
    E4, PAUSE3,
    D4, PAUSE3,
    P,  PAUSE3,
    D4S, PAUSE3,
    D4S, PAUSE3,
    E4, PAUSE3,
    P, PAUSE3,
    E4, PAUSE3,
    D4S, PAUSE3,
    D4, PAUSE3,
    C4, PAUSE3,
    D4S, PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    P,PAUSE3,
    D4S,PAUSE3,
    D4,PAUSE3,
    C4,PAUSE3,
    P,PAUSE3,
    D4,PAUSE3,
    C4,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    P,PAUSE3,
    E4,PAUSE3,
    P,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    P,PAUSE3,
    D4S,PAUSE3,
    D4S,PAUSE3,
    E4,PAUSE3,
    P,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    C4,PAUSE3,
    C4S,PAUSE3,
    D4S,PAUSE3,
    F4,PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    D4S,PAUSE3,
    D4S,PAUSE3,
    P, PAUSE3,
    E4,PAUSE3,
    D4,PAUSE3,
    D4,PAUSE3,
    P, PAUSE3,
    D4S,PAUSE3,
    C4S,PAUSE3,
    C4S,PAUSE3,
    P, PAUSE3,
    D4,PAUSE3
};

//https://onlinesequencer.net/2484977
const uint8_t music_intro[98] = {
    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    P,   PAUSE * 8,
    
    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    D4,  PAUSE,
    C4,  PAUSE,
    C4S, PAUSE,
    P,   PAUSE * 4,

    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    P,   PAUSE * 8,
    
    C4,  PAUSE,
    C4,  PAUSE * 2,
    D4,  PAUSE,
    D4S, PAUSE,
    C4,  PAUSE,
    D4,  PAUSE,
    C4,  PAUSE,
    C4S, PAUSE,
    P,   PAUSE * 4,

    D4,  PAUSE,
    D4,  PAUSE, 
    C4S, PAUSE,
    C4,  PAUSE,
    D4S, PAUSE,
    C4S, PAUSE,
    D4,  PAUSE,
    D4S, PAUSE,
    C4S, PAUSE,
    D4S, PAUSE,
    D4,  PAUSE,
    D4,  PAUSE,
    D4,  PAUSE,
    E4,  PAUSE,
    C4,  PAUSE,
    C4S, PAUSE,
    E4,  PAUSE,
    D4,  PAUSE * 3,
    P,   PAUSE * 4
};

void setMusicOn(uint8_t value)
{
    CRITICAL {
        music_on = value;
        #ifdef SEGA
        if (music_on)
        {
            playSegaMusTone(0);
            SegaMusVolumeMax();
        }
        else
        {
            SegaMusVolumeOff();
        }
        #endif
    }
}

void setSoundOn(uint8_t value)
{
    CRITICAL {
        sound_on = value;
        #ifdef SEGA
        if (sound_on)
        {
            playSegaSfxTone(0);
            SegaSfxVolumeMax();
        }
        else
        {
            SegaSfxVolumeOff();
        }
        #endif
    }
}

uint8_t isMusicOn()
{
    return music_on;
}

uint8_t isSoundOn()
{
    return sound_on;
}

void initSound()
{
    #ifdef NINTENDO
    // these registers must be in this specific order!
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels
    #else
    CRITICAL {
        sfx_sustain_zero_set = 1;
        sfx_sustain = 0;
        sfx_setting_note = 0;
    }
    #endif
}

#ifdef NINTENDO
void playNintendoCh1(int REG_10, int REG_11, int REG_12, int REG_13, int REG_14)
{
    NR10_REG = REG_10;
    NR11_REG = REG_11;
    NR12_REG = REG_12;
    NR13_REG = REG_13;
    NR14_REG = REG_14;
}

void playNintendoCh2(int REG_21, int REG_22, int REG_23, int REG_24)
{
    NR21_REG = REG_21;
    NR22_REG = REG_22;
    NR23_REG = REG_23;
    NR24_REG = REG_24;
}
#endif

#ifdef SEGA

void playSegaSfxToneCritical(int val)
{
    CRITICAL {
        playSegaSfxTone(val);   
    }
}

void playSegaSfxTone(int val) 
{
    sfx_setting_note = 1;
    PSG = PSG_LATCH | PSG_CH0 | val;
    PSG = val;
    //0 freqency don't reset sfx_sustain_zero_set and sfx_sustain
    //otherwise we'll get and infinite loop when its called from
    //ProcessSegaSound
    if (val != 0)
    {
        sfx_sustain_zero_set = 0;
        sfx_sustain = SFX_SUSTAIN;
    }
    sfx_setting_note = 0;
}


void playSegaSfxNoiseCritical(int val)
{
    CRITICAL {
        playSegaSfxNoise(val);
    }
} 

void playSegaSfxNoise(int val) 
{
    sfx_setting_note = 1;
    sfx_sustain_zero_set = 0;
    PSG = PSG_LATCH | PSG_CH3 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH3 | val;
    sfx_sustain = SFX_SUSTAIN * 3;
    sfx_setting_note = 0;
}

void playSegaMusTone(int val) 
{
    PSG = PSG_LATCH | PSG_CH1 | ((val) & 0xf);
    PSG = (((val) >> 4) & 0x3f);
    if (val != 0)
    {
        music_sustain_zero_set = 0;
    }
}

void SegaMusVolumeOff()
{
    PSG = PSG_LATCH | PSG_CH1 | PSG_VOLUME | PSG_VOLUME_OFF;
}

void SegaMusVolumeMax()
{
    PSG = PSG_LATCH | PSG_CH1 | PSG_VOLUME | PSG_VOLUME_MAX;
}

void SegaSfxVolumeOff()
{
    sfx_setting_note = 1;
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_OFF;
    sfx_setting_note = 0;
}

void SegaSfxVolumeMax()
{
    sfx_setting_note = 1;
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
    sfx_setting_note = 0;
}


void SegaNoiseVolumeOff()
{
    sfx_setting_note = 1;
    PSG = PSG_LATCH | PSG_CH3 | PSG_VOLUME | PSG_VOLUME_OFF;
    sfx_setting_note = 0;
}

void SegaNoiseVolumeMax()
{
    sfx_setting_note = 1;
    PSG = PSG_LATCH | PSG_CH3 | PSG_VOLUME | PSG_VOLUME_MAX;
    sfx_setting_note = 0;
}

void SegaStopSound()
{
    SegaMusVolumeOff();
    SegaSfxVolumeOff();
    SegaNoiseVolumeOff();
}

void processSoundSega()
{
    if (sfx_setting_note || selecting_music)
    {
        return;
    }
    
    musicTimer();
    if(sfx_sustain == 0)
    {
        if (!sfx_sustain_zero_set)
        {
            playSegaSfxTone(0);
            SegaNoiseVolumeOff();
            sfx_sustain_zero_set = 1;
        }
    }
    else
    {
        sfx_sustain--;
    }
}

#endif


void SelectMusic(uint8_t musicFile, uint8_t loop)
{
    if (prev_music != musicFile)
    {
        selecting_music = 1;
        CRITICAL {            
            prev_music = musicFile;
            memset(musicArray, 0, sizeof(musicArray));
            switch (musicFile) 
            {
                case musTitle:
                    memcpy(musicArray, music_intro, sizeof(music_intro));
                    music_length = sizeof(music_intro);
                    break;
                case musLevelClear:
                    memcpy(musicArray, music_won, sizeof(music_won));
                    music_length = sizeof(music_won);
                    break;
                case musAllLevelsClear:
                    memcpy(musicArray, music_levelsCleared, sizeof(music_levelsCleared));
                    music_length = sizeof(music_levelsCleared);
                    break;
                case musGame:
                    memcpy(musicArray, music_game, sizeof(music_game));
                    music_length = sizeof(music_game);
                    break;
            }
            music_note = 0;
            music_tempo = 0;
            music_loop = loop;
            music_sustain_zero_set = 1;
            music_sustain_note = 0;
        }
        selecting_music = 0;
    }
}

void playNote()
{    
    if(music_note < music_length)
    {
        //Play current note ( << 2 is like a *4 (bitshift is faster than * for the GB), because music note is 0,1,2,3,4... while our actual note array uses 4 entry per note, 
        //so we have to multiply note by 4 to get the actual registers index for each note)
        #ifdef NINTENDO
        playNintendoCh2(music_notes_gb[(musicArray[music_note] << 2)],
            music_notes_gb[(musicArray[music_note] << 2) + 1],
            music_notes_gb[(musicArray[music_note] << 2) + 2],
            music_notes_gb[(musicArray[music_note] << 2) + 3]);
        music_sustain_note = 0;
        #else
        playSegaMusTone(music_notes_gg[musicArray[music_note]]);
        music_sustain_note = 10;
        #endif

        //Set the new delay to wait
        music_tempo = musicArray[music_note + 1];

        //Skip to the next note
        music_note += 2;
               
        if (music_note > music_length - 1)
        {
            if(music_loop)
            {
                music_note = 0;
            }
            else
            {
                #ifdef SEGA
                playSegaMusTone(0);
                #endif
            }
        }
    }
}


void musicTimer()
{
    //for nintendo systems as sega one checks it earlier
    if (selecting_music)
    {
        return;
    }

    //Play some music
    if (music_tempo == 0)
    {
        if(music_on)
        {
            playNote();
        }
    }
    //Else wait for the next note to play
    else 
    {
        music_tempo--;
        if(music_sustain_note == 0)
        {
            if (!music_sustain_zero_set)
            {
                #ifdef SEGA
                playSegaMusTone(0);
                #endif
                music_sustain_zero_set = 1;
            }
        }
        else
        {
            music_sustain_note--;
        }
    }
}

void initMusic()
{
    
    CRITICAL {
        music_sustain_note = 0;
        prev_music = 0;
        music_note = 0;
        music_length = 0;
        music_tempo = 0;
        music_loop = 0;
        //set to 1 so nothing plays until a music was selected
        selecting_music = 1;
        music_sustain_zero_set = 1;
        #ifdef NINTENDO
        add_TIM(musicTimer);
        #else
        add_VBL(processSoundSega);
        #endif
    }
    #ifdef NINTENDO
    if (_cpu == CGB_TYPE)
    {
        TMA_REG = 0x80;
        TAC_REG = 0x4U;
    }
    else
    {
        TMA_REG = 0xC0;
        TAC_REG = 0x4U;    
    }
    set_interrupts(TIM_IFLAG | VBL_IFLAG); 
    #else
    set_interrupts(VBL_IFLAG); 
    #endif
}

void playGameMoveSound()
{
    if(!sound_on)
    {
        return;
    }
    #ifdef NINTENDO
    playNintendoCh1(0x00, 0x40, 0x63, 0x06, 0x87);
    #else
    playSegaSfxToneCritical(0x6);
    #endif
}

void playErrorSound()
{
    if(!sound_on)
    {
        return;
    }
    #ifdef NINTENDO
    playNintendoCh1(0x19, 0xC2, 0x93, 0x73, 0xC6);
    #else
    playSegaSfxNoiseCritical(0x2);
    #endif
}

void playMenuSelectSound()
{
    if(!sound_on)
    {
        return;
    }
    #ifdef NINTENDO
    playNintendoCh1(0x00, 0x00, 0x63, 0x9E, 0x87);
    #else
    playSegaSfxToneCritical(0x3);
    #endif
}

void playMenuBackSound()
{
    if(!sound_on)
    {
        return;
    }
    #ifdef NINTENDO
    playNintendoCh1(0x00, 0x81, 0x63, 0xE8, 0x83);
    #else
    playSegaSfxToneCritical(0xF);
    #endif
}

void playMenuAcknowlege()
{
    if(!sound_on)
    {
        return;
    }
    #ifdef NINTENDO
    playNintendoCh1(0x57, 0x03, 0x5e, 0xd0, 0x87);
    #else
    playSegaSfxToneCritical(0x7);
    #endif
}

void playGameAction()
{
    if(!sound_on)
    {
        return;
    }
    #ifdef NINTENDO
    playNintendoCh1(0x00, 0x80, 0x63, 0x6f, 0x86);
    #else
    playSegaSfxToneCritical(0x9);
    #endif
}
