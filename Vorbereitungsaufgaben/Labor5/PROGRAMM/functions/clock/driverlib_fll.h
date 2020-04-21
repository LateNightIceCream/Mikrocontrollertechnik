/*
 * driverlib_fll.h
 *
 *  Created on: Apr 6, 2020
 *      Author: R.G.
 *
 *  Variation of driverlib function since I only need FLL functionality
 */

#ifndef DRIVERLIB_FLL_H_
#define DRIVERLIB_FLL_H_

#include <stdint.h>
#include <msp430f5529.h>

// Not used: Default divider and XT1 default ref clock
//void UCS_initClockSignal();

// names are the same as in driverlib
void UCS_initFLLSettle(uint16_t fsystem, uint16_t ratio);
void UCS_initFLL(uint16_t fsystem, uint16_t ratio);


#endif /* DRIVERLIB_FLL_H_ */
