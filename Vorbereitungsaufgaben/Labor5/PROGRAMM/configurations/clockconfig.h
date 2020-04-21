/*
 * clockconfig.h
 *
 *  Created on: Apr 6, 2020
 *      Author: zamza
 */

#ifndef CONFIGURATIONS_CLOCKCONFIG_H_
#define CONFIGURATIONS_CLOCKCONFIG_H_

#include <msp430.h>

#define MCLK_SPEED_IN_HZ  1000000
#define MCLK_SPEED_IN_KHZ MCLK_SPEED_IN_HZ / 1000
#define REF_SPEED_IN_KHZ  32        // default XT1
#define CLK_RATIO         MCLK_SPEED_IN_KHZ / REF_SPEED_IN_KHZ

#define ACLK_DIVIDER      2
#define ACLK_SPEED_IN_KHZ MCLK_SPEED_IN_KHZ / ACLK_DIVIDER


#endif /* CONFIGURATIONS_CLOCKCONFIG_H_ */
