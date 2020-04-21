/*
 * timerconfig.h
 *
 *  Created on: Apr 6, 2020
 *      Author: R. Grünert
 */

#ifndef CONFIGURATIONS_TIMERCONFIG_H_
#define CONFIGURATIONS_TIMERCONFIG_H_

#include <msp430.h>

// TIMER A0 CONFIGURATION
#define TA0_CLK_SOURCE        TASSEL__ACLK // ACLK
#define TA0_CLK_DIVIDE_ID_VAL 1
#define TA0_CLK_DIVIDE_ID     ID_0

// TIMER A1 CONFIGURATION, used for delays
#define TA1_CLK_SOURCE        TASSEL__ACLK
#define TA1_CLK_DIVIDE_ID     ID_0


#endif /* CONFIGURATIONS_TIMERCONFIG_H_ */
