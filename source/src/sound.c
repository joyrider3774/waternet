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
//		by Dr. Ludos (http://www.ludoscience.com)

uint8_t music_note, music_tempo, music_length, music_loop, prev_music, sustain_note, sfx_sustain;
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
const uint8_t music_intro[98] ={
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


void initSound()
{
    #ifdef NINTENDO
    // these registers must be in this specific order!
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels
    #endif
}

void SelectMusic(uint8_t musicFile, uint8_t loop)
{
    if (prev_music != musicFile)
    {
        prev_music = musicFile;
        CRITICAL {
            music_note = 0;
            music_tempo = 0;
            music_loop = loop;
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
        }
    }
}

void playNote()
{    
    if(music_note < music_length)
    {
        //Play current note ( << 2 is like a *4 (bitshift is faster than * for the GB), because music note is 0,1,2,3,4... while our actual note array uses 4 entry per note, 
        //so we have to multiply note by 4 to get the actual registers index for each note)
        #ifdef NINTENDO
        NR21_REG = music_notes_gb[ musicArray[music_note]<<2 ];
        NR22_REG = music_notes_gb[ (musicArray[music_note]<<2)+1 ];
        NR23_REG = music_notes_gb[ (musicArray[music_note]<<2)+2 ];
        NR24_REG = music_notes_gb[ (musicArray[music_note]<<2)+3 ];
        sustain_note = 0;
        #else 
        PSG = PSG_LATCH | PSG_CH1 | PSG_VOLUME | PSG_VOLUME_MAX;
        PSG = PSG_LATCH | PSG_CH1 |  (music_notes_gg[ musicArray[music_note] ]) & 0xf;
	    PSG = ((music_notes_gg[ musicArray[music_note] ]) >> 4) & 0x3f;
        sustain_note = 10;
        #endif
        //Set the new delay to wait
        music_tempo = musicArray[ music_note+1 ];

        //Skip to the next note
        music_note += 2;
               
        if (music_note > music_length-1 )
        {
            if(music_loop)
            {
                music_note = 0;
            }
            else
            {
                #ifdef SEGA
                PSG = PSG_LATCH | PSG_CH1 | PSG_VOLUME | PSG_VOLUME_OFF;
                #endif
            }
        }
    }

}


void musicTimer()
{
    //Play some music
    if( music_tempo == 0 )
    {
        playNote();     
    }
    //Else wait for the next note to play
    else 
    {
        music_tempo--;
        if(sustain_note == 0)
        {
            #ifdef SEGA
            PSG = PSG_LATCH | PSG_CH1 | PSG_VOLUME | PSG_VOLUME_MAX;
            PSG = PSG_LATCH | PSG_CH1 |  0;
	        PSG = 0;
            #endif    
        }
        else
        {
            sustain_note--;
        }
    }
}

void initMusic()
{
    
    CRITICAL {
        #ifdef NINTENDO
        add_TIM(musicTimer);
        #else
        add_VBL(processSoundSega);
        #endif
        sfx_sustain = 0;
        sustain_note = 0;
        prev_music = 0;
        music_note = 1;
        music_length = 1;
        music_tempo = 0;
        music_loop = 0;
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
    #endif
    set_interrupts(TIM_IFLAG | VBL_IFLAG); 
}

void stopSoundSega()
{
    #ifdef SEGA
    //music 0 frequency
    PSG = PSG_LATCH | PSG_CH1 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH1 |  0;
	PSG = 0; 

    //sound 0 frequency
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH0 |  0;
	PSG = 0;  

    //Noise
    PSG = PSG_LATCH | PSG_CH3 | PSG_VOLUME | PSG_VOLUME_OFF;
    #endif
}

void processSoundSega()
{
    #ifdef SEGA
    musicTimer();
    if(sfx_sustain == 0)
    {
        //tone
        PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
        PSG = PSG_LATCH | PSG_CH0 |  0;
	    PSG = 0;
        //noise (error sound)
        PSG = PSG_LATCH | PSG_CH3 | PSG_VOLUME | PSG_VOLUME_OFF;
    }
    else
    {
        sfx_sustain--;
    }
    #endif
}

void playGameMoveSound()
{
    #ifdef NINTENDO
    NR10_REG = 0x00;
    NR11_REG = 0x40;
    NR12_REG = 0x63;
    NR13_REG = 0x06;
    NR14_REG = 0x87;
    #else
    //tone
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH0 | 0x6;
	PSG = 0x6;
    sfx_sustain = SFX_SUSTAIN;
    #endif
}

void playErrorSound()
{
    #ifdef NINTENDO
    NR10_REG = 0x19;
    NR11_REG = 0xC2;
    NR12_REG = 0x93;
    NR13_REG = 0x73;
    NR14_REG = 0xC6;
    #else
    //noise channel
    PSG = PSG_LATCH | PSG_CH3 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH3 | 0x2;
    sfx_sustain = SFX_SUSTAIN*3;
    #endif
}

void playMenuSelectSound()
{
    #ifdef NINTENDO
    NR10_REG = 0x00;
    NR11_REG = 0x00;
    NR12_REG = 0x63;
    NR13_REG = 0x9E;
    NR14_REG = 0x87;
    #else
    //tone
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH0 | 0x3;
	PSG = 0x3;
    sfx_sustain = SFX_SUSTAIN;
    #endif
}

void playMenuBackSound()
{
    #ifdef NINTENDO
    NR10_REG = 0x00;
    NR11_REG = 0x81;
    NR12_REG = 0x63;
    NR13_REG = 0xE8;
    NR14_REG = 0x83;
    #else
    //tone
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH0 | 0xF;
	PSG = 0xf;
    sfx_sustain = SFX_SUSTAIN;
    #endif
}

void playMenuAcknowlege()
{
    #ifdef NINTENDO
    NR10_REG = 0x57;
    NR11_REG = 0x03;
    NR12_REG = 0x5e;
    NR13_REG = 0xd0;
    NR14_REG = 0x87;
    #else
    //tone
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH0 | 0x7;
	PSG = 0x7;
    sfx_sustain = SFX_SUSTAIN;
    #endif
}

void playGameAction()
{
    #ifdef NINTENDO
    NR10_REG = 0x00;
    NR11_REG = 0x80;
    NR12_REG = 0x63;
    NR13_REG = 0x6f;
    NR14_REG = 0x86;
    #else
    //tone
    PSG = PSG_LATCH | PSG_CH0 | PSG_VOLUME | PSG_VOLUME_MAX;
    PSG = PSG_LATCH | PSG_CH0 | 0x9;
	PSG = 0x9;
    sfx_sustain = SFX_SUSTAIN;
    #endif
}
