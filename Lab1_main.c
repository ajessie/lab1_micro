// This application uses button 1 and LED 1 on Launchpad
// The operation is such that if you press the button 1, as long as you keep the button pressed,
// the LED1 is on. As soon as you release the button, LED1 is off and stays off until the button is pressed again.
// This program uses Hardware Abstraction Level (HAL) functions that hide away device-specific details.

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// According to the schematics on page 37 of the Launchpad user guide,
// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0
#define RIGHT_BUTTON BIT4
#define LEFT_BUTTON BIT1
#define LEFT_LED BIT0 //left led on the launch pad
#define BOOSTER_BTN BIT1 //Booster Pack S1 button, bit1
#define RIGHT_LED_RED BIT0//red led on launch pad
#define RIGHT_LED_BLUE BIT2 //blue led on launch pad
#define RIGHT_LED_GREEN BIT1 //green led on launch pad
#define BOOSTER_LED_BLUE BIT6 //blue LED on Booster pack
#define BOOSTER_LED_GREEN BIT4 //green LED on Booster pack
#define BOOSTER_LED_RED BIT6//red LED on Booster pack
#define BOOSTER_BTN2 BIT5//Booster Pack button s2, bit5



// This function initializes all the peripherals
void initialize();

void TurnOn_LaunchpadLED1();
void TurnOff_LaunchpadLED1();
void TurnOn_BoosterRGB();
void TurnOff_BoosterRGB();
char SwitchStatus_LaunchpadLED1();
char SwitchStatus_LaunchpadLED2();
char SwitchStatus_BoosterPack1();


