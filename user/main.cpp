/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
一个简单的命令帧接收示例
*/
#include "ebox.h"
#include "freemodbus.h"
 #include "ebox_analog.h"
#include "ddc.h"
#include "adjust.h"
#include "bsp.h"

#define LED1_ON   PB8.set()
#define LED1_OFF  PB8.reset()

#define LED2_ON   PB9.set()
#define LED2_OFF  PB9.reset()

#define  BUTTON1_READ()  PA8.read()
 
u8 count;
Timer timer4(TIM4);
Timer timer2(TIM2);

AdjustDate_t convert;

void FreeModbusIoConfig(void)
{ 
    modbus.uart=&uart1;
	modbus.timer=&timer4;
	modbus.Mode4851=&PB0;
	modbus.Mode4852=&PB1;
	
	modbus.Mode=MB_RTU;
	modbus.SlaveAdress=0x01;
	modbus.PortId=0x01;
	modbus.BaudRate=115200;
	modbus.Pariby=MB_PAR_NONE;	
  }


void LED_Poll(void)
{
  uint8_t LED_Status;
  LED_Status = ucRegCoilsBuf[0];
  if(LED_Status & 0x01) {LED1_ON;} else {LED1_OFF;}
  if(LED_Status & 0x02) {LED2_ON;} else {LED2_OFF;}
}

void Button_Poll(void)
{
  
  uint8_t Button_Status = 0x00;  
  BUTTON1_READ()?(Button_Status &=~ 0x01):(Button_Status |= 0x01);
 
  ucRegDiscreteBuf[0] = Button_Status;
} 

void Adc_Poll(void)
{ 
    usRegInputBuf[0]=analog_read(&PA0);
	usRegInputBuf[1]=analog_read(&PA1);

	usRegInputBuf[4]=analog_read(&PA4);
	usRegInputBuf[5]=analog_read(&PA5);
	
}
void ddc_input()
{
    ddc_get_char(uart1.read());
}

void setup()
{
	ebox_init();
    
    uart1.begin(115200);
    uart1.attach(ddc_input,RxIrq);
    uart1.interrupt(RxIrq,ENABLE);
    
    ddc_init();
    timer2.begin(100);
    timer2.attach(ddc_loop);
    timer2.interrupt(ENABLE);
    timer2.start();
//	FreeModbusIoConfig();
//	FreemodbusConfig();
//	
//    PB8.mode(OUTPUT_PP);
//    PB9.mode(OUTPUT_PP); 
//    
//    PA8.mode(INPUT);                                                                       
//		
//	PA0.mode(AIN);
//	PA1.mode(AIN);

//	PA4.mode(AIN);
//	PA5.mode(AIN);
    PA5.mode(OUTPUT_PP);
	adc.begin(1);
    uart1.printf("test:%d\r\n",adc.self_test());

    convert = calibrate();
    

}


int main(void)
{
	setup();



	while(1)
	{		 	
//        FreemodbusPoll();
//        LED_Poll();
//        Button_Poll();
//        Adc_Poll();
//        PA5.toggle();
        uart1.printf("===========ADC test==============\r\n");
        uart1.printf("A0-1(hex    ):%d\t%d\r\n",adc.read(AIN0),adc.read(AIN1));
        uart1.printf("A0-1(voltage):%f\t%f\r\n",adc.read_voltage(AIN0),adc.read_voltage(AIN1));

        delay_ms(500);
	}
}

float get_pt100(uint16_t adc0,uint16_t adc1,uint16_t adc2)
{


}


