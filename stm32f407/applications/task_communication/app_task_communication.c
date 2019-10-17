

#include "rtthread.h"
#include "usb_device.h"

#include "app_task_communication.h"


#define  SAMPLE_UART_NAME "uart2"/*串口设备名称*/
#define  RX_BUFFER_MAXSIZE 2048


#define  UART2_RX_FLAG (1<<0)
#define  UART2_TX_FLAG (1<<1)

#define  USB_RX_FLAG   (1<<6)
#define  USB_TX_FLAG   (1<<7)



static struct rt_event communication_event_set;

static rt_device_t serial;/*串口设备句柄*/
static rt_device_t usb_cdc;/*串口设备句柄*/
static rt_device_t pin;/*串口设备句柄*/

static uint8_t rx_buffer[RX_BUFFER_MAXSIZE]={0};
static int32_t rx_len=0;


static void     app_task_communication_init(void);
static rt_err_t uart_input(rt_device_t dev, rt_size_t size);
static rt_err_t uart_output(rt_device_t dev, void* buffer);

static rt_err_t usb_input(rt_device_t dev,rt_size_t size);
static rt_err_t usb_output(rt_device_t dev, void* buffer);

static void     app_task_communication_entry(void *parameter);

static void app_task_communication_init(void)
{
   rt_event_init(&communication_event_set,"serial_com",RT_IPC_FLAG_FIFO);
   serial = rt_device_find(SAMPLE_UART_NAME);
	usb_cdc = rt_device_find("vcom");
	
  rt_device_open(usb_cdc, RT_DEVICE_FLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
  rt_device_set_rx_indicate(usb_cdc, usb_input);
  rt_device_set_tx_complete(usb_cdc, usb_output);

//rt_device_set_rx_indicate(serial, uart_input);
//rt_device_set_tx_complete(serial,uart_output);
//rt_device_open(serial,RT_DEVICE_FLAG_DMA_RX|RT_DEVICE_FLAG_DMA_TX);
}

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */

  // rt_event_send(&communication_event_set, UART2_RX_FLAG);
   rt_event_send(&communication_event_set, UART2_TX_FLAG);
   return RT_EOK;
}

static rt_err_t uart_output(rt_device_t dev, void* buffer)
{


    return RT_EOK;
}

static rt_err_t usb_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
	  rx_len=0;
	  if(size>RX_BUFFER_MAXSIZE)
		{
			rx_len=RX_BUFFER_MAXSIZE;
		}
		rx_len=size;
//	rt_kprintf("cdc rx_counter=%d\r\n",rx_len);
    rt_device_read(usb_cdc,0,rx_buffer,rx_len);
    rt_device_write(usb_cdc,0,rx_buffer,rx_len);
    rt_event_send(&communication_event_set, USB_RX_FLAG);
   return RT_EOK;
}

static rt_err_t usb_output(rt_device_t dev, void* buffer)
{


    return RT_EOK;
}


static void app_task_communication_entry(void *parameter)
{
   
    rt_uint32_t event_set;
		app_task_communication_init();
    char* rx_singal="recieved from rx\r\n";
    char* tx_singal="usb recieved from tx\r\n";
    while (1)
    {

        if(rt_event_recv(&communication_event_set,
                         UART2_RX_FLAG|UART2_TX_FLAG|USB_RX_FLAG,
                         RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,
                         RT_WAITING_FOREVER,
												 &event_set)==RT_EOK)  
        {
           if(event_set&UART2_RX_FLAG)
           {
             rt_device_write(serial, 0,rx_singal, 18); 
           }
           else if(event_set&UART2_TX_FLAG)
           {
             rt_device_write(serial, 0,tx_singal, 18);
           }
           else if(event_set&USB_RX_FLAG)
           {
						// rt_device_write(usb_cdc,0,rx_buffer,rx_len);
           // rt_device_write(usb_cdc,0,rx_buffer,rx_len);
           }
        } 
    }
}

void app_task_communication_create(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("task_com", app_task_communication_entry, RT_NULL,
                           RT_MAIN_THREAD_STACK_SIZE, RT_MAIN_THREAD_PRIORITY, 1);
    RT_ASSERT(tid != RT_NULL);
	  rt_thread_startup(tid);
}


