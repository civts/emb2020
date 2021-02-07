#include "joystick.h"
#include <ti/devices/msp432p4xx/inc/msp432p401r.h>

void readJoystickPosition()
{
    ADC14->CTL0 |= ADC14_CTL0_SC;
}
