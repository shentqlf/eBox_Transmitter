#ifndef __ADJUST_H
#define __ADJUST_H
#include "ebox.h"
#include "LinearRegression.h"

typedef struct 
{
    double ratio;
    double offset;
    
    uint8_t rows;
    uint8_t cols;
    
//    double tss;//�ع�ƽ����
    
}AdjustDate_t;
AdjustDate_t calibrate();
AdjustDate_t adjust_test();
#endif
