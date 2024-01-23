#ifndef TOUCH_SENSORS_H
#define TOUCH_SENSORS_H

#include "frdm_bsp.h"

void TouchSensors_Init(void);

typedef enum{B0=0, B6=6, B7=7} TouchSensorOutputs;

#endif // TOUCH_SENSORS_H
