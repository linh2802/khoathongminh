/***************************************************************************************
 *  File:Keypad_Password.c
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
#include "Keypad_Password.h"
#include "LCD.h"


uint8_t Password[4]="0000"; //password mac dinh
uint8_t EnteredPassword[4];
uint8_t col=4; // dung trong ham keypad_wait open and close the pins of columns in the order
uint8_t inttertuptvalue=0; //dung de xac dinh pin nao ngat
uint8_t X;//gia tri hang keypad
uint8_t Y;//gia tri cot keypad
uint8_t Enterd_Password_Counter=0;// bien dem so ky tu dang duoc nhap
uint8_t Password_Confirm_Counter=0;// bien dem so ky tu trong password
uint8_t NewPassword_Counter=0;
bool NewPasswordControl=0;
bool CorrectPasswordWait=0;

/*
 * the 3*4 Keypad
 * */
uint8_t MKeyPad[4][3]= {
		  {'1','2','3'},
		  {'4','5','6'},
		  {'7','8','9'},
		  {'*','0','#'}
		};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KeyPad_Lcd_Setup()
{

	 has_lcd_erase();
	 has_lcd_write(1,1,"Enter the ");
	 has_lcd_write(2,1,"Password:");

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KeyPad_PassWord_Confirm()
{
	uint8_t b=0;//dung trong vong lap
   /*
    * neu password da duoc reset thi (CounterNewPassword==4) tra gia tri true
    *
    * */
   if(NewPassword_Counter==4)
   {
	   NewPasswordControl=0;
	   NewPassword_Counter=0; //Reset ve 0 neu password duoc reset nhieu lan
	   has_lcd_erase();
	   has_lcd_write(1,1,"Password has");
	   has_lcd_write(2,1,"been changed");
	   SysCtlDelay(SysCtlClockGet()/2);
	   KeyPad_Lcd_Setup();//chuan bi nhap password sau khi reset

   }
   /*
    * else kiem tra password da nhap dung hay sai
    * bang cach so sanh Password mac dinh voi EnteredPassword
    *
    * */
   else{
	     for(b=0;b<4;b++){
	   	     if(EnteredPassword[b]==Password[b]){
	   	    	Password_Confirm_Counter++;
	   	     }
	     }
	     /* if Entered Password dung
	      * in this statement PF1(RELAY or RGB) will be set to 1
	      * CorrectPasswordWait se duoc set to 1  khong cho phep nhap password nua ngoai tru reset password (xen ham void KeyPad_int())
	      * */
         if(Password_Confirm_Counter==4)
         {
        	 has_lcd_erase();
        	 has_lcd_write(1,1,"Correct Password");
        	 SysCtlDelay(SysCtlClockGet()/2);
        	 CorrectPasswordWait=1;
        	 has_lcd_erase();
        	 has_lcd_write(1,1,"Door is open ");
        	 has_lcd_write(2,1,"LED is lighting");
        	 GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,4);
         }
         /*
          * else if Entered Password sai thi try again
          * */
         else
         {
        	 has_lcd_erase();
        	 has_lcd_write(1,1,"incorrect Passwo");
        	 has_lcd_write(2,1,"Try again");

        	 SysCtlDelay(SysCtlClockGet()/2);
        	 KeyPad_Lcd_Setup();

         }

         Enterd_Password_Counter=0;
         Password_Confirm_Counter=0;

   	   }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * to calculate 2^x dung de quet keypad
 * */

uint8_t power(uint8_t x)
{
	uint8_t y=1;
	uint8_t i=0;
	for(i=0;i<x;i++)
	{
		y=y*2;
	}
return y;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KeyPad_wait()
{
	/*
	 * while the digits of password (the correct password or Enterd password )is lease than 4 digit
	 * the operation of set-clear columns pins in order will countinues
	 */

	while((Enterd_Password_Counter<4)&&(NewPassword_Counter<4)){
	   for(col=4;col<=6;col++){
	     GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,112-power(col));//look to the comment below
	     /*
	      *(2^4)+(2^5)+(2^6)=112
	      *(112-2^COl) operation sets the pins(4-7)to 1(logic) except the pin of COL ( if Col=4 --->PIN4=0(logic) )
	      */
	     SysCtlDelay(20000);//by testing 20000 MS is a good duration between Entering the digits

	   }
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NewPasswordWait()
{
	/*
	 * to Clear the pin of Relay(or RGB RED) and'CorrectPasswordWait' if the password is changed after Turning on the PIN(PF1)
	 *
	 * */
	 GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
	 CorrectPasswordWait=0;

	 /*
	  * Preparing LCD to New Password
	  * */
	 has_lcd_erase();
	 has_lcd_write(1,1,"Enter new pass");
	 has_lcd_position(2,1);

	 NewPasswordControl=1;//To determine the entered char if it belongs to the Password or to a Entered Passowrd

}
/////////////////////////////////////////////////////////////////////////////////////
/*
 * to take log2 of x
 * */

uint8_t ln(uint8_t x)
{
	uint8_t y=0;
		while(x!=1)
		{
			x=x/2;
			y++;
		}

return y;
}
///////////////////////////////////////////////////////////////////////////////

void KeyPad_int()
{
/*
 * when on of pins4to7 of PORTA is fulling to 0(interrupting) the Program Counter will Indicate to this function
 *
 */

	inttertuptvalue=GPIOIntStatus(GPIO_PORTA_BASE,false); //which pin is interrupted (EX: if pin4---> inttertuptvalue=16)

    X=ln(inttertuptvalue)-4;//look below
    /*
     * taking log2 of inttertupvalue will give the pins number
     * and then shifting by -4 will give the real number of Keypads row
     */

    Y=col-4;//shifting (EX 4 to 0 , 7 to 3)will give the real number of Keypads column

    /*
     * if the System waits a password (CorrectPasswordWait==0) the entered charecter will be sent to LCD
     *
     * */
    if(!CorrectPasswordWait){
    has_lcd_sendchar(MKeyPad[X][Y]);
    }
    SysCtlDelay(SysCtlClockGet()/4);//choosen by testing


    /*
     * this 8 lines below are written to detect the holding of #
     * that will allow to change a virtual password
     * */
    if (MKeyPad[X][Y]=='#'){
        SysCtlDelay(20000);

        /*
         *   @if the holding of # crosses a specific duration the PC will go to NewPasswordWait() function
         * */
        if(GPIOPinRead(GPIO_PORTA_BASE,inttertuptvalue)==0){
            SysCtlDelay(SysCtlClockGet());

         	while(GPIOPinRead(GPIO_PORTA_BASE,inttertuptvalue)==0){
         		NewPasswordWait();
         	}
        }

        /*
         * @else '#' will be a typical charecter
         * */

        else if(!CorrectPasswordWait){
        	PasswordTypeControl();
        }
    }
    /*
     * if '#' is not has been entered in the first place
     * */
    else if(!CorrectPasswordWait){
    	PasswordTypeControl();
    }

    GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PasswordTypeControl()
{
	/*
	 * if MKeyPad belongs to password(NewPasswordControl==1) send it to passwords array
	 * else send it to EnterdPasswords array
	 * */

	  if(NewPasswordControl){
	              Password[NewPassword_Counter]=MKeyPad[X][Y];
	              NewPassword_Counter++;
	  }
	  else{
	           EnteredPassword[Enterd_Password_Counter]=MKeyPad[X][Y];
	           Enterd_Password_Counter++;
	  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KeyPad_Setup()
{
	IntMasterEnable();
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Enable the peripheral of GPIOPORT F
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7); //PC4 to PC7 are columns output

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//look to the comment below
	/*
	 * PF1 to PF3 can be used as a controlled pins of RELAY its also connected to RGB LEDs of TIVE C kit
	 * */

    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,0xFF);//look to the comment below
    /*
     * GPIO_PORTC pin4 to pin 7 are connected to the columun pins of Keypad
     * they will sets to 1 and Keypad_wait fun. will clear its one by one in order leaving the others set to 1
     *
     * */

		GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);// look to the comment below
		/*
		 *GPIO PORTA pin4 to 7 are connected to the Rows pins of keypad
		 *they will set as input to perceive the sinyal that will come from Keypad
		 *
		 * */

		GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);// look to the comment below
		/*
		 * GPIO PORTA pin4 to 7 will sets to 1 and trigger to 1-0 fulling edge
		 * */

		GPIOIntRegister(GPIO_PORTA_BASE,KeyPad_int);//Register the Function of interrupting

		GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_FALLING_EDGE);//trigger to 1-0 fulling edge


		GPIOIntClear(GPIO_PORTA_BASE,  GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
		GPIOIntEnable(GPIO_PORTA_BASE,  GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
		IntEnable(INT_GPIOF_TM4C123);

}


