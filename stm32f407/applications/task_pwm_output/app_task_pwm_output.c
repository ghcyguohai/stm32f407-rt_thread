
#include "rtthread.h"
#include "app_task_pwm_output.h"
#include "drv_pwm_dma_output.h"

#define  TIM_CH_1_OUTPUT_FLAG (1<<0)
#define  TIM_CH_2_OUTPUT_FLAG (1<<1)
#define  TIM_CH_3_OUTPUT_FLAG (1<<2)


static struct rt_event pwm_output_event_set;

static void app_task_pwm_output_init(void)
{
	rt_event_init(&pwm_output_event_set,"PWM_OUTPUT",RT_IPC_FLAG_FIFO);
	tim_pwm_output_init();
}

static void app_task_pwm_output_entry(void *parameter)
{
	rt_uint32_t event_set;
	app_task_pwm_output_init();
  
	
    while (1)
    {
			
      if(rt_event_recv(&pwm_output_event_set,
                       TIM_CH_1_OUTPUT_FLAG|TIM_CH_2_OUTPUT_FLAG|TIM_CH_3_OUTPUT_FLAG,
                       RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,
                       RT_WAITING_FOREVER,
					             &event_set)==RT_EOK)  
      {
         if(event_set&TIM_CH_1_OUTPUT_FLAG)
         {
          // rt_kprintf("rt-thread pwm task TIM_CH_1_OUTPUT_FLAG\r\n");
           TIM_CH1_PWM_Start();	 
         }

         if(event_set&TIM_CH_2_OUTPUT_FLAG)
         {
          // rt_kprintf("rt-thread pwm task TIM_CH_2_OUTPUT_FLAG\r\n");
           TIM_CH2_PWM_Start();
         }

         if(event_set&TIM_CH_3_OUTPUT_FLAG)
         {
          // rt_kprintf("rt-thread pwm task TIM_CH_3_OUTPUT_FLAG\r\n");
           TIM_CH3_PWM_Start();
         }
      } 
    }
}


void tim_ch_1_event_send(void)
{
	rt_event_send(&pwm_output_event_set, TIM_CH_1_OUTPUT_FLAG);
}

void tim_ch_2_event_send(void)
{
  rt_event_send(&pwm_output_event_set, TIM_CH_2_OUTPUT_FLAG);
}

void tim_ch_3_event_send(void)
{
  rt_event_send(&pwm_output_event_set, TIM_CH_3_OUTPUT_FLAG);
}

void app_task_pwm_output_create(void)
{
	rt_thread_t tid;
    tid = rt_thread_create("task_pwm", app_task_pwm_output_entry, RT_NULL,
                           RT_MAIN_THREAD_STACK_SIZE, RT_MAIN_THREAD_PRIORITY, 1);
    RT_ASSERT(tid != RT_NULL);
	 rt_thread_startup(tid);

}
