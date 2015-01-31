#include <avr/io.h>
#include <stdio.h> // sprintf()
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "events.h"
#include "gameoflife.h"
#include "common.h"
#include "MK3_2560_LCD.h"
#include "setup.h"
#include "button.h"
#include "led.h"

static bool** cells;

static uint8_t ANZ_GENERATION = 15;
static int8_t SIZE_FIELD_X=55;
static int8_t SIZE_FIELD_Y=64-8; //64 ist hier maximum!!

static void destroyGameOfLife()
{
    uint8_t i;
    for(i=0; i<SIZE_FIELD_X; i++)
    {
        free(cells[i]);
    }
    free(cells);
    cells=NULL;
}

static bool create_cells()
{
    return (((rand() & 0x1F) + (rand() << 5))<((rand() & 0x1F) + (rand() << 5)));
}

static bool Evaluation(bool** field, char lowerLimit, char upperLimit, unsigned  char x, unsigned char y)
{
    int8_t cellsAlive = 0; //anzahl der lebenden zellen in nachbarschaft
    if (lowerLimit > upperLimit || lowerLimit <= 0 || upperLimit <= 0)
    {
        return false;
    }
    else
    {
        if (field[x][y])
        {
            cellsAlive--;    //falls aktuelle zelle selber lebt "nicht" mitzählen
        }

        char a,b;
        for (a = x - 1; a <= x + 1; a++)
        {
            for (b = y - 1; b <= y + 1; b++)
            {
                if ((a >= 0 && a < SIZE_FIELD_X) && (b >= 0 && b < SIZE_FIELD_Y))   //randproblem wird hier behandelt
                {
                    if (field[(unsigned char)a][(unsigned char)b])
                    {
                        cellsAlive++;
                    }
                }
            }
        }
        if (cellsAlive <= lowerLimit)
        {
            return false;    //zelle stirbt an einsamkeit
        }
        else if (cellsAlive <= upperLimit)
        {
            return true;    //zelle lebt
        }

        return false;    //zelle stirbt an überbevölkerung
    }
}

static void StartGame(void)
{
    // fuelle mit leben und male
    register uint8_t y,x;
    for(y=0; y<SIZE_FIELD_Y; y++)
    {
        for(x=0; x<SIZE_FIELD_X; x++)
        {
            cells[x][y]=create_cells();
            if (cells[x][y])
            {
                MK3_LCD_PIXEL_AT(x,y, MODE_OVERWRITE);
            }
        }
    }

    // lass die zellen leben
    uint8_t gen, i;
    for (gen = 1; gen <= ANZ_GENERATION; gen++)
    {
        bool** nextFieldState = malloc(sizeof(bool*) * SIZE_FIELD_X);

        for(i=0; i<SIZE_FIELD_X; i++)
        {
            nextFieldState[i] = malloc(sizeof(bool)*SIZE_FIELD_Y);
        }

        for (y = 0; y < SIZE_FIELD_Y; y++)           //durchlaufe in y-richtung, beginne bei koordinate 0
        {
            for (x = 0; x < SIZE_FIELD_X; x++)      //durchlaufe in x-richtung, beginne bei koordinate 0
            {
                nextFieldState[x][y] = Evaluation(cells, 3, 6, x, y);         //werte aus ob zelle am leben
            }
        }

        MK3_LCD_CLEAR();
        // print cells
        for(y=0; y<SIZE_FIELD_Y; y++)
        {
            for(x=0; x<SIZE_FIELD_X; x++)
            {
                if (nextFieldState[x][y])
                {
                    MK3_LCD_PIXEL_AT(x,y, MODE_OVERWRITE);
                }
            }
        }
        // delete current cell array
        for(i=0; i<SIZE_FIELD_X; i++)
        {
            free(cells[i]);
        }
        free(cells);

        cells = nextFieldState;
        nextFieldState = NULL;

        //__asm__ __volatile__ ( "nop nop nop nop nop");
        //asm("nop;nop;nop;nop");#
        waitMs(400);
    }

    MK3_LCD_STRING_PX(0,((SIZE_FIELD_Y+2) % 64), "Button3: Hauptmenu");
    while (!get_key_short( 1 << KEY2 ))
    {
    }

    destroyGameOfLife();
}

void GameOfLife()
{
    MK3_LCD_CLEAR();
    //initialisierung des zufallsgenerators
    srand(LEDPORT);   //seed ändern für andere zufallsfolge

    // create matrix
    cells = malloc(sizeof(bool*) * SIZE_FIELD_X);

    uint8_t i;
    for(i=0; i<SIZE_FIELD_X; i++)
    {
        cells[i] = malloc(sizeof(bool)*SIZE_FIELD_Y);
    }

	StartGame();
}

void GameOfLifeSet()
{
	MK3_LCD_CLEAR();
	uint8_t x=55, y=64-8, gen=15;
	char bufStr[22];
	unsigned char * varSize=&x;
	while(!get_key_press(1<<JOYPUSH))
	{
		sprintf(bufStr, "X: %u Y:%u G:%u   ",x,y,gen);

		MK3_LCD_STRING_PX(8,64/2,bufStr);

		if(get_key_state(1<<JOYUP))
		{
			*varSize+=1;
		}
		if(get_key_press(1<<JOYDOWN))
		{
			*varSize-=1;
		}
		if(get_key_press(1<<JOYRIGHT))
		{
			if(varSize==&x)
			{
				varSize=&y;
			}
			else if(varSize==&y)
			{
				varSize=&gen;
			}
		}
		if(get_key_press(1<<JOYLEFT))
		{
			if(varSize==&y)
			{
				varSize=&x;
			}
			else if(varSize==&gen)
			{
				varSize=&y;
			}
		}
	}

    SIZE_FIELD_X = x;
    SIZE_FIELD_Y = y;
    ANZ_GENERATION =gen;
}
