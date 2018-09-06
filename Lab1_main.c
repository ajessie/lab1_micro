// This application uses button 1 and LED 1 on Launchpad
// The operation is such that if you press the button 1, as long as you keep the button pressed,
// the LED1 is on. As soon as you release the button, LED1 is off and stays off until the button is pressed again.
// This program uses Hardware Abstraction Level (HAL) functions that hide away device-specific details.

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// According to the schematics on page 37 of the Launchpad user guide,
// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0

// This function initializes all the peripherals
void initialize();

void TurnOn_LaunchpadLED1();
void TurnOff_LaunchpadLED1();
char SwitchStatus_LaunchpadLED1();

int main(void)
{

    initialize();
    while (1) {

         // If the button is not pressed, keep the LED off
        if (SwitchStatus_LaunchpadLED1() != PRESSED)
            TurnOff_LaunchpadLED1();
        else
            TurnOn_LaunchpadLED1();
    }
}


// Initialization part is always device dependent and therefore does not change much with HAL
void initialize()
{

    // step 1: Stop watchdog timer
    // We do this at the beginning of all our programs for now.Later we learn more about it.
    WDT_A_hold(WDT_A_BASE);

    // step 2: Initializing Launchpad LED1, which is on Pin 0 of Port P1 (from page 37 of the Launchpad User Guide)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    // step 3: Initializing Launchpad S1 (switch 1 or button 1),
    // which is on Pin1 of Port 1 (from page 37 of the Launchpad User Guide)
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN1);
}

void TurnOn_LaunchpadLED1()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void TurnOff_LaunchpadLED1()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
char SwitchStatus_LaunchpadLED1()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));
}
