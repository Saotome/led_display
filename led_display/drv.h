/*
 * drv.h
 *
 * Created: 2014/06/22 0:03:52
 *  Author: saotome
 */ 


#ifndef DRV_H_
#define DRV_H_

/*****************************************************************************
 * Header Include                                                            *
 *****************************************************************************/
#include <stdbool.h>

/*****************************************************************************
 * Type definition                                                           *
 *****************************************************************************/
typedef struct
{
	uint8_t x;
	uint8_t y;
}VECTOR;

/*****************************************************************************
 * Macro                                                                     *
 *****************************************************************************/
#define Y_AREA_NUM	(2)
#define BYTE		(8)

#define Y_MAX		(Y_AREA_NUM * BYTE)
#define X_MAX		(1)

/*****************************************************************************
 * Function prototype                                                        *
 *****************************************************************************/
void Initialize(void);
void SetLed(VECTOR, bool);
void SetLedLevel(VECTOR v, uint8_t s);

#endif /* DRV_H_ */