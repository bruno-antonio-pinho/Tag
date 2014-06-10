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
	#include "radio_config.h"
	#include "interruptions.h"
	#include "flash.h"
	#include "transceiver.h"
	#include "nrf_delay.h"
	#include "simple_uart.h"
	#include "boards.h"

	uint32_t *time 		= (uint32_t *) T_TX;
	bool volatile interruptor1 = false;
	
	
	
	void init(void)
	{
		
		/* Inicia o oscilador de 16 MHz */
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_HFCLKSTART = 1;

		/* Espera o oscilador iniciar */
		while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
		{
		}

		/* Inicia o oscilador de 32 KHz */
		NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
    while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);

	}
	
	
	/**
	 * @brief Function for application main entry.
	 * @return 0. int return type required by ANSI/ISO standard.
	 */

	int main(void)
	{
		init();
		do{
			
			// Atualiza a área de memória para o qual time aponta
			time = (uint32_t *) T_TX;
			
			// Garante que a tag fique pelo menos metade do tempo em modo sleep
			if(*time < 0x5){
					
				RTC0_init(0x5);
			
			} else {
				
				RTC0_init(*time);
				
			}
			
			do{
						// Enter System ON sleep mode
						__WFE();  
						// Make sure any pending events are cleared
						__SEV();
 						__WFE();                
							
				}while(!interruptor1);
		
			}while(true);
		
	}
	
void RTC0_IRQHandler(void){
	
    // This handler will be run after wakeup from system ON (RTC wakeup)
    if(NRF_RTC0->EVENTS_COMPARE[0]){
			
					NRF_RTC0->TASKS_STOP = 1;
					NRF_RTC0->TASKS_CLEAR = 1;
					NRF_RTC0->EVENTS_COMPARE[0] = 0;
			
					interruptor1 = true;
					
					// Configura o módulo de radio
					transceiver_setup();
					
					for(int i = 0; i < 3; i++){
						
							transmissor();
					
					}
					
					//Radio_init(); //interrupção do radio atualmente não funciona
					
					RTC1_init();
					receptor();
					
					// disabilita a interrupção do RTC0
					RTC0_disable();
   
		}

}

void RTC1_IRQHandler(void){
	
			if(NRF_RTC1->EVENTS_COMPARE[0]){
			
					NRF_RTC1->TASKS_STOP = 1;
					NRF_RTC1->TASKS_CLEAR = 1;
					NRF_RTC1->EVENTS_COMPARE[0] = 0;
			
					//força a saída da estrutura de repetição de espera da recepção de pacote
					desliga_interruptor2();
					
					//Radio_disable();
					RTC1_disable();

			}
			
}

void RADIO_IRQHandler(void){
	

			
					NRF_RTC1->TASKS_STOP = 1;
					NRF_RTC1->TASKS_CLEAR = 1;
					NRF_RTC1->EVENTS_COMPARE[0] = 0;
				
					RTC1_disable();
					Radio_disable();


			
}
	/**
	 *@}
	 **/
