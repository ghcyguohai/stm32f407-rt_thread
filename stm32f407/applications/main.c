/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "app_task_communication.h"
#include "drv_pwm_dma_output.h"

/* defined the LED0 pin: PF9 */
#define LED0_PIN    GET_PIN(F, 9)

int main(void)
{
	  uint32_t count=0;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	  
	  app_task_pwm_output_create();
	 
		app_task_communication_create();
    while (++count)
    {
        //rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(60); 
		    tim_ch_1_event_send();
        tim_ch_2_event_send();
        tim_ch_3_event_send();	 
        //rt_kprintf("rt-thread is running\r\n");
       // rt_pin_write(LED0_PIN, PIN_LOW);
    }
		
	return 1;
}

void led_toggle(void)
{

 static uint32_t flag=0;

 if(flag==0)
 {
    rt_pin_write(LED0_PIN,PIN_HIGH);
    flag=1;
 }
 else
 {
    rt_pin_write(LED0_PIN,PIN_LOW);
    flag=0;
 }
}