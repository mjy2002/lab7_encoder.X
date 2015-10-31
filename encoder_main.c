#include <p24F16KA301.h>

// Select oscillator
_FOSCSEL(FNOSC_FRC);        // 8 MHz FRC oscillator

int aState;
int bState;
int aState_last;
int bState_last;
long count;

void _ISR _CNInterrupt(void)    //Encoder Reading Code...
{
    _CNIF = 0;      // Clear interrupt flag (IFS1 register)
     
    aState = _RA0;  //channel A on Pin2
    bState = _RA1;  //Channel B on Pin3
    
    if(aState != aState_last)   //Then the change occured on channel A
    {
        if(aState)    //Rising Edge
        {
            if(bState)  //B is HIGH, thus CCW
            {
                count--;
            }
            else    //B is LOW, thus CW
            {
                count++;
            }
        }
        else    //Falling Edge
        {
            if(bState)  //B is HIGH, thus CW
            {
                count++;
            }
            else    //B is LOW, thus CCW
            {
                count--;
            }
        }
    }
    else if(bState != bState_last)  //B changed
    {
        if(bState)  //Rising Edge on B
        {
            if(aState)  //A is HIGH, thus CW
            {
                count++;
            }
            else    //A is LOW, thus CCW
            {
                count--;
            }
        }
        else    //Falling Edge on B
        {
           if(aState)  //A is HIGH, thus CCW
            {
               count--;
            }
            else    //A is LOW, thus CW
            {
               count++;
            } 
        }
        
    }
//    else
//    {
//        //count = 2000;//error, neither A or B changed...
//    }
    
    /*
    if(count > 120)
    {
        count = 0;
    }
    if(count < -120)
    {
        count = 0;
    }
     
     */
    
    aState_last = aState;   //Update the global values...
    bState_last = bState;   //Update the global values...
}


void config_pwm_4()
{
    //------------------------------------------------------------------
    // CONFIGURE PWM1 (on pin 14) USING TIMER2

    // Configure Timer2, which will be the source for the output compare that
    // drives PWM1
    T2CONbits.TON = 0;      // Timer2 off while configuring PWM, pg. 144
    T2CONbits.TCKPS = 0b00;    // Timer2 1:1 clock pre-scale, pg. 144
    T2CONbits.T32 = 0;      // Timer2 acts as a single 16-bit timer, pg. 144
    T2CONbits.TCS = 0;      // Timer2 internal clock source, pg. 144

    // Configure PWM1 on OC1 (pin 14)
    OC2CON1 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC2CON2 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC2CON1bits.OCTSEL = 0b000;       // Use Timer2, pg. 157
    OC2CON1bits.OCM = 0b110;          // Edge-aligned PWM mode, pg. 158
    OC2CON2bits.SYNCSEL = 0b01100;    // Use Timer2, pg. 160

    // Set period and duty cycle
    PR2 = 3999;                // Period of Timer2 to achieve FPWM = 1 kHz
                            // See Equation 15-1 in the data sheet
    OC2R = 1;               // Set Output Compare value to achieve desired duty
                            // cycle. This is the number of timer counts when
                            // the OC should send the PWM signal low. The duty
                            // cycle as a fraction is OC1R/PR2.

    // Turn on Timer2
    T2CONbits.TON = 1;
}


int main()
{
    ANSA = 0;       //Turn off analog for port A
    ANSB = 0;       //Turn off analog for port B
    
    _TRISA0 = 1;    //Set RA0 (pin2) as INPUT
    _TRISA1 = 1;    //Set RA0 (pin3) as INPUT
    
    aState_last = _RA0;  //channel A on Pin2
    bState_last = _RA1;  //Channel B on Pin3
    
    // Configure CN interrupt on Pin2 and Pin3
    _CN2IE = 1;     // Enable CN on pin 2 (CNEN1 register)
    _CN2PUE = 0;    // Disable pull-up resistor (CNPU1 register)
    _CN3IE = 1;     // Enable CN on pin 3 (CNEN1 register)
    _CN3PUE = 0;    // Disable pull-up resistor (CNPU1 register)
    _CNIP = 4;      // Set CN interrupt priority (IPC4 register)
    _CNIF = 0;      // Clear interrupt flag (IFS1 register)
    _CNIE = 1;      // Enable CN interrupts (IEC1 register)
    
    config_pwm_4();
    
    while(1)
    {
        OC2R = 33.3*count;
    }
    
}