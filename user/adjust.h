#ifndef __ADJUST_H
#define __ADJUST_H
#include "ebox.h"
#include "LinearRegression.h"

typedef struct 
{
    DataDouble_t ratioPt;
    DataDouble_t offsetPt;
    DataDouble_t ccPt;

    DataDouble_t ratioRx;
    DataDouble_t offsetRx;  
    DataDouble_t ccRx;
    
    uint8_t     rxMode;
    uint8_t     ptMode;
    DataFloat_t rx;
    DataFloat_t rxOrigin;
    
    DataFloat_t rt;
    DataFloat_t temp;

}PtData_t;

PtData_t calibrate();
PtData_t adjust_test();


bool adjust_check();
void adjust_save(PtData_t *data);
void adjust_read(PtData_t *data);
#endif
