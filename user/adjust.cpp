
#include "adjust.h"
#include "ddc.h"
#include "bsp.h"

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
    uart1.printf("�ع鷽��ʽ:    Y = %.5lf", Answer[0]);
    for (i = 1; i < cols; i ++)
        uart1.printf(" + %.5lf*X%d\r\n", Answer[i], i);
    uart1.printf("�ع������Լ���: \r\n");
    uart1.printf("�ع�ƽ���ͣ�%12.4lf  �ع鷽�%12.4lf \r\n", SquarePoor[0], SquarePoor[2]);
    uart1.printf("ʣ��ƽ���ͣ�%12.4lf  ʣ�෽�%12.4lf \r\n", SquarePoor[1], SquarePoor[3]);
    uart1.printf("���ƽ���ͣ�%12.4lf  ��׼��%12.4lf \r\n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
    uart1.printf("F   ��  �飺%12.4lf  ���ϵ����%12.4lf \r\n", SquarePoor[2] /SquarePoor[3],
           sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
    uart1.printf("ʣ�����: \r\n");
    uart1.printf("      �۲�ֵ      ����ֵ      ʣ��ֵ    ʣ��ƽ�� \r\n");
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
    float value;
    
    value = atof((char *)ptr);
}
void calibrate()
{
    uint8_t buf[20];
    DataU16_t adc_value;
    uint8_t len;
    ddc_attach_chx(1,calibrate_para);
    
    while(1)
    {
        adc_value.value = adc.read(AIN1);
//        adc_value.byte[0] =  8;
//        adc_value.byte[1] =  9;
        len = ddc_make_frame(buf,&adc_value.byte[0],2,DDC_NoAck,1);
        ddc_add_to_list(buf);    
        delay_ms(1000);
    }
    linear_regression((double*)data1,12,&answer[0],&answer[1],&SquarePoor[0]);
    display((double*)data1,answer,&SquarePoor[0],12,2);
    ddc_attach_chx(1,0);

}



