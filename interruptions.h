/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
 
#include <stdint.h>
#ifndef INTERRUPTIONS_H
#define INTERRUPTIONS_H

void RTC0_init(uint32_t temporizador);
void RTC1_init(void);
void Radio_init(void);
void RTC0_disable(void);
void RTC1_disable(void);
void Radio_disable(void);

#endif
