#include "pcm.h"
#include <ti/devices/msp432p4xx/driverlib/pcm.h>

void gotoLPM()
{
    PCM_gotoLPM0();
}
