//-----------------------------------------------------------------------------
// MK3_2560_BEEP
// Version 1.2
// 29.05.2012 / UB
// "www.Mikrocontroller-4U.de"
// mc-4u(@)t-online.de
//-----------------------------------------------------------------------------
// Belegt : @MK3 (ATMega2560)
//            Port-E (Bit3)
//            Timer-0
//-----------------------------------------------------------------------------

#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#include "speaker.h"


#define TONE_OFF      0    // no sound
#define TONE_C1       264
#define TONE_CS1      282
#define TONE_D1       297
#define TONE_DS1      317
#define TONE_E1       330
#define TONE_F1       352
#define TONE_FS1      371
#define TONE_G1       396
#define TONE_GS1      422
#define TONE_A1       440 // (Concert pitch)
#define TONE_AS1      475
#define TONE_B1       495

const uint16_t tone[] PROGMEM =
{
    TONE_OFF,

    TONE_C1,
    TONE_CS1,
    TONE_D1,
    TONE_DS1,
    TONE_E1,
    TONE_F1,
    TONE_FS1,
    TONE_G1,
    TONE_GS1,
    TONE_A1,
    TONE_AS1,
    TONE_B1,

    TONE_C1 << 1,
    TONE_CS1 << 1,
    TONE_D1 << 1,
    TONE_DS1 << 1,
    TONE_E1 << 1,
    TONE_F1 << 1,
    TONE_FS1 << 1,
    TONE_G1 << 1,
    TONE_GS1 << 1,
    TONE_A1 << 1,
    TONE_AS1 << 1,
    TONE_B1 << 1,

    TONE_C1 << 2,
    TONE_CS1 << 2,
    TONE_D1 << 2,
    TONE_DS1 << 2,
    TONE_E1 << 2,
    TONE_F1 << 2,
    TONE_FS1 << 2,
    TONE_G1 << 2,
    TONE_GS1 << 2,
    TONE_A1 << 2,
    TONE_AS1 << 2,
    TONE_B1 << 2,

    TONE_C1 << 3,
    TONE_CS1 << 3,
    TONE_D1 << 3,
    TONE_DS1 << 3,
    TONE_E1 << 3,
    TONE_F1 << 3,
    TONE_FS1 << 3,
    TONE_G1 << 3,
    TONE_GS1 << 3,
    TONE_A1 << 3,
    TONE_AS1 << 3,
    TONE_B1 << 3,

    TONE_C1 << 4,
};

enum note
{
    off=0,
    c1,
    cs1,
    d1,
    ds1,
    e1,
    f1,
    fs1,
    g1,
    gs1,
    a1,
    as1,
    b1,

    c2,
    cs2,
    d2,
    ds2,
    e2,
    f2,
    fs2,
    g2,
    gs2,
    a2,
    as2,
    b2,

    c3,
    cs3,
    d3,
    ds3,
    e3,
    f3,
    fs3,
    g3,
    gs3,
    a3,
    as3,
    b3,

    c4,
    cs4,
    d4,
    ds4,
    e4,
    f4,
    fs4,
    g4,
    gs4,
    a4,
    as4,
    b4,

    c5,
} __attribute__((packed));

// time in 10ms to be able to save the enum as char
enum toneLen
{
    LEN_STOP   =0,             // zum beeden eines Songs
    LEN_QUARTER= 6000/*10ms*/ / BPM,

    LEN_EIGHT  =LEN_QUARTER / 2,
    LEN_SIXTEEN=LEN_EIGHT   / 2,

    LEN_HALF   =LEN_QUARTER*2,
    LEN_FULL   =LEN_HALF   *2,
} __attribute__((packed));


const Note_t Scale[] PROGMEM=
{
    {c1,LEN_QUARTER},
    {d1,LEN_QUARTER},
    {e1,LEN_QUARTER},
    {f1,LEN_QUARTER},
    {g1,LEN_QUARTER},
    {a1,LEN_QUARTER},
    {b1,LEN_QUARTER},
    {c2,LEN_QUARTER},

    {d2,LEN_QUARTER},
    {e2,LEN_QUARTER},
    {f2,LEN_QUARTER},
    {g2,LEN_QUARTER},
    {a2,LEN_QUARTER},
    {b2,LEN_QUARTER},
    {c3,LEN_QUARTER},

    {d3,LEN_QUARTER},
    {e3,LEN_QUARTER},
    {f3,LEN_QUARTER},
    {g3,LEN_QUARTER},
    {a3,LEN_QUARTER},
    {b3,LEN_QUARTER},
    {c4,LEN_QUARTER},

    {d4,LEN_QUARTER},
    {e4,LEN_QUARTER},
    {f4,LEN_QUARTER},
    {g4,LEN_QUARTER},
    {a4,LEN_QUARTER},
    {b4,LEN_QUARTER},
    {c5,LEN_QUARTER},

    {off,LEN_STOP}  // beendet das Lied
};

