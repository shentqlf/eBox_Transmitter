
#include "adjust.h"
#include "ddc.h"
#include "bsp.h"

uint8_t calibrate_flag = 0;
double data1[12][2] = {
//    X      Y
    {187.1, 25.4},
    {179.5, 22.8},
    {157.0, 20.6},
    {197.0, 21.8},
    {239.4, 32.4},
    {217.8, 24.4},
    {227.1, 29.3},
    {233.4, 27.9},
    {242.0, 27.8},
    {251.9, 34.2},
    {230.0, 29.2},
    {271.8, 30.0}
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
    DataU16_t adc;
    DataFloat_t value;
    for(int i = 0; i < len/6; i++)
    {
        adc.byte[0] = *ptr++;
        adc.byte[1] = *ptr++;
        value.byte[0] = *ptr++;
        value.byte[1] = *ptr++;
        value.byte[2] = *ptr++;
        value.byte[3] = *ptr++;
        data1[i/6][0] = (double)adc.value;
        data1[i/6][1] = (double)value.value;
    }
    linear_regression((double*)data1,12,&answer[0],&answer[1],&SquarePoor[0]);
    display((double*)data1,answer,&SquarePoor[0],12,2);
    calibrate_flag = 1;
}
AdjustDate_t calibrate()
{
    AdjustDate_t temp;
    uint8_t buf[20];
    DataU16_t adc_value;
    uint8_t len;
    
    linear_regression((double*)data1,12,&answer[0],&answer[1],&SquarePoor[0]);
    display((double*)data1,answer,&SquarePoor[0],12,2);

    ddc_attach_chx(1,calibrate_para);
    calibrate_flag = 0;
    while(calibrate_flag == 0)
    {
        adc_value.value = adc.read(AIN1);
//        adc_value.byte[0] =  8;
//        adc_value.byte[1] =  9;
       // len = ddc_make_frame(buf,&adc_value.byte[0],2,DDC_NoAck,1);
       // ddc_add_to_list(buf);    
        delay_ms(100);
    }
    ddc_attach_chx(1,0);
    
    temp.ratio = answer[1];
    temp.offset = answer[0];
    
    return temp;
}


AdjustDate_t adjust_test()
{
    AdjustDate_t x;
    x.ratio = 1;
    x.offset = 2;
    x.rows = 3;
    
    return x;
}

