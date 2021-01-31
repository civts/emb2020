#include <ti/devices/msp432p4xx/inc/msp432p401r.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Timer_A Up Configuration Parameter */
const Timer_A_UpModeConfig upConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK = 3 MhZ
    TIMER_A_CLOCKSOURCE_DIVIDER_12,      // SMCLK/12 = 250 KhZ
    10000,                               // 40 ms tick period
    TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0 interrupt
    TIMER_A_DO_CLEAR                     // Clear value
};

void buzzerInit()
{
    /* Configures P2.7 to PM_TA0.4 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring Timer_A0 for Up Mode */
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
}

void playWinTone()
{
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    int i;
    for (i = 7; i < 10; i++)
    {
        TIMER_A_CMSIS(TIMER_A0_BASE)->CCR[0] = 100000 / i;

        /* Timer_A Compare Configuration Parameter  (PWM) */
        Timer_A_CompareModeConfig compareConfig_PWM = {
            TIMER_A_CAPTURECOMPARE_REGISTER_4,        // Use CCR3
            TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE, // Disable CCR interrupt
            TIMER_A_OUTPUTMODE_TOGGLE_SET,            // Toggle output but
            1000 * i                                  // 10% Duty Cycle initially
        };

        Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);
        int j;
        for (j = 0; j < 800000; j++)
            ;
    }

    //Stop timer
    Timer_A_stopTimer(TIMER_A0_BASE);
}

void playLossTone()
{
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    int i;
    for (i = 2; i < 7; i++)
    {
        TIMER_A_CMSIS(TIMER_A0_BASE)->CCR[0] = 10000 * i;

        /* Timer_A Compare Configuration Parameter  (PWM) */
        Timer_A_CompareModeConfig compareConfig_PWM = {
            TIMER_A_CAPTURECOMPARE_REGISTER_4,        // Use CCR3
            TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE, // Disable CCR interrupt
            TIMER_A_OUTPUTMODE_TOGGLE_SET,            // Toggle output but
            1000 * i                                  // 10% Duty Cycle initially
        };

        Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);
        int j;
        for (j = 0; j < 800000; j++)
            ;
    }

    //Stop timer
    Timer_A_stopTimer(TIMER_A0_BASE);
}
