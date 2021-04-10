#ifndef LCDDRIVER_H_INCLUDED
#define LCDDRIVER_H_INCLUDED
//-----------------------------------------------------------------------------
// MK3_2560_LCD
// Version 1.7
// 24.05.2012 / UB
// "www.Mikrocontroller-4U.de"
// mc-4u(@)t-online.de
//-----------------------------------------------------------------------------
// Fuer das myAVR MK3 Board
// mit ATMega-2560 @ 16MHz
// LCD-Ausgabe
// Grafik-LCD an PortA Bit0 bis Bit7 und PortC Bit0 bis Bit7
// Quick-Jumper PortA+C muss gesetzt sein
//-----------------------------------------------------------------------------
// das File muss im Ordner
// "myAVR/WinAVR/AVR/Include/UB_MK3_2560/"
// liegen und mit #include <UB_MK3_2560/lcd.h>
// eingebunden werden
//
// Vorsicht :  Falls die Funktionen "FLOAT" benutzt werden sollen,
//             muss im Main-Programmkopf diese Zeile stehen :
//             // LinkerOption:    -Wl,-u,vfprintf -lprintf_flt -lm
//
//-----------------------------------------------------------------------------
// Belegt : Port-A (komplett)
// Belegt : Port-C (komplett)
//-----------------------------------------------------------------------------

#include <avr/io.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// INFOS zur Benutzung :
// 01. Funktion "MK3_INIT_LCD" starten
// 02. Funktion "MK3_LCD_LIGHT" schaltet Beleuchtung EIN/AUS
// 03. Funktion "MK3_LCD_DARK"  schaltet Display EIN/AUS
// 04. Funktion "MK3_LCD_INVERS" Mode : NORMAL / INVERTIERT
// 05. Funktion "MK3_LCD_CLEAR" loescht das Display
// 06. Funktion "MK3_LCD_SETCURSOR" setzt den Cursor an eine Position
// 07. Funktion "MK3_LCD_PRINT_STRING" schreibt einen String ab Cursor
// 08. Funktion "MK3_LCD_PRINT_UINT" schreibt eine Zahl ab Cursor
// 09. Funktion "MK3_LCD_PRINT_BYTE" schreibt ein Byte ab Cursor
// 10. Funktion "MK3_LCD_PRINT_AT" schreibt ein Byte an Position
// 11. Funktion "MK3_LCD_PIXEL_AT" schreibt ein Pixel an Position
// 12. Funktion "MK3_LCD_STRING_AT" schreibt einen String an Position
// 13. Funktion "MK3_LCD_UINT_AT" schreibt eine Zahl an Position
// 14. Funktion "MK3_LCD_FLOAT_AT" schreibt eine Float an Position
// 15. Funktion "MK3_LCD_PRINT_FLOAT" schreibt eine Float ab Cursor
// 16. Funktion "MK3_LCD_PRINT_UTYP" schreibt eine unsigned Typ Zahl ab Cursor
// 17. Funktion "MK3_LCD_PRINT_STYP" schreibt eine signed Typ Zahl ab Cursor
// 18. Funktion "MK3_LCD_PRINT_FTYP" schreibt eine Float Typ Zahl ab Cursor
// 19. Funktion "MK3_LCD_BMP_AT" anzeigen eines Bildes an Position (aus dem RAM)
// 20. Funktion "MK3_LCD_BMP_FLASH_AT" anzeigen eines Bildes an Position (aus dem FLASH)
// 21. Funktion "MK3_LCD_STRING_PX" schreibt einen Text Pixelgenau
// 22. Funktion "MK3_LCD_UTYP_PX" schreibt eine unsigned Typ Zahl Pixelgenau
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Display Groesse
//-----------------------------------------------------------------------------
#define LCD_MAXY 64   // Anzahl der Pixel in Y-Richtung (0...63)
#define LCD_MAXX 128  // Anzahl der Pixel in X-Richtung (0...127)


//-----------------------------------------------------------------------------
// Enumerationen fuer die Funktioen
//-----------------------------------------------------------------------------

typedef enum {
  MODE_SET = 0,
  MODE_RESET,
  MODE_TOGGLE,
  MODE_OVERWRITE
}DRAWTYP;

