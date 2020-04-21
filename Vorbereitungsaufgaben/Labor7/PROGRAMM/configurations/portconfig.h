/*
 * portconfig.h
 *
 *  Created on: Apr 18, 2020
 *      Author: R. Grünert
 */

#ifndef CONFIGURATIONS_PORTCONFIG_H_
#define CONFIGURATIONS_PORTCONFIG_H_

/// LEDs for testing
#define LED_DIR P1DIR
#define LED_OUT P1OUT

#define LED_0 4
#define LED_1 3
#define LED_2 2

///////////////////////////////////////////////////////
/// 74HC148 ENCODER

/// port 3 encoder output
#define ENC_DIR P3DIR
#define ENC_IN  P3IN
#define A0      0
#define A1      1
#define A2      2
#define ENC_MASK (1<<A0 | 1 << A1 | 1 << A2)

/// port 2 encoder group select (for interrupt)
#define GS_DIR  P2DIR
#define GS_IN   P2IN
#define GS_SEL  P2SEL
#define GS_IE   P2IE
#define GS_IES  P2IES
#define GS_IFG  P2IFG
#define GS      0

///////////////////////////////////////////////////////
#endif /* CONFIGURATIONS_PORTCONFIG_H_ */
