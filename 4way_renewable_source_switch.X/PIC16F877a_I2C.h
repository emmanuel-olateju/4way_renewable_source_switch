/*
 * File: Header file to use I2C with PIC16F877A
 * Author: B.Aswinth Raj
 * Created on 5 May, 2018, 7:24 PM
 */

//PIN 18 -> RC3 -> SCL
//PIN 23 -> RC4 ->SDA

void I2C_Initialize(const unsigned long feq_K) //Begin IIC as master
{ 
  SSPCON  = 0b00101000;    //pg84/234 
  SSPCON2 = 0b00000000;    //pg85/234
  SSPSTAT = 0b00000000;    //pg83/234
  SSPADD = (_XTAL_FREQ/(4*feq_K))-1; //Setting Clock Speed pg99/234
  TRISC3 = 1;  TRISC4 = 1;  //Set SDA and SCL pins as input pins
}

void I2C_Hold()
{
    while (   (SSPCON2 & 0b00011111)    ||    (SSPSTAT & 0b00000100)   ) ; //check the bis on registers to make sure the IIC is not in progress
}

void I2C_Begin()
{
  I2C_Hold();  //Hold the program is I2C is busy  
  SEN = 1;     //Begin IIC pg85/234
}

void I2C_Restart(){
    I2C_Hold();
    RSEN=1;
}

void I2C_End()
{
  I2C_Hold(); //Hold the program is I2C is busy  
  PEN = 1;    //End IIC pg85/234
}


unsigned short I2C_Write(unsigned short data)
{
    I2C_Hold(); //Hold the program is I2C is busy 
    SSPBUF = data;         //pg82/234
    I2C_Hold();
    return ACKSTAT;
}

unsigned short I2C_Read(unsigned short ack)
{
    unsigned short data;
    I2C_Hold();
    RCEN = 1;
    I2C_Hold();
    data=SSPBUF;
    I2C_Hold();
    ACKDT=ack;
    ACKEN=1;
    return data;
}