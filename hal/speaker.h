#ifndef SPEAKER_H_INCLUDED
#define SPEAKER_H_INCLUDED

typedef struct
{
    uint8_t pitch;
    uint8_t length;
} Note_t;


void beep_init(void);
void beep_play_sound(unsigned int frq, unsigned int dauer);
void beep_play_song(const Note_t *songToPlay);


#define DDR_BEEP  DDRE    // DDR-Register vom Sound
#define PORT_BEEP PORTE   // Port vom Sound
#define BEEP_OUT  (1<<PE3)    // Bit vom Sound

#define BPM 150


#endif // SPEAKER_H_INCLUDED
