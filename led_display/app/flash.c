/*
 * flash.c
 *
 * Created: 2014/06/23 22:03:13
 *  Author: saotome
 */ 


/*****************************************************************************
 * Header Include                                                            *
 *****************************************************************************/
#include <avr/io.h>
#include "../drv.h"
#include "../func.h"

/*****************************************************************************
 * public function                                                           *
 *****************************************************************************/
/*----------------------------------------------------------------------------
 * とりあえず点滅するだけのテスト
 * Argument	: -
 * Return	: -
 *---------------------------------------------------------------------------*/
void flash()
{
	uint8_t i;
	
	SetAll(true);
	wait_ms(250);
	SetAll(false);
	wait_ms(250);
}
