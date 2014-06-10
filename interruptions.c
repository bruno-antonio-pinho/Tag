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
 * @defgroup flashwrite_example_main main.c
 * @{
 * @ingroup flashwrite_example
 *
 * @brief This file contains the source code for a sample application using the Flash Write Application.
 *
 *
 * @image html example_board_setup_a.jpg "Use board setup A for this example."
 */

#include <stdbool.h>
#include <stdint.h>
#include "interruptions.h"
#include "nrf.h"
#include "nrf_delay.h"

/** @brief Function for erasing a page in flash.
 *
 * @param page_address Address of the first word in the page to be erased.
 */
 
 	void RTC0_init(uint32_t temporizador){
		
				
		/* Calibra o oscilador de 32 KHZ */
		NRF_CLOCK->EVENTS_DONE = 0;
    NRF_CLOCK->TASKS_CAL = 1;
    while(NRF_CLOCK->EVENTS_DONE == 0);
		
		/* Configura a interrupção do RTC0 */
		NRF_RTC0->PRESCALER = 0;
    NRF_RTC0->EVTENSET = RTC_EVTEN_COMPARE0_Msk; 
    NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk; 
    NRF_RTC0->CC[0] = (4 * (temporizador));
		
		/* Habilita a interuupção do RTC0 e seta a prioridade como 2 */
		NVIC_EnableIRQ(RTC0_IRQn);
		NVIC_SetPriority(RTC0_IRQn, 2);
		
		/* Inicia RTC0 */
		NRF_RTC0->TASKS_START = 1;

		
}
	
	void RTC1_init(){
		
				
		/* Calibra o oscilador de 32 KHZ */
		NRF_CLOCK->EVENTS_DONE = 0;
    NRF_CLOCK->TASKS_CAL = 1;
    while(NRF_CLOCK->EVENTS_DONE == 0);
	
		/* Configura a interrupção do RTC1 */
		NRF_RTC1->PRESCALER = 0;
    NRF_RTC1->EVTENSET = RTC_EVTEN_COMPARE0_Msk; 
    NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk; 
    NRF_RTC1->CC[0] = 7;
		
		/* Habilita a interuupção do RTC1 e seta a prioridade como 1 */
		NVIC_EnableIRQ(RTC1_IRQn);
		NVIC_SetPriority(RTC1_IRQn, 1);
		
		/* Inicia RTC1 */
		//NRF_RTC1->TASKS_START = 1;
		
}
	
 	void Radio_init(void){
		
		/* Configura a interrupção do Radio */ 
    NRF_RADIO->INTENSET = RADIO_INTENSET_DEVMATCH_Msk; 
		
		/* Habilita a interuupção do Radio e seta a prioridade como 0 */
		NVIC_EnableIRQ(RADIO_IRQn);
		NVIC_SetPriority(RADIO_IRQn, 0);

		
}
	
void RTC0_disable(void){
	
			NRF_RTC0->INTENSET = (RTC_INTENSET_COMPARE0_Disabled << RTC_INTENSET_COMPARE0_Pos);
			NRF_RTC0->EVTENSET = (RTC_EVTEN_COMPARE0_Disabled << RTC_EVTEN_COMPARE0_Pos);
			NRF_RTC0->INTENCLR = (RTC_INTENCLR_COMPARE0_Enabled << RTC_INTENCLR_COMPARE0_Pos);
			NRF_RTC0->EVTENCLR = (RTC_EVTENCLR_COMPARE0_Enabled << RTC_EVTENCLR_COMPARE0_Pos);
	
}

void RTC1_disable(void){
	
			NRF_RTC1->INTENSET = (RTC_INTENSET_COMPARE0_Disabled << RTC_INTENSET_COMPARE0_Pos);
			NRF_RTC1->EVTENSET = (RTC_EVTEN_COMPARE0_Disabled    << RTC_EVTEN_COMPARE0_Pos);
			NRF_RTC1->INTENCLR = (RTC_INTENCLR_COMPARE0_Enabled  << RTC_INTENCLR_COMPARE0_Pos);
			NRF_RTC1->EVTENCLR = (RTC_EVTENCLR_COMPARE0_Enabled  << RTC_EVTENCLR_COMPARE0_Pos);
	
}

void Radio_disable(void){
	
			NRF_RADIO->INTENSET = (RADIO_INTENSET_DEVMATCH_Disabled << RADIO_INTENSET_DEVMATCH_Pos);
			NRF_RADIO->INTENCLR = (RADIO_INTENCLR_DEVMATCH_Enabled  << RADIO_INTENCLR_DEVMATCH_Pos);

	
}




/** @} */
