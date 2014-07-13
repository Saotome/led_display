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
#include <avr/pgmspace.h>
#define F_CPU 8000000
#include <util/delay.h>

#include "drv.h"

/*****************************************************************************
 * Macro                                                                     *
 *****************************************************************************/
#define Y_AXIS			PORTD
#define Y_AREA			PORTB
#define CHANGE_DELAY    (36)

/*****************************************************************************
 * Type Definition                                                           *
 *****************************************************************************/
union LEVEL{
	uint64_t level64;
	uint8_t  level8[BYTE];
};

/*****************************************************************************
 * Variable                                                                  *
 *****************************************************************************/
static uint8_t pos_x;
static uint8_t area_y;
static uint16_t duty;
static uint8_t led_map[X_MAX][Y_AREA_NUM];
//static union LEVEL led_level[X_MAX][Y_AREA_NUM];

/*const uint64_t LED_LEVEL_MASK[BYTE + 1] PROGMEM = {
	0x0000000000000000,
	0x0100000000000000,
	0x0101000000000000,
	0x0101010000000000,
	0x0101010100000000,
	0x0101010101000000,
	0x0101010101010000,
	0x0101010101010100,
	0x0101010101010101
};*/

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
			led_map[i][j]   = 0;
			//led_level[i][j].level64 = 0;
		}
	}

	pos_x = 0;
	area_y = 0;
	//duty = 0;
	
	// Register settings
	// timer
	TCNT0 = 0;				// Timer 0 Initialize
	TCCR0B = 2;				// Clock Select clk/8
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
	if(v.x < X_MAX && v.y < Y_MAX && s <= BYTE)
	{
		//led_level[v.x][v.y / BYTE].level64 |=  (LED_LEVEL_MASK[BYTE] << (v.y % BYTE) );
		//led_level[v.x][v.y / BYTE].level64 &= ~(LED_LEVEL_MASK[8]    << (v.y % BYTE) );
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
	Y_AXIS = 0xff;
	
	//現状のハードがx=1しかないので、とりあえずx=0にて点灯させる。
	if(pos_x == 0) {
		Y_AREA = area_y / X_MAX;
		_delay_us(CHANGE_DELAY);
		Y_AXIS = led_map[pos_x][area_y / X_MAX];
		//	   | led_level[pos_x / (Y_AREA_NUM)][area_y].level8[duty / (X_MAX * Y_AREA_NUM)];
	} else {
		Y_AREA = area_y/ X_MAX;
		_delay_us(CHANGE_DELAY);
		Y_AXIS = 0xff;
	}
	
	area_y++;
	pos_x++;
	pos_x  %= X_MAX;
	area_y %= X_MAX * Y_AREA_NUM;
	//duty = count % (Y_AREA_NUM * X_MAX * BYTE);
}
