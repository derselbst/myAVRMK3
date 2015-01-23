#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

typedef struct
{
       uint8_t x;
       uint8_t y;
} point;

void drawline(point p0, point p1);
void toogleLCDDisp();
void toogleLCDLight();


#endif // LCD_H_INCLUDED
