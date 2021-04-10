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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

//-----------------------------------------------------------------------------
// Hier koennen Funktionen die nicht benutzt werden
// abgeschaltet werden...einfach den DEFINE von "1" auf "0" aendern
//-----------------------------------------------------------------------------

#define USE_MK3_LCD_LIGHT 1
#define USE_MK3_LCD_DARK 1
#define USE_MK3_LCD_INVERS 0
#define USE_MK3_LCD_PRINT_STRING 0
#define USE_MK3_LCD_PRINT_UINT 0
#define USE_MK3_LCD_PRINT_FLOAT 0
#define USE_MK3_LCD_PRINT_AT 0
#define USE_MK3_LCD_PIXEL_AT 1
#define USE_MK3_LCD_STRING_AT 0
#define USE_MK3_LCD_UINT_AT 0
#define USE_MK3_LCD_FLOAT_AT 0
#define USE_MK3_LCD_PRINT_TYP 0
#define USE_MK3_LCD_BMP_AT 0
#define USE_MK3_LCD_BMP_FLASH_AT 0
#define USE_MK3_LCD_PIXELGENAU 1

//-----------------------------------------------------------------------------
// hier wird ein eigener FLASH-Bereich definiert,damit keine
// Compiler-Warning erzeugt wird, wie beim benutzen von "PROGMEM"
//-----------------------------------------------------------------------------
#define UB_PROGMEM __attribute((section(".progmem.ubflash")))


//-----------------------------------------------------------------------------
void MK3_INIT_LCD(void) {	// zum initialisieren des LCD

  // Daten-Port
  // Als Eingang (ohne Pull-UP)
  MK3_DDR_LCDD=0x00;
  MK3_PORT_LCDD=0;

  // Control-Port
  // Als Ausgang
  MK3_DDR_LCDC=0xff;
  MK3_PORT_LCDC=0xC3;
  MK3_PORT_LCDC|=LCD_PS;   // Mode=Parallel
  MK3_PORT_LCDC|=LCD_MI;   // Mode=6800
  MK3_PORT_LCDC&=~LCD_HB;  // Beleuchtung AUS
  MK3_PORT_LCDC&=~LCD_CLK; // Clk=Lo
  MK3_PORT_LCDC&=~LCD_WR;  // Write
  MK3_PORT_LCDC&=~LCD_RS;  // Control
  MK3_PORT_LCDC|=LCD_RST;  // Reset=OFF
  MK3_PORT_LCDC|=LCD_CS;   // CS=Disable


  ub_pause(60000);

  // Hardware-Reset Sequenz
  MK3_PORT_LCDC&=~LCD_RST;
  ub_pause(10000);
  MK3_PORT_LCDC|=LCD_RST;
  ub_pause(10000);

  // LCD-Init Sequenz
  lcdSendCmd(0xE2);
  lcdSendCmd(0xA0);
  lcdSendCmd(0xC8);
  lcdSendCmd(0xA2);
  lcdSendCmd(0x2F);
  lcdSendCmd(0x26);
  lcdSendCmd(0xF8);
  lcdSendCmd(0x00);
  lcdSendCmd(0x81);
  lcdSendCmd(0x09);
  lcdSendCmd(0xE0);
  lcdSendCmd(0xAF);

}

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_LIGHT==1
void MK3_LCD_LIGHT(bool wert) {  // zum ein-ausschalten der Beleuchtung
  if(wert)
  {
    MK3_PORT_LCDC|=LCD_HB;    // Beleuchtung EIN
  }
  else
  {
    MK3_PORT_LCDC&=~LCD_HB;  // Beleuchtung AUS
  }
}
#endif

//-----------------------------------------------------------------------------
void MK3_LCD_CLEAR(void) {       // zum loeschen des LCD
  unsigned char x,y;

  for(y=0;y<LCD_MAXY;y+=8) {
    MK3_LCD_SETCURSOR(0,y);
    for(x=0;x<LCD_MAXX;x++) {
      MK3_LCD_PRINT_BYTE(0);
    }
  }
  MK3_LCD_SETCURSOR(0,0);

}

