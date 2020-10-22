/*
 * Copyright (c) 2020 Fingerprint Cards AB
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Modified by Andrey Perminov <andrey.ppp@gmail.com> 
 * for FPC BM-Lite applications
 */

/**
 * @file    platform_timebase.c
 * @brief   timebase interface functions
 */


#include <stdint.h>

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "bmlite_hal.h"



static volatile uint32_t systick;
volatile uint32_t button_pressed_time = 0;

static uint32_t btn_press_start;
static uint32_t btn_pressed = 0;

#define BMLITE_BUTTON 3

static void check_buttons();

/**
 * @brief Handler for timer events.
 */
void TIM6_DAC_IRQHandler()
{
			TIM6->SR &= ~ TIM_SR_UIF;
			/*if (SPI1->SR & SPI_SR_TXE){
				GPIOA->BRR = 0x10;
				SPI1->DR = 0xff;
			}
			while(!(SPI1->SR & SPI_SR_TXE))
				;;
			GPIOA->BSRR = 0x10;
			*/
            systick++;
            //check_buttons();
            return;
}

void hal_timebase_init(void)
{
    /*uint32_t time_ms = 1; //Time (in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_LED, &timer_cfg, timer_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_LED, time_ms);

    nrf_drv_timer_extended_compare(
         &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER_LED);*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->PSC = 47;
	TIM6->ARR = 999;
	TIM6->DIER |= TIM_DIER_UIE;
	TIM6->CR1 |= TIM_CR1_CEN;
	NVIC->ISER[0] = (1 << 17);
}

void hal_timebase_busy_wait(uint32_t delay)
{
    uint32_t start;
    uint32_t delay_internal = 0;

    /* Ensure minimum delay or skip if delay is zero*/
    if (delay) {
        delay_internal = delay + 1;
        start = systick;
        while ((systick - start) < delay_internal) {
        }
    }
}

uint32_t hal_timebase_get_tick(void)
{
    return systick;
}

static void check_buttons()
{
    /*if (bsp_board_button_state_get(BMLITE_BUTTON)) {
        if (btn_pressed == 0) {
            btn_press_start = systick;
            btn_pressed = 1;
        }
    } else { // Btn released
        if (btn_pressed) {
            if (systick > btn_press_start) {
                button_pressed_time = systick - btn_press_start;
            } else {
                button_pressed_time = systick + ~btn_press_start + 1;
            }
            btn_pressed = 0;
        }
    }*/
}

uint32_t hal_get_button_press_time()
{
    uint32_t time = button_pressed_time;
    button_pressed_time = 0;
    return time;
}

uint32_t hal_check_button_pressed()
{
    uint32_t time = button_pressed_time;
    return time;
}
