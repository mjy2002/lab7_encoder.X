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
    else
    {
        //error, neither A or B changed...
    }
    
    aState_last = aState;   //Update the global values...
    bState_last = bState;   //Update the global values...
}

int main()
{
    
    _TRISA0 = 1;    //Set RA0 (pin2) as INPUT
    _TRISA1 = 1;    //Set RA0 (pin3) as INPUT
    
    // Configure CN interrupt on Pin2 and Pin3
    _CN2IE = 1;     // Enable CN on pin 2 (CNEN1 register)
    _CN2PUE = 0;    // Disable pull-up resistor (CNPU1 register)
    _CN3IE = 1;     // Enable CN on pin 3 (CNEN1 register)
    _CN3PUE = 0;    // Disable pull-up resistor (CNPU1 register)
    _CNIP = 4;      // Set CN interrupt priority (IPC4 register)
    _CNIF = 0;      // Clear interrupt flag (IFS1 register)
    _CNIE = 1;      // Enable CN interrupts (IEC1 register)
    
    while(1)
    {
        
    }
    
}