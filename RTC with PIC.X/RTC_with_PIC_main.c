/*
 * File:   RTC_with_PIC_main.c
 * Author: Aswinth
 * More info: www.circuitdigest.com 
 * Created on 5 May, 2018, 6:42 PM
 */
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000 //We are running on 20MHz crystal 

//Define the LCD pins
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

/*Set the current value of date and time below*/
int sec = 00;
int min = 55;
int hour = 10;
int date = 06;
int month = 05;
int year = 18;
/*Time and Date Set*/

#include <xc.h>
#include "lcd.h" //Header for using LCD module
#include "PIC16F877a_I2C.h" // Header for using I2C protocal
#include "PIC16F877a_DS3231.h" //Header for using DS3231 RTC module




int main()
{

    TRISD = 0x00; //Make Port D pins as outptu for LCD interfacing 
    
    Lcd_Start(); // Initialize LCD module
    
    I2C_Initialize(100); //Initialize I2C Master with 100KHz clock
        
    Set_Time_Date(); //set time and date on the RTC module
    
     //Give an intro message on the LCD
     Lcd_Clear();
     Lcd_Set_Cursor(1,1);
     Lcd_Print_String("  RTC with PIC");
     Lcd_Set_Cursor(2,1);
     Lcd_Print_String(" -Circuit Digest");
     __delay_ms(1500); //display for 1.5sec

while(1)
  {
 
    Update_Current_Date_Time(); //Read the current date and time from RTC module
    
    //Split the into char to display on lcd
     char sec_0 = sec%10;
     char sec_1 = (sec/10);
     char min_0 = min%10;
     char min_1 = min/10;
     char hour_0 = hour%10;
     char hour_1 = hour/10;
     char date_0 = date%10;
     char date_1 = date/10;
     char month_0 = month%10;
     char month_1 = month/10;
     char year_0 = year%10;
     char year_1 = year/10;
    
       //Display the Time on the LCD screen
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Print_String("TIME: ");
        Lcd_Print_Char(hour_1+'0');
        Lcd_Print_Char(hour_0+'0');
        Lcd_Print_Char(':');
        Lcd_Print_Char(min_1+'0');
        Lcd_Print_Char(min_0+'0');
        Lcd_Print_Char(':');
        Lcd_Print_Char(sec_1+'0');
        Lcd_Print_Char(sec_0+'0');
        
        //Display the Date on the LCD screen
        Lcd_Set_Cursor(2,1);
        Lcd_Print_String("DATE: ");
        Lcd_Print_Char(date_1+'0');
        Lcd_Print_Char(date_0+'0');
        Lcd_Print_Char(':');
        Lcd_Print_Char(month_1+'0');
        Lcd_Print_Char(month_0+'0');
        Lcd_Print_Char(':');
        Lcd_Print_Char(year_1+'0');
        Lcd_Print_Char(year_0+'0');
        __delay_ms(500); //refresh for every 0.5 sec
    
  }

    return 0;
}