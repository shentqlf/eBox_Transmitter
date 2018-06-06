#ifndef __ADJUST_H
#define __ADJUST_H
#include "ebox.h"
#include "LinearRegression.h"

typedef struct 
{
    DataFloat_t ratioP;
    DataFloat_t offsetP;
    DataFloat_t ccP;

    DataFloat_t ratioR;
    DataFloat_t offsetR;  
    DataFloat_t ccR;
}AdjustDate_t;

AdjustDate_t calibrate();
AdjustDate_t adjust_test();
#endif
