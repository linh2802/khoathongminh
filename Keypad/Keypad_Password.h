/*****************************************************************************
 *  File: Keypad_Password.h
 *****************************************************************************/

#ifndef KEYPAD_PASSWORD_H_
#define KEYPAD_PASSWORD_H_

/******************************************************************************************
*Function: Keypad_wait()
*Description: while the digits of password (the correct password or Enterd password )is lease than 4 digit
*             the operation of set-clear columns pins in order will countinues by using this function
 *****************************************************************************************/
void KeyPad_wait();


/******************************************************************************************
*Function: Keypad_int()
*Description: the interrupting function of Keypad_Setup()
*             in this function operation of taking and processing the entered character will be perform
 *****************************************************************************************/
void KeyPad_int();

/******************************************************************************************
*Function: Keypad_Setup()
*Description:this fun is used to Enable the peripheral,preparing the External interrupting
*            and config the input methods
 *****************************************************************************************/
void KeyPad_Setup();

/******************************************************************************************
*Function:KeyPad_Lcd_Setup()
*Description:writing to LCD
*
 *****************************************************************************************/
void KeyPad_Lcd_Setup();

/******************************************************************************************
*Function:KeyPad_PassWord_Confirm()
*Description:1-this function is used to Confirm the password if its correct or not
*              and according to that the Relay will be sets to 1 or not
*            2-also used to confirm password reseting
 *****************************************************************************************/
void KeyPad_PassWord_Confirm();

/******************************************************************************************
*Function: NewPasswordWait()
*Description:to Clear the pin of Relay(or LED) and'CorrectPasswordWait'
*            if the password is changed after Turning on the PIN(PF1)
*        	 Preparing LCD to New Password
*
 *****************************************************************************************/
void NewPasswordWait();

/******************************************************************************************
*Function: PasswordTypeControl()
*Description:To determine the type of password if it's Entered or New
*
 *****************************************************************************************/
void PasswordTypeControl();

/******************************************************************************************
*Function: uint8_t power(uint8_t)
*Description:Calculate 2^x
*
 *****************************************************************************************/
uint8_t power(uint8_t);

/******************************************************************************************
*Function: uint8_t ln(uint8_t)
*Description:Calculate log2(x)
*

 *****************************************************************************************/
uint8_t ln(uint8_t);


#endif /* KEYPAD_PASSWORD_H_ */