typedef enum {
  DEZ_U5 =0, // Typ = dezimal, unsigned, 5 ziffern
  DEZ_U4,    // Typ = dezimal, unsigned, 4 ziffern
  DEZ_U3,    // Typ = dezimal, unsigned, 3 ziffern
  DEZ_U2,    // Typ = dezimal, unsigned, 2 ziffern
  DEZ_U1,    // Typ = dezimal, unsigned, 1 ziffer
  DEZ_U0,    // Typ = dezimal, unsigned, ziffern abhaengig von der zahl
  HEX_U4,    // Typ = hex,     unsigned, 4 ziffern
  HEX_U3,    // Typ = hex,     unsigned, 3 ziffern
  HEX_U2,    // Typ = hex,     unsigned, 2 ziffern
  HEX_U1,    // Typ = hex,     unsigned, 1 ziffer
  HEX_U0     // Typ = hex,     unsigned, ziffern abhaengig von der zahl
}U_ZAHLTYP;

typedef enum {
  DEZ_S5 =0, // Typ = dezimal, signed,   5 ziffern
  DEZ_S4,    // Typ = dezimal, signed,   4 ziffern
  DEZ_S3,    // Typ = dezimal, signed,   3 ziffern
  DEZ_S2,    // Typ = dezimal, signed,   2 ziffern
  DEZ_S1,    // Typ = dezimal, signed,   1 ziffer
  DEZ_S0     // Typ = dezimal, signed,   ziffern abhaengig von der zahl
}S_ZAHLTYP;

typedef enum {
  FLT_0P1 =0, // Float mit 1 Nachkommastelle
  FLT_0P2,    // Float mit 2 Nachkommastellen
  FLT_0P3,    // Float mit 3 Nachkommastellen
  FLT_1P1,    // Float mit 1 Ziffer und 1 Nachkommastelle
  FLT_1P2,    // Float mit 1 Ziffer und 2 Nachkommastellen
  FLT_1P3,    // Float mit 1 Ziffer und 3 Nachkommastellen
  FLT_2P1,    // Float mit 2 Ziffern und 1 Nachkommastelle
  FLT_2P2,    // Float mit 2 Ziffern und 2 Nachkommastellen
  FLT_2P3,    // Float mit 2 Ziffern und 3 Nachkommastellen
  FLT_3P1,    // Float mit 3 Ziffern und 1 Nachkommastelle
  FLT_3P2,    // Float mit 3 Ziffern und 2 Nachkommastellen
  FLT_3P3,    // Float mit 3 Ziffern und 3 Nachkommastellen
  FLT_4P1,    // Float mit 4 Ziffern und 1 Nachkommastelle
  FLT_4P2,    // Float mit 4 Ziffern und 2 Nachkommastellen
  FLT_4P3,    // Float mit 4 Ziffern und 3 Nachkommastellen
  FLT_5P1,    // Float mit 5 Ziffern und 1 Nachkommastelle
  FLT_5P2,    // Float mit 5 Ziffern und 2 Nachkommastellen
  FLT_5P3,    // Float mit 5 Ziffern und 3 Nachkommastellen
  FLT_6P1,    // Float mit 6 Ziffern und 1 Nachkommastelle
  FLT_6P2,    // Float mit 6 Ziffern und 2 Nachkommastellen
  FLT_6P3     // Float mit 6 Ziffern und 3 Nachkommastellen
}F_ZAHLTYP;

typedef struct
{
       uint8_t x;
       uint8_t y;
} point;



