
#include "adjust.h"
#include "ddc.h"
#include "bsp.h"


AdjustDate_t adjust;

DataFloat_t RxValue;
DataFloat_t RxValueAdjust;

DataFloat_t adc_value;
DataFloat_t adc_value1;
DataFloat_t adc_value2;
DataFloat_t adc_value3;
DataFloat_t adc_voltage;
DataFloat_t adc_voltage1;
DataFloat_t adc_voltage2;
DataFloat_t adc_voltage3;

uint8_t calibrate_flag = 0;
double data1[12][2] = {
//    X      Y
//    {187.1, 25.4},
//    {179.5, 22.8},
//    {157.0, 20.6},
//    {197.0, 21.8},
//    {239.4, 32.4},
//    {217.8, 24.4},
//    {227.1, 29.3},
//    {233.4, 27.9},
//    {242.0, 27.8},
//    {251.9, 34.2},
//    {230.0, 29.2},
//    {271.8, 30.0}
};
double answer[2];
double SquarePoor[4];


void display(double *dat, double *Answer, double *SquarePoor, int rows, int cols)
{
    double v, *p;
    int i, j;
    uart1.printf("回归方程式:    Y = %.5lf", Answer[0]);
    for (i = 1; i < cols; i ++)
        uart1.printf(" + %.5lf*X%d\r\n", Answer[i], i);
    uart1.printf("回归显著性检验: \r\n");
    uart1.printf("回归平方和：%12.4lf  回归方差：%12.4lf \r\n", SquarePoor[0], SquarePoor[2]);
    uart1.printf("剩余平方和：%12.4lf  剩余方差：%12.4lf \r\n", SquarePoor[1], SquarePoor[3]);
    uart1.printf("离差平方和：%12.4lf  标准误差：%12.4lf \r\n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
    uart1.printf("F   检  验：%12.4lf  相关系数：%12.4lf \r\n", SquarePoor[2] /SquarePoor[3],
           sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
    uart1.printf("剩余分析: \r\n");
    uart1.printf("      观察值      估计值      剩余值    剩余平方 \r\n");
    for (i = 0, p = dat; i < rows; i ++, p ++)
    {
        v = Answer[0];
        for (j = 1; j < cols; j ++, p ++)
            v += *p * Answer[j];
        uart1.printf("%12.2lf%12.2lf%12.2lf%12.2lf \r\n", *p, v, *p - v, (*p - v) * (*p - v));
    }
}

void calibrate_para(uint8_t *ptr, uint16_t len )
{
    uint8_t buf[30];
    uint8_t buf2[30];
    uint8_t data2[10];
    DataFloat_t adc;
    DataFloat_t value;
    
    uint8_t rows = len/8;

    

    for(int i = 0; i < rows; i++)
    {
        adc.byte[0] = *ptr++;
        adc.byte[1] = *ptr++;
        adc.byte[2] = *ptr++;
        adc.byte[3] = *ptr++;

        value.byte[0] = *ptr++;
        value.byte[1] = *ptr++;
        value.byte[2] = *ptr++;
        value.byte[3] = *ptr++;
        
        data1[i][0] = (double)adc.value;
        data1[i][1] = (double)value.value;
    }
    linear_regression((double*)data1,rows,&answer[0],&answer[1],&SquarePoor[0]);
    display((double*)data1,answer,&SquarePoor[0],rows,2);
    

    adjust.offsetP.value = answer[0];
    adjust.ratioP.value = answer[1];
    adjust.ccP.value = sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1]));
    
    for(int i = 0 ; i < 4; i++)
    {
        data2[i]= adjust.ratioP.byte[i];
    }
    for(int i = 4 ; i < 8; i++)
    {
        data2[i]= adjust.offsetP.byte[i];
    }    
    ddc_make_frame(buf2,data2,8,DDC_NoAck,11);
    ddc_add_to_list(buf2);
                    ddc_loop();        

    ddc_make_frame(buf,&adjust.ccP.byte[0],4,DDC_NoAck,10);
    ddc_add_to_list(buf);  
                        ddc_loop();        

    PB8.toggle();
//    calibrate_flag = 1;

}

