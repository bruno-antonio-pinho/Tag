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
 
#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H
#define TX                  (0x00007C00)
#define PTX                 (0x00007C04)
#define T_TX                (0x00007C08)
#define ID	                (0x00007C0C)
#define FLAG	              (0x00007C10)
#define EXTRA_TIME          (0x00007C14)


void reconf_info_tag(void);
void transceiver_setup();
void transmissor();
void receptor();
uint32_t conversor_byte_int(uint8_t buff[], int pos);
void conversor_int_byte(uint8_t buff[], int pos, uint32_t valor);
int gravar_dados(uint32_t valor_ptx, uint32_t valor_ttx, uint32_t valor_id, uint32_t valor_flag);
void desliga_interruptor2(void);
#endif
