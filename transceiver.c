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

/** @file
*
* @defgroup nrf_dev_led_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_led_radio_tx_example
*
* @brief Radio Transmitter Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral. 
*
*/

#include <stdint.h>
#include <stdbool.h>
#include "nrf_gpio.h"
#include "interruptions.h"
#include "transceiver.h"
#include "uicr_config.h"
#include "flash.h"
#include "radio_config.h"


uint32_t *taxa 					= (uint32_t *) TX;
uint32_t *potencia 			= (uint32_t *) PTX;
uint32_t *intervalo 		= (uint32_t *) T_TX;
uint32_t *identidade 		= (uint32_t *) ID;
uint32_t *tx_flag		 		= (uint32_t *) FLAG;
uint32_t *extra			 		= (uint32_t *) EXTRA_TIME;

static uint8_t packet[11];  ///< Packet to transmit

bool volatile interruptor2;

void reconf_info_tag(void){
	
taxa 					= (uint32_t *) TX;
potencia 			= (uint32_t *) PTX;
intervalo 		= (uint32_t *) T_TX;
identidade 		= (uint32_t *) ID;
tx_flag		 		= (uint32_t *) FLAG;
extra			 		= (uint32_t *) EXTRA_TIME;
	
}

	void transceiver_setup(void){
		
		// Atualiza a área de memória apontada pelos ponteiros.
		reconf_info_tag();

		// Set radio configuration parameters
		radio_configure(&taxa, &potencia);
		

		// Set payload pointer
		NRF_RADIO->PACKETPTR = (uint32_t)packet;  
	}

void transmissor(){

		NRF_RADIO->PACKETPTR = (uint32_t)packet;
		
		conversor_int_byte((uint8_t *)&packet, 0, (uint32_t)*identidade);
   
		NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN = 1;
    while (NRF_RADIO->EVENTS_READY == 0U)
    {
    }
    NRF_RADIO->TASKS_START = 1U;
    NRF_RADIO->EVENTS_END = 0U;  
    while(NRF_RADIO->EVENTS_END == 0U)
    {
    }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;
    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }
}

void receptor(){

		uint32_t id_atual;
		uint32_t id_nova;
	
		interruptor2 = true;
	
		NRF_RADIO->PACKETPTR = (uint32_t)packet;
    NRF_RADIO->EVENTS_READY = 0U;
    
		// Enable radio and wait for ready
    NRF_RADIO->TASKS_RXEN = 1U;
	
    while(NRF_RADIO->EVENTS_READY == 0U){
    }
		
    NRF_RADIO->EVENTS_END = 0U;
		
		// Start listening and wait for address received event
    NRF_RADIO->TASKS_START = 1U;
		
		// inicia o RTC1
		NRF_RTC1->TASKS_START = 1;
		
    // Wait for end of packet
    while(NRF_RADIO->EVENTS_END == 0U && interruptor2){
    }
		
		// Desabilita o RTC1
		NRF_RTC1->TASKS_STOP = 1;
		NRF_RTC1->TASKS_CLEAR = 1;
		NRF_RTC1->EVENTS_COMPARE[0] = 0;
		RTC1_disable();
		
    // Write received data to LED0 and LED1 on CRC match
    if (NRF_RADIO->CRCSTATUS == 1U){
			
			id_atual = conversor_byte_int((uint8_t *)&packet, 2);
			id_nova  = conversor_byte_int((uint8_t *)&packet, 6);
			
			if((id_atual == (uint32_t)*identidade) && (((uint32_t)packet[0] != (uint32_t)*potencia) || 
				((uint32_t)packet[1] != (uint32_t)*intervalo) || ((uint32_t)packet[2] != (uint32_t)*intervalo) || 
				(id_atual != id_nova))){
					
						gravar_dados((uint32_t)packet[0],(uint32_t)packet[1],id_nova, packet[10]);
				
			}
			
    }
		
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;
    while(NRF_RADIO->EVENTS_DISABLED == 0U){
    }
		
  }

uint32_t conversor_byte_int(uint8_t buff[], int pos){
	
				uint32_t valor = buff[pos]<<24 | buff[pos + 1]<<16 | buff[pos + 2]<<8 | buff[pos + 3];
				return valor;
	
}

void conversor_int_byte(uint8_t buff[], int pos, uint32_t valor){

				buff[pos] 		= (uint8_t)((valor & 0xFF000000) >> 24 );
				buff[pos + 1] = (uint8_t)((valor & 0x00FF0000) >> 16 );
				buff[pos + 2] = (uint8_t)((valor & 0x0000FF00) >> 8 );
				buff[pos + 3] = (uint8_t)((valor & 0X000000FF));
	
	
}
	
	
		int gravar_dados(uint32_t valor_ptx, uint32_t valor_ttx, uint32_t valor_id, uint32_t valor_flag){
			
			flash_page_erase((uint32_t *)TX);
			
			flash_word_write((uint32_t *)TX, 1);
			flash_word_write((uint32_t *)PTX, valor_ptx);
			flash_word_write((uint32_t *)ID, valor_id);
			flash_word_write((uint32_t *)T_TX, valor_ttx);
			flash_word_write((uint32_t *)FLAG, valor_flag);
			flash_word_write((uint32_t *)EXTRA_TIME, 0x00C350);
			
			
		}
		
void desliga_interruptor2(void){
	
	interruptor2 = false;
	
}
	
/**
 *@}
 **/
