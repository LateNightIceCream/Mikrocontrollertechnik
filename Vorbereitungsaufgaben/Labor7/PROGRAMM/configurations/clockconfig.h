/*
 * clockconfig.h
 *
 *  Created on: Apr 6, 2020
 *      Author: R. Grünert
 */

#ifndef CONFIGURATIONS_CLOCKCONFIG_H_
#define CONFIGURATIONS_CLOCKCONFIG_H_

#include <msp430.h>

/// MCLK CLOCK PARAMETERS
///////////////////////////////////////////////////////

#define MCLK_SPEED_IN_HZ  5000000
#define MCLK_SPEED_IN_KHZ MCLK_SPEED_IN_HZ / 1000
#define REF_SPEED_IN_KHZ  32        // default (XT1)
#define CLK_RATIO         MCLK_SPEED_IN_KHZ / REF_SPEED_IN_KHZ


/// ACLK CLOCK PARAMETERS
///////////////////////////////////////////////////////

#define ACLK_DIVIDER      1
#define ACLK_SPEED_IN_HZ  MCLK_SPEED_IN_HZ / ACLK_DIVIDER
#define ACLK_SPEED_IN_KHZ ACLK_SPEED_IN_HZ / 1000


#endif /* CONFIGURATIONS_CLOCKCONFIG_H_ */