const Note_t Elise[] PROGMEM=
{
    {e2,LEN_EIGHT},
    {ds2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {ds2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {c2,LEN_EIGHT},
    {a1,LEN_QUARTER},
    {off,LEN_EIGHT},
    {c1,LEN_EIGHT},
    {e1,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {b1,LEN_QUARTER},
    {off,LEN_EIGHT},
    {e1,LEN_EIGHT},
    {gs1,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {c2,LEN_QUARTER},
    {off,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {ds2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {ds2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {c2,LEN_EIGHT},
    {a1,LEN_QUARTER},
    {off,LEN_EIGHT},
    {c1,LEN_EIGHT},
    {e1,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {b1,LEN_QUARTER},
    {off,LEN_EIGHT},
    {e1,LEN_EIGHT},
    {c2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {a1,LEN_QUARTER},
    {off,LEN_STOP}  // beendet das Lied
};

const Note_t Korobeiniki[] PROGMEM=
{
    {e3,LEN_QUARTER},
    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_EIGHT},
    {e3,LEN_SIXTEEN},
    {d3,LEN_SIXTEEN},
    {c3,LEN_EIGHT},
    {b2,LEN_EIGHT},
    {a2,LEN_QUARTER},

    {a2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {e3,LEN_QUARTER},
    {d3,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {b2,LEN_QUARTER},

    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {e3,LEN_QUARTER},
    {c3,LEN_QUARTER},
    {a2,LEN_QUARTER},
    {a2,LEN_HALF},

    {off,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {f3,LEN_EIGHT},
    {a3,LEN_QUARTER},
    {g3,LEN_EIGHT},
    {f3,LEN_EIGHT},

    {e3,LEN_QUARTER+LEN_EIGHT},
    {c3,LEN_EIGHT},
    {e3,LEN_QUARTER},
    {d3,LEN_EIGHT},
    {c3,LEN_EIGHT},

    {b2,LEN_QUARTER},
    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {e3,LEN_QUARTER},

    {c3,LEN_QUARTER},
    {a2,LEN_QUARTER},
    {a2,LEN_HALF},



    {e3,LEN_QUARTER},
    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_EIGHT},
    {e3,LEN_SIXTEEN},
    {d3,LEN_SIXTEEN},
    {c3,LEN_EIGHT},
    {b2,LEN_EIGHT},
    {a2,LEN_QUARTER},

    {a2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {e3,LEN_QUARTER},
    {d3,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {b2,LEN_QUARTER},

    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {e3,LEN_QUARTER},
    {c3,LEN_QUARTER},
    {a2,LEN_QUARTER},
    {a2,LEN_HALF},

    {off,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {f3,LEN_EIGHT},
    {a3,LEN_QUARTER},
    {g3,LEN_EIGHT},
    {f3,LEN_EIGHT},

    {e3,LEN_QUARTER+LEN_EIGHT},
    {c3,LEN_EIGHT},
    {e3,LEN_QUARTER},
    {d3,LEN_EIGHT},
    {c3,LEN_EIGHT},

    {b2,LEN_QUARTER},
    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {e3,LEN_QUARTER},

    {c3,LEN_QUARTER},
    {a2,LEN_QUARTER},
    {a2,LEN_HALF},



    {e2,LEN_HALF},
    {c2,LEN_HALF},
    {d2,LEN_HALF},
    {b1,LEN_HALF},
    {c2,LEN_HALF},
    {a1,LEN_HALF},
    {gs1,LEN_HALF+LEN_QUARTER},


    {off,LEN_QUARTER},
    {e2,LEN_HALF},
    {c2,LEN_HALF},
    {d2,LEN_HALF},
    {b1,LEN_HALF},
    {c2,LEN_QUARTER},
    {e2,LEN_QUARTER},
    {a2,LEN_HALF},
    {gs2,LEN_HALF+LEN_QUARTER},
    {off,LEN_QUARTER},



    {e3,LEN_QUARTER},
    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_EIGHT},
    {e3,LEN_SIXTEEN},
    {d3,LEN_SIXTEEN},
    {c3,LEN_EIGHT},
    {b2,LEN_EIGHT},
    {a2,LEN_QUARTER},

    {a2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {e3,LEN_QUARTER},
    {d3,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {b2,LEN_QUARTER},

    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {e3,LEN_QUARTER},
    {c3,LEN_QUARTER},
    {a2,LEN_QUARTER},
    {a2,LEN_HALF},

    {off,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {f3,LEN_EIGHT},
    {a3,LEN_QUARTER},
    {g3,LEN_EIGHT},
    {f3,LEN_EIGHT},

    {e3,LEN_QUARTER+LEN_EIGHT},
    {c3,LEN_EIGHT},
    {e3,LEN_QUARTER},
    {d3,LEN_EIGHT},
    {c3,LEN_EIGHT},

    {b2,LEN_QUARTER},
    {b2,LEN_EIGHT},
    {c3,LEN_EIGHT},
    {d3,LEN_QUARTER},
    {e3,LEN_QUARTER},

    {c3,LEN_QUARTER},
    {a2,LEN_QUARTER},
    {a2,LEN_HALF},

    {off,LEN_STOP}// beendet das Lied
};

const Note_t Menuet[] PROGMEM=
{
    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {as1,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {b1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {as1,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {g2,LEN_EIGHT},
    {e2,LEN_EIGHT},

    {cs2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {d2,LEN_EIGHT},

    {b1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {b1,LEN_EIGHT},

    {as1,LEN_EIGHT},
    {fs1,LEN_EIGHT},
    {as1,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {e2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {as1,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {b1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {as1,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {g2,LEN_EIGHT},
    {e2,LEN_EIGHT},

    {cs2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {a2,LEN_EIGHT},
    {e2,LEN_EIGHT},

    {fs2,LEN_EIGHT},
    {a2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {cs2,LEN_EIGHT},

    {d2,LEN_HALF},
    {off,LEN_QUARTER},

//----------------------------

    {a2,LEN_EIGHT},
    {g2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {cs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {g2,LEN_EIGHT},

    {fs2,LEN_EIGHT},
    {g2,LEN_EIGHT},
    {a2,LEN_QUARTER},
    {cs2,LEN_QUARTER},

    {d2,LEN_QUARTER},
    {fs2,LEN_QUARTER},
    {e2,LEN_QUARTER},

    {fs2,LEN_QUARTER},
    {b2,LEN_QUARTER},
    {gs2,LEN_QUARTER},

    {b1,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {d2,LEN_QUARTER},
    {cs2,LEN_QUARTER},

    {b1,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {gs1,LEN_EIGHT},
    {fs1,LEN_EIGHT},
    {gs1,LEN_EIGHT},
    {f1,LEN_EIGHT},

    {fs1,LEN_HALF},
    {off,LEN_QUARTER},

    {a1,LEN_EIGHT+LEN_QUARTER},
    {b1,LEN_SIXTEEN},
    {c2,LEN_SIXTEEN},
    {b1,LEN_QUARTER},

    {a1,LEN_EIGHT},
    {g1,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {fs1,LEN_EIGHT},
    {g1,LEN_EIGHT},
    {e1,LEN_EIGHT},

    {b1,LEN_QUARTER+LEN_EIGHT},
    {cs2,LEN_SIXTEEN},
    {d2,LEN_SIXTEEN},
    {cs2,LEN_QUARTER},

    {b1,LEN_EIGHT},
    {as1,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {gs1,LEN_EIGHT},
    {a1,LEN_EIGHT},
    {fs1,LEN_EIGHT},

    {e2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {fs1,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {fs2,LEN_EIGHT},

    {g2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {fs2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {cs2,LEN_EIGHT},

    {d2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {fs1,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {b1,LEN_EIGHT},

    {fs2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {b1,LEN_EIGHT},

    {cs2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {b1,LEN_EIGHT},

    {fs2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {g2,LEN_EIGHT},
    {b1,LEN_EIGHT},
    {e2,LEN_EIGHT},
    {b1,LEN_EIGHT},

    {fs2,LEN_QUARTER},
    {e2,LEN_EIGHT},
    {d2,LEN_EIGHT},
    {cs2,LEN_EIGHT},
    {d2,LEN_EIGHT},

    {b1,LEN_HALF+LEN_QUARTER},
    {off,LEN_STOP}// beendet das Lied
};

//-----------------------------------------------------------------------------
// interne Globale Variabeln
//-----------------------------------------------------------------------------
typedef struct
{
    volatile unsigned int frq_ist_10us;

    // holds the time in 10us to pass until the speaker gets toogled
    // used to evaluate the freq to play
    unsigned int frq_soll_10us;

    // holds the time in 10us passed till last ISR call
    // used to evaluate the duration for a tone
    // only counts to 10
    volatile uint8_t time_ist_10us;

    // holds the time in 100us passed till last ISR call
    // used to evaluate the duration for a tone
    volatile unsigned int time_ist_100us;

    unsigned int time_soll_100us;
    volatile bool beep_on;
    volatile bool sound_on;
} beep_t;
static beep_t beep;

//-----------------------------------------------------------------------------
void beep_init(void)      // zum initialisieren des Sound
{
    // Pin als Ausgang
    // und AUS
    DDR_BEEP|=BEEP_OUT;    // als Ausgang
    PORT_BEEP&=~BEEP_OUT;  // BEEP AUS

    // alle Variablen initialisieren
    beep.frq_ist_10us=0;
    beep.frq_soll_10us=0;
    beep.time_ist_10us=0;
    beep.time_ist_100us=0;
    beep.time_soll_100us=0;
    beep.beep_on=false;
    beep.sound_on=false;

    // Hardware = myAVR MK3-Board

    // benutzter Timer : Timer-0
    // im Mode2 -> ClearTimerOnCompare-A Match

    TCCR0A=0x02;         // Mode2 (CTC)
//    TCCR0B=0x00;
//
//    TCNT0=0x00;
//    OCR0B=0x00;          // Comp-B wird nicht benutzt

    TIMSK0=0x02;         // Interrupt bei Comp-A

    // 16MHz
    OCR0A=159;
    TCCR0B|=0x01; // Vorteiler=1, OCR=159 -> 10.00us

    sei(); // Alle Interrupts einschalten
}


//-----------------------------------------------------------------------------
// startet einen Ton mit einer bestimmten dauer
// Ton wird dann per Intterupt gestoppt
// frq in Hz
// dauer in ms
//-----------------------------------------------------------------------------
void beep_play_sound(unsigned int frq, unsigned int dauer)
{
    while(beep.beep_on);

    if((dauer/10) != LEN_STOP)
    {
        if(frq != pgm_read_word(&tone[off]))
        {
            // convert frq to 10us and make sure the beep is toogled double the frq
            float frq_wert=(1.0/frq)*(100000/2);

            beep.frq_soll_10us=(unsigned int)(frq_wert);
            beep.time_soll_100us=dauer*10;
            beep.sound_on=true;
            beep.beep_on=true;
        }
        else
        {
            // pause spielen (ohne ton)
            beep.frq_soll_10us=0;
            beep.time_soll_100us=dauer*10;
            beep.sound_on=false;
            beep.beep_on=true;
        }
    }
    else
    {
        // skip this tone
        beep.frq_soll_10us=0;
        beep.time_soll_100us=0;
        beep.sound_on=false;
        beep.beep_on=true;
    }
}

// Hardware = myAVR MK3-Board
//-----------------------------------------------------------------------------
// ISR von Timer-0 Compare-A
// 1. Verwaltet den Zahler fuer die Tonfrequenz und Tondauer
// 2. Toggelt bei bedarf den Ton-Ausgang
//-----------------------------------------------------------------------------
ISR (TIMER0_COMPA_vect)                            // ISR : Timer0 Comp-A
{
    // Beim Comp-A interrupt wird
    // der Timer automatisch wieder auf 0 gesetzt
    //
    // der Interrupt wird alle 10us aufgerufen

    if(beep.beep_on==true)
    {
        beep.frq_ist_10us++; // 10us are over
        beep.time_ist_10us++;
        if(beep.time_ist_10us>=10)
        {
            beep.time_ist_10us=0;
            beep.time_ist_100us++;
        }

        // test ob frequenz erreicht
        if(beep.frq_ist_10us>=beep.frq_soll_10us)
        {
            beep.frq_ist_10us=0;
            if(beep.sound_on)
            {
                PORT_BEEP^=BEEP_OUT;  // toggle beep
            }
        }

        // test ob dauer erreicht
        if(beep.time_ist_100us>=beep.time_soll_100us)
        {
            beep.time_ist_100us=0;
            beep.frq_ist_10us=0;
            beep.beep_on=false;
            PORT_BEEP&=~BEEP_OUT;  // beep off
        }
    }
}

void beep_play_song(const Note_t *songToPlay)
{
    uint16_t pos=0;
    bool exit=false;

    do
    {
        Note_t buffer;
        memcpy_P(&buffer, (PGM_VOID_P)(songToPlay+pos), sizeof(Note_t));

        if(buffer.length==LEN_STOP || pos==0xFFFF)
        {
            exit=true;
        }
        else
        {
            uint16_t frq = pgm_read_word(&tone[buffer.pitch]);

            beep_play_sound(frq,buffer.length*10); // play a note
            pos++;
        }
    }
    while(!exit);
}
