/**
  ******************************************************************************
  * @file    bsp.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H
#include "ebox.h"
#include "button.h"
#include "w5500.h"
#include "led.h"
#include "mmc_sd.h"
#include "lcd_1.8.h"
#include "w25x16.h"
#include "at24c02.h"
#include "ads1118.h"

extern Led      led1;
extern Led      led2;
extern Button   btn;
extern W5500    w5500;
extern SD       sd;
//extern Lcd      lcd;
extern W25x16   flash;
extern At24c02  eeprom;


extern Ads1118 adc;
extern Flash iflash;

//class Father
//{
//    public:
//    Father(){};

//    virtual bool    begin (uint8_t a)  = 0;

//};

//class Chiled : public Father
//{
//public:
//    Chiled(){};
//    bool    begin (uint8_t a)
//    {
//        uart1.printf("chiled\r\n");
//        return true;
//    }

//};

//class Chiled2 : public Father
//{
//public:
//    Chiled2(){};
//    bool    begin (uint8_t a)
//    {
//        uart1.printf("chiled2\r\n");
//        return true;
//    }

//};

//class Test
//{
//    public :
//        Test(Father *p)
//        {
//            this->p = p;
//        }
//        void begin()
//        {
//            p->begin(1);
//        }
//  Father *p;
//};

#endif