//-----------------------------------------------------------------------------
void MK3_LCD_SETCURSOR(char x,char y) { // zum setzen des cursors
  // X = X-Koordinate in Pixel (0...MAXX-1)
  // Y = Y-Koordinate in Pixel (0...MAXY-1)


  // X-Koordinate
  lcdSendCmd(0x10+(x>>4));
  lcdSendCmd(0x00+(x&0x0F));

  // Y-Koordinate
  lcdSendCmd(0xB0+(y>>3));
}

//-----------------------------------------------------------------------------
void MK3_LCD_PRINT_BYTE(char wert) { // zum schreiben von daten ab Cursor
  MK3_PORT_LCDC|=LCD_RS; // sendet Daten-Werte
  lcdSend(wert);
}

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PRINT_AT==1
void MK3_LCD_PRINT_AT(char x,char y,char wert) {     // zum schreiben von daten
  MK3_LCD_SETCURSOR(x,y);
  MK3_LCD_PRINT_BYTE(wert);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PIXEL_AT==1
void MK3_LCD_PIXEL_AT(char x,char y,DRAWTYP m) {  // zum zeichnen eines pixels
  char maske,wert;

  MK3_LCD_SETCURSOR(x,y);
  maske=1<<(y&0x07);
  wert=lcdReadData();
  if(m==MODE_SET) wert|=maske;
  if(m==MODE_RESET) wert&=~maske;
  if(m==MODE_TOGGLE) wert^=maske;
  if(m==MODE_OVERWRITE) wert|=maske;
  MK3_LCD_PRINT_BYTE(wert);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_DARK==1
void MK3_LCD_DARK(bool wert) {		// zum ein-ausschalten des Display
  if(wert==true) lcdSendCmd(0xA5);      // Didplay dunkel
  if(wert==false) lcdSendCmd(0xA4);     // Didplay normal
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_INVERS==1
void MK3_LCD_INVERS(bool wert) { 	// zum invertieren des Display
  if(wert==true) lcdSendCmd(0xA7);      // Didplay INVERS
  if(wert==false) lcdSendCmd(0xA6);     // Didplay NORMAL
}
#endif

//-----------------------------------------------------------------------------
void MK3_LCD_PRINT_STRING(const char *ptr) { // zum schreiben von texten ab Cursor
  unsigned int n;
  unsigned char c;

  for(n=0;n<=255;n++) {
    c=ptr[n];
    if(c==0) {
      break;
    }
    else {
      lcdPrintAsc(c);
    }
  }
}

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_STRING_AT==1
void MK3_LCD_STRING_AT(char x,char y,const char *ptr) {  // zum schreiben eines Strings
  MK3_LCD_SETCURSOR(x,y);
  MK3_LCD_PRINT_STRING(ptr);
}
#endif


//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PRINT_UINT==1 || USE_MK3_LCD_UINT_AT==1
void MK3_LCD_PRINT_UINT(unsigned int wert) {     // zum schreiben von zahlen ab Cursor
  char buf[7];

  sprintf(buf,"%5u",wert); // 5 stellige unsigned_int (ohne fuehrenden Nullen)
//  sprintf(buf,"%05u",wert); // 5 stellige unsigned_int (mit fuehrenden Nullen)

  MK3_LCD_PRINT_STRING(buf);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PRINT_FLOAT==1 || USE_MK3_LCD_FLOAT_AT==1
void MK3_LCD_PRINT_FLOAT(double wert) {     // zum schreiben von float ab Cursor
  char buf[12];

  // Anzeige einer Float mit 9 Ziffern (Werte von 0.0001 bis 99999.9999)
  sprintf(buf,"%10.4f",wert);  // 5 Vorkommastellen, 1 Punkt, 4 Nachkommastellen

  MK3_LCD_PRINT_STRING(buf);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_UINT_AT==1
void MK3_LCD_UINT_AT(char x,char y,unsigned int wert) {  // zum schreiben einer Zahl
  MK3_LCD_SETCURSOR(x,y);
  MK3_LCD_PRINT_UINT(wert);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_FLOAT_AT==1
void MK3_LCD_FLOAT_AT(char x,char y,double wert) {  // zum schreiben einer Float
  MK3_LCD_SETCURSOR(x,y);
  MK3_LCD_PRINT_FLOAT(wert);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PRINT_TYP==1
void MK3_LCD_PRINT_UTYP(unsigned int wert,U_ZAHLTYP typ) {
  // anzeigen einer "unsigned" Zahl
  // mit einem bestimmten Formatierung

  char buf[7];

  switch(typ) {
    case DEZ_U5 : sprintf(buf,"%05u",wert);break;
    case DEZ_U4 : sprintf(buf,"%04u",wert);break;
    case DEZ_U3 : sprintf(buf,"%03u",wert);break;
    case DEZ_U2 : sprintf(buf,"%02u",wert);break;
    case DEZ_U1 : sprintf(buf,"%01u",wert);break;
    case DEZ_U0 : sprintf(buf,"%u",wert);break;
    case HEX_U4 : sprintf(buf,"%04x",wert);break;
    case HEX_U3 : sprintf(buf,"%03x",wert);break;
    case HEX_U2 : sprintf(buf,"%02x",wert);break;
    case HEX_U1 : sprintf(buf,"%01x",wert);break;
    case HEX_U0 : sprintf(buf,"%x",wert);break;
    default : sprintf(buf,"%05u",wert);
  }
  MK3_LCD_PRINT_STRING(buf);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PRINT_TYP==1
void MK3_LCD_PRINT_STYP(int wert,S_ZAHLTYP typ) {
  // anzeigen einer "signed" Zahl
  // mit einem bestimmten Formatierung

  char buf[7];

  switch(typ) {
    case DEZ_S5 : sprintf(buf,"%+06i",wert);break;
    case DEZ_S4 : sprintf(buf,"%+05i",wert);break;
    case DEZ_S3 : sprintf(buf,"%+04i",wert);break;
    case DEZ_S2 : sprintf(buf,"%+03i",wert);break;
    case DEZ_S1 : sprintf(buf,"%+02i",wert);break;
    case DEZ_S0 : sprintf(buf,"%+i",wert);break;
    default : sprintf(buf,"%+06i",wert);
  }
  MK3_LCD_PRINT_STRING(buf);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PRINT_TYP==1
void MK3_LCD_PRINT_FTYP(double wert,F_ZAHLTYP typ) {
  // anzeigen einer Kommazahl
  // mit einem bestimmten Formatierung

  char buf[12];

  switch(typ) {
    case FLT_0P1 : sprintf(buf,"%.1f",wert);break;
    case FLT_0P2 : sprintf(buf,"%.2f",wert);break;
    case FLT_0P3 : sprintf(buf,"%.3f",wert);break;
    case FLT_1P1 : sprintf(buf,"%03.1f",wert);break;
    case FLT_1P2 : sprintf(buf,"%04.2f",wert);break;
    case FLT_1P3 : sprintf(buf,"%05.3f",wert);break;
    case FLT_2P1 : sprintf(buf,"%04.1f",wert);break;
    case FLT_2P2 : sprintf(buf,"%05.2f",wert);break;
    case FLT_2P3 : sprintf(buf,"%06.3f",wert);break;
    case FLT_3P1 : sprintf(buf,"%05.1f",wert);break;
    case FLT_3P2 : sprintf(buf,"%06.2f",wert);break;
    case FLT_3P3 : sprintf(buf,"%07.3f",wert);break;
    case FLT_4P1 : sprintf(buf,"%06.1f",wert);break;
    case FLT_4P2 : sprintf(buf,"%07.2f",wert);break;
    case FLT_4P3 : sprintf(buf,"%08.3f",wert);break;
    case FLT_5P1 : sprintf(buf,"%07.1f",wert);break;
    case FLT_5P2 : sprintf(buf,"%08.2f",wert);break;
    case FLT_5P3 : sprintf(buf,"%09.3f",wert);break;
    case FLT_6P1 : sprintf(buf,"%08.1f",wert);break;
    case FLT_6P2 : sprintf(buf,"%09.2f",wert);break;
    case FLT_6P3 : sprintf(buf,"%010.3f",wert);break;
    default : sprintf(buf,"%.1f",wert);
  }
  MK3_LCD_PRINT_STRING(buf);
}
#endif


//-----------------------------------------------------------------------------
#if USE_MK3_LCD_BMP_AT==1
void MK3_LCD_BMP_AT(char x,char y,const unsigned char *ptr,int laenge) {
  // anzeigen eines Bildes auf dem Display (aus dem RAM)
  // an x,y Position
  // ptr = pointer auf das Bild-Array
  // laenge = laenge des Bild-Arrays
  //
  // das Bild muss im Format : C-Header-File vertikal (BLV) vorliegen
  // dieses Format kann mit dem Programm : BitmapEdit
  // von Electronic Assembly erstellt werden

  unsigned int v_pos;
  unsigned char maxx,wert,x_pos,y_pos;

  x_pos=1;
  y_pos=y;

  MK3_LCD_SETCURSOR(x,y);

  maxx=ptr[0]; // breite des bildes

  for(v_pos=2;v_pos<laenge;v_pos++) {
    // das ganze Bild-Array durchgehen
    wert=ptr[v_pos];
    MK3_LCD_PRINT_BYTE(wert);
    // naechstes zeichen
    x_pos++;
    if(x_pos>maxx) {
      // naechste Zeile
      x_pos=1;
      y_pos+=8;
      MK3_LCD_SETCURSOR(x,y_pos);
    }
  }
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_BMP_FLASH_AT==1
void MK3_LCD_BMP_FLASH_AT(char x,char y,const unsigned char *ptr,int laenge) {
  // anzeigen eines Bildes auf dem Display (aus dem FLASH)
  // an x,y Position
  // ptr = pointer auf das Bild-Array
  // laenge = laenge des Bild-Arrays
  //
  // das Bild muss im Format : C-Header-File vertikal (BLV) vorliegen
  // dieses Format kann mit dem Programm : BitmapEdit
  // von Electronic Assembly erstellt werden
  // das Bild-Array muss per "UB_PROGMEM" zusatz in den Flash verschoben werden

  unsigned int v_pos;
  unsigned char maxx,wert,x_pos,y_pos;

  x_pos=1;
  y_pos=y;

  MK3_LCD_SETCURSOR(x,y);

  maxx=pgm_read_byte(&ptr[0]); // breite des bildes

  for(v_pos=2;v_pos<laenge;v_pos++) {
    // das ganze Bild-Array durchgehen
    wert=pgm_read_byte(&ptr[v_pos]);
    MK3_LCD_PRINT_BYTE(wert);
    // naechstes zeichen
    x_pos++;
    if(x_pos>maxx) {
      // naechste Zeile
      x_pos=1;
      y_pos+=8;
      MK3_LCD_SETCURSOR(x,y_pos);
    }
  }
}
#endif

//-----------------------------------------------------------------------------
void ub_pause(unsigned int p) { // kleine pause
  unsigned int n;

  for(n=0;n<=p;n++) {
    // kleine Pause
    __asm volatile ("nop \n");  // mini Pause
  }
}

//-----------------------------------------------------------------------------
void lcdSendCmd(char wert) {
  MK3_PORT_LCDC&=~LCD_RS; // sendet Control-Werte
  lcdSend(wert);
}

//-----------------------------------------------------------------------------
void lcdSend(char wert) { // sendet Werte zum Display
  MK3_PORT_LCDC&=~LCD_WR; // Write
  MK3_PORT_LCDD=wert;     // Daten anlegen
  MK3_DDR_LCDD=0xFF;      // Port als Ausgang
  MK3_PORT_LCDC&=~LCD_CS; // CS=enable
  MK3_PORT_LCDC|=LCD_CLK; // CLK=HI
  __asm volatile ("nop \n nop \n nop \n");  // mini Pause
  MK3_PORT_LCDC&=~LCD_CLK;// CLK=LO
  MK3_PORT_LCDC|=LCD_CS;  // CS=disable
}

//-----------------------------------------------------------------------------
char lcdReadData(void) {  // liest ein byte an Cursor-Pos aus
  char wert;

  MK3_DDR_LCDD=0x00;	 // port auf Eingang
  MK3_PORT_LCDC|=LCD_RS; // sendet Daten-Werte
  MK3_PORT_LCDC|=LCD_WR; // mode = read
  MK3_PORT_LCDC&=~LCD_CS; // CS=enable
  MK3_PORT_LCDC|=LCD_CLK; // CLK=HI
  __asm volatile ("nop \n nop \n nop \n");  // mini Pause
  MK3_PORT_LCDC&=~LCD_CLK;// CLK=LO
  MK3_PORT_LCDC|=LCD_CLK; // CLK=HI
  __asm volatile ("nop \n nop \n nop \n");  // mini Pause
  wert=MK3_PIN_LCDD; // daten lesen
  MK3_PORT_LCDC&=~LCD_CLK;// CLK=LO
  MK3_PORT_LCDC|=LCD_CS;  // CS=disable
  MK3_DDR_LCDD=0xFF;	  // port auf ausgang

  return wert;
}




/************************************************************************/
/* Filename: font90_8x5                                                 */
/* Version : 1.01                                                       */
/* Date    : 02.11.2011                                                 */
/* Owner   : UB                                                         */
/*----------------------------------------------------------------------*/
/* Remark  : Font-File zum LCD-Modul                                    */
/*           Schriftgroesse : 8x5 bit                                   */
/*                            wobei ein Zeichen aus 5x7 Pixel besteht   */
/*           Vorsicht : das Array muss fortlaufend sein...              */
/*                      z.B. von Ascii-Zeichen 32d (Space)              */
/*                           bis Ascii-Zeichen 126d (Tilde)             */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  z.B. Ascii 82d = R -> 0x7F,0x09,0x19,0x29,0x46,                     */
/*                                                                      */
/*                  (linke obere Ecke = LSB                             */
/*                                                                      */
/*                       12345 (Spalte)                                 */
/*                                                                      */
/*                  01h  ####:         = 0x7F (Spalte-1)                */
/*                  02h  #:::#         = 0x09 (Spalte-2)                */
/*                  04h  #:::#         = 0x19 (Spalte-3)                */
/*                  08h  ####:         = 0x29 (Spalte-4)                */
/*                  10h  #:#::         = 0x46 (Spalte-5)                */
/*                  20h  #::#:                                          */
/*                  40h  #:::#                                          */
/*                  80h  :::::                                          */
/*                                                                      */
/************************************************************************/

typedef struct {              // def von Font 8x5
  char b0;
  char b1;
  char b2;
  char b3;
  char b4;
}Font8x5_t;


Font8x5_t font8x5[] = {
  {0x00,0x00,0x00,0x00,0x00}, // Space
  {0x00,0x00,0x4F,0x00,0x00}, // !
  {0x00,0x07,0x00,0x07,0x00}, // "
  {0x14,0x7F,0x14,0x7F,0x14}, // #
  {0x24,0x2A,0x7F,0x2A,0x12}, // $
  {0x63,0x13,0x08,0x64,0x63}, // %
  {0x30,0x4E,0x5D,0x26,0x50}, // &
  {0x04,0x02,0x01,0x00,0x00}, // '
  {0x00,0x1C,0x22,0x41,0x00}, // (
  {0x00,0x41,0x22,0x1C,0x00}, // )
  {0x14,0x08,0x3E,0x08,0x14}, // *
  {0x08,0x08,0x3E,0x08,0x08}, // +
  {0xC0,0x30,0x00,0x00,0x00}, // ,
  {0x08,0x08,0x08,0x08,0x08}, // -
  {0x00,0x60,0x60,0x00,0x00}, // .
  {0x60,0x10,0x08,0x04,0x03}, // /
  {0x3E,0x41,0x41,0x3E,0x00}, // 0
  {0x00,0x42,0x7F,0x40,0x00}, // 1
  {0x72,0x49,0x49,0x49,0x46}, // 2
  {0x21,0x41,0x49,0x4D,0x33}, // 3
  {0x18,0x14,0x12,0x7F,0x10}, // 4
  {0x47,0x45,0x45,0x45,0x39}, // 5
  {0x38,0x4C,0x4A,0x49,0x30}, // 6
  {0x01,0x71,0x09,0x05,0x03}, // 7
  {0x36,0x49,0x49,0x49,0x36}, // 8
  {0x06,0x49,0x29,0x19,0x0E}, // 9
  {0x00,0x00,0x44,0x00,0x00}, // :
  {0x40,0x34,0x00,0x00,0x00}, // ;
  {0x08,0x14,0x22,0x41,0x41}, // <
  {0x14,0x14,0x14,0x14,0x14}, // =
  {0x41,0x41,0x22,0x14,0x08}, // >
  {0x02,0x01,0x51,0x09,0x06}, // ?
  {0x3E,0x41,0x5D,0x55,0x5E}, // @
  {0x7E,0x09,0x09,0x09,0x7E}, // A
  {0x7F,0x49,0x49,0x49,0x36}, // B
  {0x3E,0x41,0x41,0x41,0x22}, // C
  {0x7F,0x41,0x41,0x22,0x1C}, // D
  {0x7F,0x49,0x49,0x49,0x41}, // E
  {0x7F,0x09,0x09,0x09,0x01}, // F
  {0x3E,0x41,0x41,0x49,0x7A}, // G
  {0x7F,0x08,0x08,0x08,0x7F}, // H
  {0x00,0x41,0x7F,0x41,0x00}, // I
  {0x20,0x40,0x41,0x3F,0x01}, // J
  {0x7F,0x08,0x14,0x22,0x41}, // K
  {0x7F,0x40,0x40,0x40,0x40}, // L
  {0x7F,0x02,0x0C,0x02,0x7F}, // M
  {0x7F,0x06,0x08,0x30,0x7F}, // N
  {0x3E,0x41,0x41,0x41,0x3E}, // O
  {0x7F,0x09,0x09,0x09,0x06}, // P
  {0x3E,0x41,0x51,0x21,0x5E}, // Q
  {0x7F,0x09,0x19,0x29,0x46}, // R
  {0x46,0x49,0x49,0x49,0x31}, // S
  {0x01,0x01,0x7F,0x01,0x01}, // T
  {0x3F,0x40,0x40,0x40,0x3F}, // U
  {0x1F,0x20,0x40,0x20,0x1F}, // V
  {0x7F,0x20,0x18,0x20,0x7F}, // W
  {0x63,0x14,0x08,0x14,0x63}, // X
  {0x07,0x08,0x78,0x08,0x07}, // Y
  {0x61,0x51,0x49,0x45,0x43}, // Z
  {0x7F,0x41,0x41,0x00,0x00}, // [
  {0x02,0x04,0x08,0x10,0x20}, // Backslash
  {0x00,0x00,0x41,0x41,0x7F}, // ]
  {0x04,0x02,0x01,0x02,0x04}, // ^
  {0x80,0x80,0x80,0x80,0x80}, // _
  {0x00,0x01,0x02,0x04,0x00}, // `
  {0x38,0x44,0x44,0x28,0x7C}, // a
  {0x7F,0x28,0x44,0x44,0x38}, // b
  {0x38,0x44,0x44,0x44,0x00}, // c
  {0x38,0x44,0x44,0x28,0x7F}, // d
  {0x38,0x54,0x54,0x54,0x18}, // e
  {0x00,0x08,0x7E,0x09,0x00}, // f
  {0x18,0xA4,0xA4,0x98,0x7C}, // g
  {0x7F,0x08,0x04,0x04,0x78}, // h
  {0x00,0x44,0x7D,0x40,0x00}, // i
  {0x40,0x80,0x80,0x7D,0x00}, // j
  {0x7F,0x10,0x18,0x24,0x42}, // k
  {0x00,0x41,0x7F,0x40,0x00}, // l
  {0x7C,0x04,0x78,0x04,0x78}, // m
  {0x7C,0x04,0x04,0x78,0x00}, // n
  {0x38,0x44,0x44,0x44,0x38}, // o
  {0xFC,0x18,0x24,0x24,0x18}, // p
  {0x18,0x24,0x24,0x18,0xFC}, // q
  {0x7C,0x08,0x04,0x04,0x04}, // r
  {0x48,0x54,0x54,0x54,0x24}, // s
  {0x00,0x04,0x3F,0x44,0x00}, // t
  {0x3C,0x40,0x40,0x40,0x7C}, // u
  {0x0C,0x30,0x40,0x30,0x0C}, // v
  {0x3C,0x40,0x20,0x40,0x3C}, // w
  {0x44,0x28,0x10,0x28,0x44}, // x
  {0x1C,0xA0,0xA0,0xA0,0x7C}, // y
  {0x44,0x64,0x54,0x4C,0x44}, // z
  {0x08,0x36,0x41,0x00,0x00}, // {
  {0x00,0x00,0x77,0x00,0x00}, // |
  {0x00,0x00,0x41,0x36,0x08}, // }
  {0x10,0x08,0x08,0x08,0x04}  // ~
};

#define LCD_FIRST_CHAR 0x20 // erste Ascii-Nr im Array
#define LCD_LAST_CHAR  0x7E // letzt Ascii-Nr im Array


//-----------------------------------------------------------------------------
void lcdPrintAsc(char wert) {    // schreib ein Ascii-Zeichen an Cursor-Pos
  unsigned char c_pos;

  if((wert<LCD_FIRST_CHAR) || (wert>LCD_LAST_CHAR)) {
    // wenn Ascii-Zeichen nicht erlaubt ist
    wert=LCD_FIRST_CHAR;
  }

  // position im Array berechnen
  c_pos=wert-LCD_FIRST_CHAR;

  // ein komplette Ascii-Zeichen (aus 5Bytes) zeichnen
  MK3_LCD_PRINT_BYTE(font8x5[c_pos].b0);
  MK3_LCD_PRINT_BYTE(font8x5[c_pos].b1);
  MK3_LCD_PRINT_BYTE(font8x5[c_pos].b2);
  MK3_LCD_PRINT_BYTE(font8x5[c_pos].b3);
  MK3_LCD_PRINT_BYTE(font8x5[c_pos].b4);
  // abschliesend noch ein trennungsbyte
  MK3_LCD_PRINT_BYTE(0x00);
}


//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PIXELGENAU==1
void lcdBytePx(char x,char y,char wert) { // schreibt ein Byte (Pixelgenau)
  unsigned char y_off,y_pos;
  unsigned char w1,w2;
  unsigned int maske_16,data_16=0;

  if((x<LCD_MAXX) && (y<LCD_MAXY)) {
    // X-Koordinate
    lcdSendCmd(0x10+(x>>4));
    lcdSendCmd(0x00+(x&0x0F));

    // Test auf Y-Position
    y_off=(y&0x07);
    if((y_off!=0) && (y<(LCD_MAXY-8))) {
      // zwischen zwei Zeilen
      y_pos=(y>>3);
      // Zeile-1 auslesen
      lcdSendCmd(0xB0+y_pos);
      w1=lcdReadData();
      // Zeile-2 auslesen
      lcdSendCmd(0xB0+y_pos+1);
      w2=lcdReadData();
      // 16bit erstellen
      maske_16=(w2<<8)|(w1);
      // maske und daten setzen
      switch(y_off) {
        case 1 : maske_16&=0xFE01;data_16=(wert<<1);break;
        case 2 : maske_16&=0xFC03;data_16=(wert<<2);break;
        case 3 : maske_16&=0xF807;data_16=(wert<<3);break;
        case 4 : maske_16&=0xF00F;data_16=(wert<<4);break;
        case 5 : maske_16&=0xE01F;data_16=(wert<<5);break;
        case 6 : maske_16&=0xC03F;data_16=(wert<<6);break;
        case 7 : maske_16&=0x807F;data_16=(wert<<7);break;
      }
      // neue Daten setzen
      data_16|=maske_16;
      // aufspalten
      w1=(data_16&0x00FF);
      w2=(data_16>>8);
      // Zeile2 schreiben
      // Koordinate stimmt schon
      MK3_LCD_PRINT_BYTE(w2);
      // Zeile1 schreiben
      // X-Koordinate
      lcdSendCmd(0x10+(x>>4));
      lcdSendCmd(0x00+(x&0x0F));
      // Y-Koordinate
      lcdSendCmd(0xB0+y_pos);
      MK3_LCD_PRINT_BYTE(w1);
    }
    else {
      // genau auf einer Zeile

      // Y-Koordinate
      lcdSendCmd(0xB0+(y>>3));

      // Daten schreiben
      MK3_LCD_PRINT_BYTE(wert);
    }
  }
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PIXELGENAU==1
void lcdPrintAscPx(char x,char y,char wert) { // schreib ein Ascii-Zeichen (Pixelgenau)
  unsigned char c_pos;

  if((wert<LCD_FIRST_CHAR) || (wert>LCD_LAST_CHAR)) {
    // wenn Ascii-Zeichen nicht erlaubt ist
    wert=LCD_FIRST_CHAR;
  }

  // position im Array berechnen
  c_pos=wert-LCD_FIRST_CHAR;

  // ein komplette Ascii-Zeichen (aus 5Bytes) zeichnen
  lcdBytePx(x+0,y,font8x5[c_pos].b0);
  lcdBytePx(x+1,y,font8x5[c_pos].b1);
  lcdBytePx(x+2,y,font8x5[c_pos].b2);
  lcdBytePx(x+3,y,font8x5[c_pos].b3);
  lcdBytePx(x+4,y,font8x5[c_pos].b4);
  // abschliesend noch ein trennungsbyte
  lcdBytePx(x+5,y,0x00);
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PIXELGENAU==1
void MK3_LCD_STRING_PX(char x,char y,const char *ptr) { // print Text (Pixelgenau)
  unsigned int n;
  unsigned char c;
  unsigned char x_pos;

  x_pos=x;
  for(n=0;n<=255;n++) {
    c=ptr[n];
    if(c==0) {
      break;
    }
    else {
      lcdPrintAscPx(x_pos,y,c);
	  x_pos+=6;
    }
  }
}
#endif

//-----------------------------------------------------------------------------
#if USE_MK3_LCD_PIXELGENAU==1
void MK3_LCD_UTYP_PX(char x,char y,unsigned int wert,U_ZAHLTYP typ) {
  // anzeigen einer "unsigned" Zahl
  // mit einem bestimmten Formatierung
  // Pixelgenau

  char buf[7];

  switch(typ) {
    case DEZ_U5 : sprintf(buf,"%05u",wert);break;
    case DEZ_U4 : sprintf(buf,"%04u",wert);break;
    case DEZ_U3 : sprintf(buf,"%03u",wert);break;
    case DEZ_U2 : sprintf(buf,"%02u",wert);break;
    case DEZ_U1 : sprintf(buf,"%01u",wert);break;
    case DEZ_U0 : sprintf(buf,"%u",wert);break;
    case HEX_U4 : sprintf(buf,"%04x",wert);break;
    case HEX_U3 : sprintf(buf,"%03x",wert);break;
    case HEX_U2 : sprintf(buf,"%02x",wert);break;
    case HEX_U1 : sprintf(buf,"%01x",wert);break;
    case HEX_U0 : sprintf(buf,"%x",wert);break;
    default : sprintf(buf,"%05u",wert);
  }
  MK3_LCD_STRING_PX(x,y,buf);
}
#endif



//-----------------------------------------------------------------------------
// High-Level-Functions
//-----------------------------------------------------------------------------

void plot(uint8_t x, uint8_t y)
{
    MK3_LCD_PIXEL_AT(x, y, MODE_SET);
}

void drawline(point p0, point p1)
{
    int16_t dx = abs(p1.x - p0.x), sx = p0.x < p1.x ? 1 : -1;
    int16_t dy = -abs(p1.y - p0.y), sy = p0.y < p1.y ? 1 : -1;
    int16_t err = dx + dy, e2; /* error value e_xy */

    for (;;)   /* loop */
    {
        plot(p0.x, p0.y);
        if (p0.x == p1.x && p0.y == p1.y)
        {
            break;
        }

        e2 = 2 * err;
        if (e2 > dy) /* e_xy+e_x > 0 */
        {
            err += dy;
            p0.x += sx;
        }
        if (e2 < dx) /* e_xy+e_y < 0 */
        {
            err += dx;
            p0.y += sy;
        }
    }
}

void toogleLCDLight()
{
	static bool licht = true;
	if (licht)
	{
		licht=false;
		MK3_LCD_LIGHT(licht);
	}
	else
	{
		licht = true;
		MK3_LCD_LIGHT(licht);
	}
}

void toogleLCDDisp()
{
    static bool display = false;
	if (display)
	{
		display=false;
		MK3_LCD_DARK(display);
	}
	else
	{
		display = true;
		MK3_LCD_DARK(display);
    }
}
