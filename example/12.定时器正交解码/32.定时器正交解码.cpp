/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "math.h"
#include "ebox_encoder.h"
Encoder encoder(TIM4,&PB6,&PB7);
float x;
uint16_t y;
Pwm pwm1(&PA0);
void setup()
{
    ebox_init();
    uart1.begin(115200);
    encoder.begin(3);
    pwm1.begin(1000, 1000);
    pwm1.set_oc_polarity(1);//set output polarity after compare
    uart1.printf("max frq = %dKhz\r\n",pwm1.get_max_frq()/1000);
    uart1.printf("max frq = %f\r\n",pwm1.get_accuracy());
}
int main(void)
{
    setup();
    uint16_t temp;
    float speed;
    while(1)
    {
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2500 - (sin(x) + 1) * 1000;
        pwm1.set_duty(y);
        speed = encoder.read_speed()/50;
        uart1.printf("count :%0.2f(%d)\r\n",speed,encoder.read_direction());
        delay_ms(100);
        
    }
}




