/***************************************************************************************
 *  File:main.c
 ***************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "LCD.h"
#include "Keypad_Password.h"

int main()
{
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
    has_lcd_4bitsetup(); //setup lcd
    KeyPad_Lcd_Setup();
    KeyPad_Setup();
    while(1)
    {
                KeyPad_wait();
    	        KeyPad_PassWord_Confirm();
    }
}