//-----------------------------------------------------------------------------
// Funktionen die vom MAIN benutzt werden koennen
//-----------------------------------------------------------------------------
void MK3_INIT_LCD(void);                         // zum initialisieren des LCD
void MK3_LCD_LIGHT(bool wert);                   // zum ein-ausschalten der Beleuchtung
void MK3_LCD_CLEAR(void);                        // zum loeschen des LCD
void MK3_LCD_SETCURSOR(unsigned char x,unsigned char y);           // zum setzen des cursors
void MK3_LCD_PRINT_BYTE(char wert);              // zum schreiben von daten ab Cursor
void MK3_LCD_PRINT_AT(unsigned char x,unsigned char y,char wert);  // zum schreiben von daten
void MK3_LCD_PIXEL_AT(unsigned char x,unsigned char y,DRAWTYP m);  // zum zeichnen eines pixels
void MK3_LCD_STRING_AT(unsigned char x,unsigned char y,const char *ptr); // zum schreiben eines Strings
void MK3_LCD_UINT_AT(unsigned char x,unsigned char y,unsigned int wert); // zum schreiben einer Zahl
void MK3_LCD_FLOAT_AT(unsigned char x,unsigned char y,double wert);      // zum schreiben einer Float
void MK3_LCD_DARK(bool wert);                    // zum ein-ausschalten des Display
void MK3_LCD_INVERS(bool wert);                  // zum invertieren des Display
void MK3_LCD_PRINT_STRING(const char *ptr);      // zum schreiben von texten ab Cursor
void MK3_LCD_PRINT_UINT(unsigned int wert);      // zum schreiben von zahlen ab Cursor
void MK3_LCD_PRINT_FLOAT(double wert);           // zum schreiben von float ab Cursor
void MK3_LCD_PRINT_UTYP(unsigned int wert,U_ZAHLTYP typ); // unsigned zahlentyp
void MK3_LCD_PRINT_STYP(int wert,S_ZAHLTYP typ);          // signed zahlentyp
void MK3_LCD_PRINT_FTYP(double wert,F_ZAHLTYP typ);       // float zahlentyp
void MK3_LCD_BMP_AT(unsigned char x,unsigned char y,const unsigned char *ptr,int laenge);       // Bild anzeigen
void MK3_LCD_BMP_FLASH_AT(unsigned char x,unsigned char y,const unsigned char *ptr,int laenge); // Bild anzeigen
void MK3_LCD_STRING_PX(unsigned char x,unsigned char y,const char *ptr);               // String (Pixelgenau)
void MK3_LCD_UTYP_PX(unsigned char x,unsigned char y,unsigned int wert,U_ZAHLTYP typ); // unsigned (Pixelgenau)

void drawline(point p0, point p1);
void toogleLCDDisp();
void toogleLCDLight();
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// interne Funktionen
//-----------------------------------------------------------------------------
void ub_pause(unsigned int p);  // kleine Pause
void lcdSendCmd(char wert);     // sendet kommandos zum LCD
void lcdSend(char wert);        // sendet werte zum LCD
char lcdReadData(void);         // liest ein byte an Cursor-Pos aus
void lcdPrintAsc(char wert);    // schreib ein Ascii-Zeichen an Cursor-Pos
void lcdBytePx(unsigned char x,unsigned char y,char wert);     // schreibt ein Byte (Pixelgenau)
void lcdPrintAscPx(unsigned char x,unsigned char y,char wert); // schreib ein Ascii-Zeichen (Pixelgenau)



//-----------------------------------------------------------------------------
// Port-Pin definition
//-----------------------------------------------------------------------------

#define MK3_DDR_LCDD DDRC	// DDR-Register der LCD-Daten
#define MK3_PORT_LCDD PORTC	// Port der LCD-Daten
#define MK3_PIN_LCDD PINC	// Pin der LCD-Daten

#define MK3_DDR_LCDC DDRA	// DDR-Register der LCD-Control
#define MK3_PORT_LCDC PORTA	// Port der LCD-Control
#define MK3_PIN_LCDC PINA	// Pin der LCD-Control

#define LCD_PS   0x80		// Mode:       0=Seriell, 1=Parallel
#define LCD_MI   0x40		// Mode:       0=8080   , 1=6800
#define LCD_HB   0x20		// Beleuchtung:0=AUS    , 1=EIN
#define LCD_CLK  0x10		// Clk:        Daten bei Hi-Flanke
#define LCD_WR   0x08		// ReadWrite:  0=Write  , 1=Read
#define LCD_RS   0x04		// C/D:        0=Control, 1=Data
#define LCD_RST  0x02		// Reset:      0=reset  , 1=go
#define LCD_CS   0x01		// ChipSelect: 0=enable , 1=disable

#endif
