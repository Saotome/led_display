/*
 * led_display.c
 *
 * Created: 2014/06/22 0:02:16
 *  Author: saotome
 */ 

/*****************************************************************************
 * Header Include                                                            *
 *****************************************************************************/
#include <avr/io.h>
#include "drv.h"
#include "app/app.h"

int main(void)
{
	Initialize();
	
    while(1)
    {
		flash();
		relay();
        //TODO:: Please write your application code 
    }
}