DataFloat_t value_ratio;
DataFloat_t value_offset;
void adjust_rx(uint8_t *ptr, uint16_t len )
{
    uint8_t buf[30];
    uint8_t buf2[30];
    uint8_t data2[10];
    DataFloat_t adc;
    DataFloat_t value;
    uint8_t rows = len/8;

    
    DataFloat_t value_cc;
    if(len == 8)
    {
        adc.byte[0] = *ptr++;
        adc.byte[1] = *ptr++;
        adc.byte[2] = *ptr++;
        adc.byte[3] = *ptr++;
        value.byte[0] = *ptr++;
        value.byte[1] = *ptr++;
        value.byte[2] = *ptr++;
        value.byte[3] = *ptr++;
        if(value.value == 0)
        {
           adjust.ratioR.value = 101;
           adjust.offsetR.value = adc_voltage1.value - adc_voltage2.value;//差值
            //adjust.ccR.value = 100;
        }
    }
    else
    {
        for(int i = 0; i < rows; i++)
        {
            adc.byte[0] = *ptr++;
            adc.byte[1] = *ptr++;
            adc.byte[2] = *ptr++;
            adc.byte[3] = *ptr++;
            value.byte[0] = *ptr++;
            value.byte[1] = *ptr++;
            value.byte[2] = *ptr++;
            value.byte[3] = *ptr++;
            data1[i][0] = (double)adc.value;
            data1[i][1] = (double)value.value;
        }
        linear_regression((double*)data1,len/8,&answer[0],&answer[1],&SquarePoor[0]);
        display((double*)data1,answer,&SquarePoor[0],len/8,2);
        

        adjust.offsetR.value  = answer[0];
        adjust.ratioR.value = answer[1];
        adjust.ccR.value = sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1]));
        
        ddc_make_frame(buf,&value_cc.byte[0],4,DDC_NoAck,10);
        ddc_add_to_list(buf);  
                            ddc_loop();        

    }
        for(int i = 0 ; i < 4; i++)
        {
            data2[i]= value_ratio.byte[i];
        }
        for(int i = 4 ; i < 8; i++)
        {
            data2[i]= value_offset.byte[i];
        }    
        ddc_make_frame(buf2,data2,8,DDC_NoAck,11);
        ddc_add_to_list(buf2);
                    ddc_loop();        

    
    PB9.toggle();

}
AdjustDate_t calibrate()
{
    AdjustDate_t temp;
    uint8_t buf[20];
    uint8_t buf2[20];
    uint8_t data[20];

    uint8_t len;
    uint32_t ajust_timer;
    
    linear_regression((double*)data1,12,&answer[0],&answer[1],&SquarePoor[0]);
    display((double*)data1,answer,&SquarePoor[0],12,2);

    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    PB8.set();
    ddc_attach_chx(1,calibrate_para);
    ddc_attach_chx(2,adjust_rx);
    calibrate_flag = 0;
    while(1)
    {
        if(millis() - ajust_timer > 1000)
        {
            ajust_timer = millis();
            adc_value.value = adc.read_average(ADC_AIN0);
            adc_voltage.value = adc.adc_to_voltage(adc_value.value);
            data[0] = adc_value.byte[0];
            data[1] = adc_value.byte[1];
            data[2] = adc_value.byte[2];
            data[3] = adc_value.byte[3];
            data[4] = adc_voltage.byte[0];
            data[5] = adc_voltage.byte[1];
            data[6] = adc_voltage.byte[2];
            data[7] = adc_voltage.byte[3];
            len = ddc_make_frame(buf,data,8,DDC_NoAck,1);
            ddc_add_to_list(buf);  
                    ddc_loop();        

            adc_value1.value = adc.read_average(ADC_AIN1);
            adc_voltage1.value = adc.adc_to_voltage(adc_value1.value);
            data[0] = adc_value1.byte[0];
            data[1] = adc_value1.byte[1];
            data[2] = adc_value1.byte[2];
            data[3] = adc_value1.byte[3];
            data[4] = adc_voltage1.byte[0];
            data[5] = adc_voltage1.byte[1];
            data[6] = adc_voltage1.byte[2];
            data[7] = adc_voltage1.byte[3];
            len = ddc_make_frame(buf,data,8,DDC_NoAck,2);
            ddc_add_to_list(buf);  
        ddc_loop();        

            adc_value2.value = adc.read_average(ADC_AIN2);
            adc_voltage2.value = adc.adc_to_voltage(adc_value2.value);
            data[0] = adc_value2.byte[0];
            data[1] = adc_value2.byte[1];
            data[2] = adc_value2.byte[2];
            data[3] = adc_value2.byte[3];
            data[4] = adc_voltage2.byte[0];
            data[5] = adc_voltage2.byte[1];
            data[6] = adc_voltage2.byte[2];
            data[7] = adc_voltage2.byte[3];
            len = ddc_make_frame(buf,data,8,DDC_NoAck,3);
            ddc_add_to_list(buf);  
        ddc_loop();        


            adc_value3.value = adc.read_average(ADC_AIN3);
            adc_voltage3.value = adc.adc_to_voltage(adc_value3.value);
            data[0] = adc_value3.byte[0];
            data[1] = adc_value3.byte[1];
            data[2] = adc_value3.byte[2];
            data[3] = adc_value3.byte[3];
            data[4] = adc_voltage3.byte[0];
            data[5] = adc_voltage3.byte[1];
            data[6] = adc_voltage3.byte[2];
            data[7] = adc_voltage3.byte[3];
            len = ddc_make_frame(buf,data,8,DDC_NoAck,4);
            ddc_add_to_list(buf);  
            ddc_loop();        


            RxValue.value = (adc_voltage1.value - adc_voltage2.value )/101;
            RxValueAdjust.value = (adc_voltage1.value - adc_voltage2.value - value_offset.value)/101;
            data[0] = RxValue.byte[0];
            data[1] = RxValue.byte[1];
            data[2] = RxValue.byte[2];
            data[3] = RxValue.byte[3];
            data[4] = RxValueAdjust.byte[0];
            data[5] = RxValueAdjust.byte[1];
            data[6] = RxValueAdjust.byte[2];
            data[7] = RxValueAdjust.byte[3];
            len = ddc_make_frame(buf,data,8,DDC_NoAck,5);
            ddc_add_to_list(buf);
                    ddc_loop();        

            uart1.printf("\r\n====%f====\r\n",(adc_value1.value - adc_value2.value - 12));
        }
       // ddc_loop();        
    }
//    ddc_attach_chx(1,0);
    
    temp.ratioP.value = answer[1];
    temp.offsetP.value = answer[0];
    
    return temp;
}


AdjustDate_t adjust_test()
{
    AdjustDate_t x;

    
    return x;
}

