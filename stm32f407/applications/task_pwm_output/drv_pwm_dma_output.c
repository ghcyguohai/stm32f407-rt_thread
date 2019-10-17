#include "drv_pwm_dma_output.h"
#include "main.h"
#define   PWM_OUTPUT_TEST_DATA_SIZE  1000
#define   PWM_OUTPUT_PULSE_DEFAULT   50
#define   PWM_OUTPUT_PEROID_DEFAULT  50

TIM_HandleTypeDef Tim_Handle;
DMA_HandleTypeDef Tim_ch1_pwm_dma_handle;
DMA_HandleTypeDef Tim_ch2_pwm_dma_handle;
DMA_HandleTypeDef Tim_ch3_pwm_dma_handle;



extern void led_toggle(void);

static uint32_t pwm_test_array[PWM_OUTPUT_TEST_DATA_SIZE+1]={0};

void pwm_array_init(void);

extern void tim_ch_1_event_send(void);


void pwm_array_init(void)
{
	for (int i = 0; i < PWM_OUTPUT_TEST_DATA_SIZE-1; i++)
	{
		pwm_test_array[i]=25-1;
	}

}

static void tim_ch1_pwm_dma_init(void)
{

  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

  Tim_ch1_pwm_dma_handle.Init.Channel = DMA_CHANNEL_6;                     /* DMA_CHANNEL_0                    */
  Tim_ch1_pwm_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;          /* M2M transfer mode                */
  Tim_ch1_pwm_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* Peripheral increment mode Enable */
  Tim_ch1_pwm_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                  /* Memory increment mode Enable     */
  Tim_ch1_pwm_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD; /* Peripheral data alignment : Word */
  Tim_ch1_pwm_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;    /* memory data alignment : Word     */
  Tim_ch1_pwm_dma_handle.Init.Mode = DMA_NORMAL;                         /* Normal DMA mode                  */
  Tim_ch1_pwm_dma_handle.Init.Priority = DMA_PRIORITY_HIGH;              /* priority level : high            */
  Tim_ch1_pwm_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* FIFO mode enabled                */
  Tim_ch1_pwm_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; /* FIFO threshold: 1/4 full   */
  Tim_ch1_pwm_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;              /* Memory burst                     */
  Tim_ch1_pwm_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;           /* Peripheral burst                 */
  Tim_ch1_pwm_dma_handle.Instance=DMA1_Stream2;
 
  HAL_DMA_Init(&Tim_ch1_pwm_dma_handle);
}

static void tim_ch2_pwm_dma_init(void)
{

  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

  Tim_ch2_pwm_dma_handle.Init.Channel = DMA_CHANNEL_6;                     /* DMA_CHANNEL_0                    */
  Tim_ch2_pwm_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;          /* M2M transfer mode                */
  Tim_ch2_pwm_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* Peripheral increment mode Enable */
  Tim_ch2_pwm_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                  /* Memory increment mode Enable     */
  Tim_ch2_pwm_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD; /* Peripheral data alignment : Word */
  Tim_ch2_pwm_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;    /* memory data alignment : Word     */
  Tim_ch2_pwm_dma_handle.Init.Mode = DMA_NORMAL;                         /* Normal DMA mode                  */
  Tim_ch2_pwm_dma_handle.Init.Priority = DMA_PRIORITY_HIGH;              /* priority level : high            */
  Tim_ch2_pwm_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* FIFO mode enabled                */
  Tim_ch2_pwm_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; /* FIFO threshold: 1/4 full   */
  Tim_ch2_pwm_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;              /* Memory burst                     */
  Tim_ch2_pwm_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;           /* Peripheral burst                 */
  Tim_ch2_pwm_dma_handle.Instance=DMA1_Stream4;
 
  HAL_DMA_Init(&Tim_ch2_pwm_dma_handle);
}

static void tim_ch3_pwm_dma_init(void)
{

  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

  Tim_ch3_pwm_dma_handle.Init.Channel = DMA_CHANNEL_6;                     /* DMA_CHANNEL_0                    */
  Tim_ch3_pwm_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;          /* M2M transfer mode                */
  Tim_ch3_pwm_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* Peripheral increment mode Enable */
  Tim_ch3_pwm_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                  /* Memory increment mode Enable     */
  Tim_ch3_pwm_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD; /* Peripheral data alignment : Word */
  Tim_ch3_pwm_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;    /* memory data alignment : Word     */
  Tim_ch3_pwm_dma_handle.Init.Mode = DMA_NORMAL;                         /* Normal DMA mode                  */
  Tim_ch3_pwm_dma_handle.Init.Priority = DMA_PRIORITY_HIGH;              /* priority level : high            */
  Tim_ch3_pwm_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* FIFO mode enabled                */
  Tim_ch3_pwm_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; /* FIFO threshold: 1/4 full   */
  Tim_ch3_pwm_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;              /* Memory burst                     */
  Tim_ch3_pwm_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;           /* Peripheral burst                 */
  Tim_ch3_pwm_dma_handle.Instance=DMA1_Stream0;
 
  HAL_DMA_Init(&Tim_ch3_pwm_dma_handle);
}


