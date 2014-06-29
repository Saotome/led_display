/*
 * drv.c
 *
 * Created: 2014/06/22 0:03:32
 *  Author: saotome
 */ 


/*****************************************************************************
 * Header Include                                                            *
 *****************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "drv.h"

/*****************************************************************************
 * Macro                                                                     *
 *****************************************************************************/
#define Y_AXIS			PORTD
#define Y_AREA			PORTB

/*****************************************************************************
 * Variable                                                                  *
 *****************************************************************************/
static uint8_t pos_x;
static uint8_t area_y;
static uint16_t duty;
static uint8_t led_map[X_MAX][Y_AREA_NUM];
static uint8_t led_level[X_MAX][Y_AREA_NUM][BYTE];

/*****************************************************************************
 * Function prototype                                                        *
 *****************************************************************************/
static void LightLed(void);

/*****************************************************************************
 * public function                                                           *
 *****************************************************************************/
/*----------------------------------------------------------------------------
 * Initialize
 * Argument	: -
 * Return	: -
 *---------------------------------------------------------------------------*/
void Initialize()
{
	uint8_t i, j, k;
		
	for(i = 0; i < X_MAX; i++) {
		for( j = 0; j < Y_AREA_NUM; j++) {
			led_map[i][j] = 0;
			for(k = 0; k < BYTE; k++) {
				led_level[i][j][k] = 0;
			}
		}
	}
	
	pos_x = 0;
	area_y = 0;
	duty = 0;
	
	// Register settings
	// timer
	TCNT0 = 0;				// Timer 0 Initialize
	TCCR0B = 1;				// Clock Select ck/1
	TIMSK0 = _BV(TOIE0);	// Over Flow Interrupt enable
	
	// output
	Y_AXIS = 0xff;			// PORTD Initialize
	DDRD = 0xff;			// bit 0-7 set to Output
	
	PORTB = 0x00;
	DDRB = 0x01;
	
	sei();
}

/*----------------------------------------------------------------------------
 * 
 * Argument	: VECTOR v
 *          : bool	 s
 * Return	: -
 *---------------------------------------------------------------------------*/
ISR(TIMER0_OVF_vect)
{
	LightLed();
}

/*----------------------------------------------------------------------------
 * 
 * Argument	: VECTOR v
 *          : bool	 s
 * Return	: -
 *---------------------------------------------------------------------------*/
void SetLed(VECTOR v, bool s)
{
	if(v.x < X_MAX && v.y < Y_MAX)
	{
		// 1:Light ON, 0:Light OFF
		if(s)
		{
			led_map[v.x][v.y / BYTE] &= ~_BV(v.y % BYTE);
		}
		else
		{
			led_map[v.x][v.y / BYTE] |= _BV(v.y % BYTE);
		}
	}
}

/*----------------------------------------------------------------------------
 * 
 * Argument	: VECTOR v
 *          : uint8_t s
 * Return	: -
 *---------------------------------------------------------------------------*/
void SetLedLevel(VECTOR v, uint8_t s)
{
	uint8_t i;
	
	if(v.x < X_MAX && v.y < Y_MAX)
	{
		for(i = 1; i <= BYTE; i++) {
			if(i <= s) {
				led_level[v.x][v.y / BYTE][i-1] &= ~_BV(v.y % BYTE);
			} else {
				led_level[v.x][v.y / BYTE][i-1] |= _BV(v.y % BYTE);
			}
		}
	}
}

/*****************************************************************************
 * private function                                                          *
 *****************************************************************************/
/*----------------------------------------------------------------------------
 * 
 * Argument	: -
 * Return	: -
 *---------------------------------------------------------------------------*/
static void LightLed()
{
	//現状のハードがx=1しかないので、とりあえずx=0 or 1にて点灯させる。
	if(pos_x < Y_AREA_NUM) {
		Y_AREA = area_y;
		Y_AXIS = led_map[pos_x / (Y_AREA_NUM)][area_y]
			   | led_level[pos_x / (Y_AREA_NUM)][area_y][duty / (X_MAX * Y_AREA_NUM)];
	} else {
		Y_AREA = area_y;
		Y_AXIS = 0xff;
	}
	
	area_y++;
	area_y %= Y_AREA_NUM;
	pos_x++;
	pos_x  %= Y_AREA_NUM * X_MAX;
	duty++;
	duty   %= Y_AREA_NUM * X_MAX * BYTE;
}
