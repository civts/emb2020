#include "../../LightSensor/HAL_OPT3001.h"
#include "light_sensor.h"

unsigned long int getLux(){
    return OPT3001_getLux();
}