int main(void)
{

    initialize();
    while (1) {

        if ((P1IN & LEFT_BUTTON) != PRESSED)                //we have to check to see if the left button has been pressed
            P1OUT = P1OUT & ~LEFT_LED;                     //if it hasn't been pressed, we will keep the right red LED off

        else if ((P1IN & LEFT_BUTTON) == PRESSED)           //if it has, we will turn on the left red LED
            P1OUT = P1OUT | LEFT_LED;

        if ((P1IN & RIGHT_BUTTON) != PRESSED){              //if right button has not been pressed
            P2OUT = P2OUT & ~RIGHT_LED_RED;                 // we will keep the LEDS off
            P2OUT = P2OUT & ~RIGHT_LED_BLUE;
            P1OUT = P1OUT & ~LEFT_LED;
            P2OUT = P2OUT & ~BOOSTER_LED_GREEN;

        }
        else if ((P1IN & RIGHT_BUTTON) == PRESSED) {                                    //if right button is pressed we have to check if the left button is also pressed

            if ((P1IN & LEFT_BUTTON) == PRESSED && (P1IN & RIGHT_BUTTON) == PRESSED){  //check to see if both the left and right buttons are pressed on launch pad
                //turn on left red led on launch pad
                P1OUT = P1OUT | LEFT_LED;
                //turn on right blue led on launch pad
                P2OUT = P2OUT | RIGHT_LED_BLUE;
                P2OUT = P2OUT | BOOSTER_LED_GREEN;
            }
            else                                                                       //if only the right button is pressed turn on these LEDS
                P2OUT = P2OUT | (RIGHT_LED_RED | RIGHT_LED_BLUE);
        }

        if ((P5IN & BOOSTER_BTN)!= PRESSED){                                          //if top button on booster pack isn't pressed we will keep these LEDs off
            P5OUT = P5OUT & ~BOOSTER_LED_BLUE;
            P2OUT = P2OUT & (~BOOSTER_LED_GREEN & ~BOOSTER_LED_RED);
            P1OUT = P1OUT & ~LEFT_LED;

        }
        else if ((P5IN & BOOSTER_BTN) == PRESSED){                                    //if the top button is pressed we have to also check if the button is pressed as well
            if ((P5IN & BOOSTER_BTN) == PRESSED && (P3IN & BOOSTER_BTN2) == PRESSED){ //check to see if top and bottom buttons on booster pack have been pressed
                P1OUT = P1OUT | LEFT_LED;
                P2OUT = P2OUT | (RIGHT_LED_RED |BOOSTER_LED_RED);
            }
            else {                                                                     //if only the top button has been pressed we will turn these LEDs on
            P5OUT = P5OUT | BOOSTER_LED_BLUE;
            P2OUT = P2OUT | (BOOSTER_LED_GREEN | BOOSTER_LED_RED);
            }
        }

        if ((P3IN & BOOSTER_BTN2) != PRESSED){                                        //if bottom button isn't pressed, keep LEDs off
            P5OUT = P5OUT & ~BOOSTER_LED_BLUE;
            P2OUT = P2OUT & (~BOOSTER_LED_GREEN & ~BOOSTER_LED_RED);
            P2OUT = P2OUT & (~RIGHT_LED_BLUE & ~RIGHT_LED_RED & ~RIGHT_LED_GREEN);

        }

        else if ((P3IN & BOOSTER_BTN2) == PRESSED) {
            if ((P5IN & BOOSTER_BTN) == PRESSED && (P3IN & BOOSTER_BTN2) == PRESSED){ //check to see if top and bottom buttons on booster pack have been pressed
                P1OUT = P1OUT | LEFT_LED;                                             //we will turn on all red LEDs
                P2OUT = P2OUT | (RIGHT_LED_RED | BOOSTER_LED_RED);
            }



            else{
            P5OUT = P5OUT | BOOSTER_LED_BLUE;                                       //if only bottom button is pressed, turn on designated LEDs
            P2OUT = P2OUT | (BOOSTER_LED_GREEN | BOOSTER_LED_RED);
            P2OUT = P2OUT | (RIGHT_LED_RED | RIGHT_LED_BLUE | RIGHT_LED_GREEN);
            }
        }

        if ((P3IN & BOOSTER_BTN2) != PRESSED && (P5IN & BOOSTER_BTN)!= PRESSED && (P1IN & RIGHT_BUTTON)!= PRESSED && (P1IN & LEFT_BUTTON)!= PRESSED){ //if not all four buttons are pressed, keep red, white, blue off
            P5OUT = P5OUT & ~BOOSTER_LED_BLUE;
            P2OUT = P2OUT & (~BOOSTER_LED_GREEN & ~BOOSTER_LED_RED);
            P2OUT = P2OUT & (~RIGHT_LED_BLUE & ~RIGHT_LED_RED & ~RIGHT_LED_GREEN);
            P1OUT = P1OUT & ~LEFT_LED;
        }


        else if ((P3IN & BOOSTER_BTN2) == PRESSED && (P5IN & BOOSTER_BTN) == PRESSED && (P1IN & RIGHT_BUTTON)== PRESSED && (P1IN & LEFT_BUTTON)== PRESSED){ //but if all four buttons are pressed, turn them on
            P5OUT = P5OUT | BOOSTER_LED_BLUE;
            P2OUT = P2OUT | (RIGHT_LED_RED | RIGHT_LED_BLUE | RIGHT_LED_GREEN);
            P1OUT = P1OUT | LEFT_LED;
        }
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
    P1DIR |= LEFT_LED;

    //initialize LED2 on launch pad
    P2DIR |= (RIGHT_LED_RED |RIGHT_LED_BLUE | RIGHT_LED_GREEN);


    //Initialize the button2 or S2 on the launch pad
    P1DIR &= ~RIGHT_BUTTON;
    P1REN |= RIGHT_BUTTON;
    P1OUT |= RIGHT_BUTTON;

//    //Initializing LEDs on Booster Pack
    P5DIR |= BOOSTER_LED_BLUE;
    P2DIR |= BOOSTER_LED_GREEN;
    P2DIR |= BOOSTER_LED_RED;

    //initialize button (S1) on Booster Pack
    P5DIR &= ~BOOSTER_BTN;
    P5REN |= BOOSTER_BTN;
    P5OUT |= BOOSTER_BTN;

    //Initialize button (S2) on Booster Pack
    P3DIR &= ~BOOSTER_BTN2;
    P3REN |= BOOSTER_BTN2;
    P3OUT |= BOOSTER_BTN2;

    //Initializing left button on the launch pad
    P1DIR &= ~LEFT_BUTTON;
    P1REN |= LEFT_BUTTON;
    P1OUT |= LEFT_BUTTON;

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

void TurnOnRGB(){
    //rgb on booster pack
    P5OUT = P5OUT | BOOSTER_LED_BLUE;
    P2OUT = P2OUT | BOOSTER_LED_GREEN;
    P5OUT = P5OUT | BOOSTER_LED_RED;

    //leds on Launchpad
    P2OUT = P2OUT | RIGHT_LED_RED;
    P2OUT = P2OUT | RIGHT_LED_BLUE;
    P2OUT = P2OUT | RIGHT_LED_GREEN;
}

//void TurnOnRGB2(){
//    //turn on left red led on launch pad
//    P1OUT = LEFT_LED;
//    //turn on right blue led on launch pad
//    P2OUT = P2OUT | (RIGHT_LED_BLUE | BOOSTER_LED_GREEN);
//}

//void TurnOffRGB2(){
//    //turn off red and blue on the
//    P2OUT = P2OUT & (~RIGHT_LED_BLUE & ~BOOSTER_LED_GREEN);
//    P1OUT = P1OUT & ~LEFT_LED;
//}

void TurnOn_BoosterRGB()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6); //turn on blue LED
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4); //turn on green LED
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6); //turn on red LED
}

void TurnOff_BoosterRGB()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6); //turn off blue
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4); //turn off green
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6); //turn off red

}

void turnOnRGBLEDS()
{
    //leds on Booster Pack
    P5OUT = P5OUT | BOOSTER_LED_BLUE;
    P2OUT = P2OUT | BOOSTER_LED_GREEN;
    P5OUT = P5OUT | BOOSTER_LED_RED;

    //leds on Launchpad
    P2OUT = P2OUT | RIGHT_LED_RED;
    P2OUT = P2OUT | RIGHT_LED_BLUE;
    P2OUT = P2OUT | RIGHT_LED_GREEN;

}

char SwitchStatus_LaunchpadLED1()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));
}

char SwitchStatus_LaunchpadLED2()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4));
}


char SwitchStatus_BoosterPack1()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN1));
}

