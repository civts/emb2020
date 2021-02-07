#ifndef __HW_INIT_H__
#define __HW_INIT_H__

// Performs the hardware initialization
// Also initializes the graphics context
void hwInit();

void _graphicsInit();

void _lightSensorInit();

void _initButton();

void _configure_ADC();
#endif