void tim_pwm_output_init(void)
{
	

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	TIM_OC_InitTypeDef TIM_OC_InitStruct;

	pwm_array_init();
	
  __HAL_RCC_TIM5_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  
	//rt_kprintf("HAL_RCC_GetSysClockFreq=%d\r\n",HAL_RCC_GetSysClockFreq());
	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM5_IRQn);
	


  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 	GPIO_InitStruct.Pin = GPIO_PIN_1;
 	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 	GPIO_InitStruct.Pin = GPIO_PIN_2;
 	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  Tim_Handle.Instance = TIM5;
  Tim_Handle.Init.Period        = PWM_OUTPUT_PEROID_DEFAULT-1;

  Tim_Handle.Init.Prescaler     = (uint32_t)84-1;
  Tim_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  Tim_Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
  Tim_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	Tim_Handle.Init.RepetitionCounter=0;
  HAL_TIM_PWM_Init(&Tim_Handle);

  
 	/*##-2- Configure the Output Compare channels ##############################*/ 
 	/* Common configuration for all channels */
  TIM_OC_InitStruct.OCMode  = TIM_OCMODE_PWM1;
	TIM_OC_InitStruct.OCFastMode  = TIM_OCFAST_ENABLE;
 	TIM_OC_InitStruct.OCPolarity  = TIM_OCPOLARITY_HIGH;
	TIM_OC_InitStruct.OCNPolarity = TIM_OCPOLARITY_LOW;
	TIM_OC_InitStruct.OCIdleState =TIM_OCIDLESTATE_RESET;
	TIM_OC_InitStruct.OCNIdleState=TIM_OCNIDLESTATE_RESET;
 	TIM_OC_InitStruct.Pulse = PWM_OUTPUT_PULSE_DEFAULT-1;  

	HAL_TIM_PWM_ConfigChannel(&Tim_Handle,&TIM_OC_InitStruct,TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&Tim_Handle,&TIM_OC_InitStruct,TIM_CHANNEL_2);
  HAL_TIM_PWM_ConfigChannel(&Tim_Handle,&TIM_OC_InitStruct,TIM_CHANNEL_3);
  Tim_Handle.hdma[TIM_DMA_ID_CC1]=&Tim_ch1_pwm_dma_handle;
  Tim_Handle.hdma[TIM_DMA_ID_CC2]=&Tim_ch2_pwm_dma_handle;
  Tim_Handle.hdma[TIM_DMA_ID_CC3]=&Tim_ch3_pwm_dma_handle;
  tim_ch1_pwm_dma_init();
  tim_ch2_pwm_dma_init();
  tim_ch3_pwm_dma_init();
	HAL_TIM_PWM_Start(&Tim_Handle,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&Tim_Handle,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&Tim_Handle,TIM_CHANNEL_3);
}


void TIM_CH1_PWM_Start(void)
{
	HAL_TIM_PWM_Start_DMA(&Tim_Handle,TIM_CHANNEL_1,(uint32_t*)pwm_test_array,PWM_OUTPUT_TEST_DATA_SIZE);
	Tim_Handle.State=HAL_TIM_STATE_READY;
}

void TIM_CH2_PWM_Start(void)
{
  HAL_TIM_PWM_Start_DMA(&Tim_Handle,TIM_CHANNEL_2,(uint32_t*)pwm_test_array,PWM_OUTPUT_TEST_DATA_SIZE);
  Tim_Handle.State=HAL_TIM_STATE_READY;
}

void TIM_CH3_PWM_Start(void)
{
  HAL_TIM_PWM_Start_DMA(&Tim_Handle,TIM_CHANNEL_3,(uint32_t*)pwm_test_array,PWM_OUTPUT_TEST_DATA_SIZE);
  Tim_Handle.State=HAL_TIM_STATE_READY;
}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	led_toggle();
}


void DMA1_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Tim_Handle.hdma[TIM_DMA_ID_CC3]);
}


void DMA1_Stream2_IRQHandler(void)
{
	
	HAL_DMA_IRQHandler(Tim_Handle.hdma[TIM_DMA_ID_CC1]);
}


void DMA1_Stream4_IRQHandler(void)
{
  
  HAL_DMA_IRQHandler(Tim_Handle.hdma[TIM_DMA_ID_CC2]);
}
