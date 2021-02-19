/*
 * File: Header file to use DS3231 RTC module with PIC16F877A
 * Author: B.Aswinth Raj
 * Created on 5 May, 2018, 10:06 PM
 * Connect the RTC module to SDA and SCK pins of the PIC.
 * This header needs the PIC16f877a_I2C.h header to compile
 */


//PIN 18 -> RC3 -> SCL
//PIN 23 -> RC4 ->SDA


/****** Functions for RTC module *******/

int  BCD_2_DEC(int num)
{
    int data=((num>>4)*10)+(num&0x0F);
    return data; 
}

int DEC_2_BCD (int num)
{
    int data=((num/10)<<4)+(num%10);
    return data;
}

void Set_Time_Date()
{
   I2C_Begin();       
   I2C_Write(0xD0); 
   I2C_Write(0);
   I2C_Write(DEC_2_BCD(sec));  
   I2C_Write(DEC_2_BCD(min)); //update sec
   I2C_Write(DEC_2_BCD(hour)); //update min
   I2C_Write(1); //update hour
   I2C_Write(1); //ignore updating day
   I2C_Write(1); //update date
   I2C_Write(1); //update month
   I2C_End();
}

void Update_Current_Date_Time()
{
   I2C_Begin();       
   I2C_Write(0xD0); 
   I2C_Write(0);    
   I2C_Restart(); 
   I2C_Write(0xD1);
   sec=BCD_2_DEC(I2C_Read(0));
   min=BCD_2_DEC(I2C_Read(0));
   hour=BCD_2_DEC(I2C_Read(0));
   I2C_Read(0);
   I2C_Read(0);
   I2C_Read(0);
   I2C_Read(1);
   I2C_End(); 
}
