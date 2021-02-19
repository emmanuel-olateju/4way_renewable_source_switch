/*
 * File:   main.c
 * Author: peTron_Quantinity
 *
 * Created on February 1, 2021, 7:25 PM
 */


#include <xc.h>
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF         // Data EEPROM Memory Code Protection bit (Data EEPROM code-protected)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF          // Flash Program Memory Code Protection bit (All program memory code-protected)

#define _XTAL_FREQ 20000000
#include "PIC16F877a_I2C.h"
int sec=0;
int min=10;
int hour=15;
#include "PIC16F877a_DS3231.h"
#define rtc_min_set RC0
#define rtc_hour_set RA4
#define RS RB6
#define EN RB5
#define D4 RB4
#define D5 RB3
#define D6 RB2
#define D7 RB1
#include "LCDLIBRARY1.h"

//INPUTS
int wind_val,solar_val,biogas_val,phcn_val;
#define wind_pindir TRISA0
#define solar_pindir TRISA1
#define biogas_pindir TRISA2
#define phcn_pindir TRISA3
//OUTPUT TUPLE
#define wind RD0
#define solar RD1
#define biogas RD2
#define phcn RD3
//STATES
enum state {initialState,morning,afternoon,night,set_rtc,defunct}nextState;
enum state nextState=initialState;
//VIAS
int thresh=878;

void __interrupt() ISR(void){
    if(TMR0IF==1){
        if(hour<=22)hour++;
        else hour=0;
    }
    if(TMR1IF==1){
        if(min<=22)min++;
        else min=0;
    }
    TMR0IF=0;
    TMR1IF=0;
    TMR0=255;
    TMR1=65535;
    CLRWDT();
    Set_Time_Date();
    CLRWDT();
    RD5=0;
}
void main(void){
    TRISB=0x00;
    PORTB=0x00;
    TRISA=0xFF;
    ADCON1=0x80;
    ADCON0=0x41;
    TRISD=0xC0;
    PORTD=0x00;
    TMR0=255;
    TMR0IE=1;
    OPTION_REG=0xA8;
    TRISC0=1;
    T1CON=0x06;
    TMR1=65535;
    TMR1IE=1;
    SSPIE=0;
    PEIE=1;
    GIE=1;
    TMR1ON=1;
    wind_pindir=1;
    solar_pindir=1;
    biogas_pindir=1;
    phcn_pindir=1;
    I2C_Initialize(100000);
    Set_Time_Date();
    config();
    CLRDISP();
    TRISD4=0;TRISD5=0;
    RD4=0;RD5=0;
    
    while(1){
        CLRDISP();
        CURSOR(FIRSTROW,3);
        LCDWRITE("PHASE SELECTOR");
        CURSOR(SECONDROW,0);
        LCDNUM(hour);
        LCDWRITE(":");
        LCDNUM(min);
        LCDWRITE(":");
        LCDNUM(sec);
        if(nextState==initialState){
            RC7=RC6=RC5=RC4=1;
            __delay_ms(100);
            ADCON0bits.CHS=0;
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            __delay_us(20);
            wind_val=(ADRESH<<8)+ADRESL;
            ADCON0bits.CHS=1;
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            __delay_us(20);
            solar_val=(ADRESH<<8)+ADRESL;
            ADCON0bits.CHS=2;
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            __delay_us(20);
            biogas_val=(ADRESH<<8)+ADRESL;
            ADCON0bits.CHS=3;
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            __delay_us(20);
            phcn_val=(ADRESH<<8)+ADRESL;
            Update_Current_Date_Time();//RTC module for time
            if((hour>0)&&(hour<=7))nextState=night;
            else if((hour>7)&&(hour<12))nextState=morning;
            else if(hour<18)nextState=afternoon;
            else if(hour<23)nextState=night;
        }
        CLRWDT();
        __delay_ms(10);
        if(nextState==morning){
            if(solar_val<thresh){
                wind=0;solar=1;biogas=0;phcn=0;
            }else if(wind_val<thresh){
                wind=1;solar=0;biogas=0;phcn=0;
            }else if(phcn_val<thresh){
                wind=0;solar=0;biogas=0;phcn=1;
            }else{
                wind=0;solar=0;biogas=0;phcn=0;
            }
            nextState=initialState;
        }
        if(nextState==afternoon){
            if(solar_val<thresh){
                wind=0;solar=1;biogas=0;phcn=0;
            }else if(wind_val<thresh){
                wind=1;solar=0;biogas=0;phcn=0;
            }else if(phcn_val<thresh){
                wind=0;solar=0;biogas=0;phcn=1;
            }else{
                wind=0;solar=0;biogas=1;phcn=0;
            }
            nextState=initialState;
        }
        if(nextState==night){
            if(biogas_val<thresh){
                wind=0;solar=0;biogas=1;phcn=0;
            }else if(wind_val<thresh){
                wind=1;solar=0;biogas=0;phcn=0;
            }else if(phcn_val<thresh){
                wind=0;solar=0;biogas=0;phcn=1;
            }else{
                wind=0;solar=1;biogas=0;phcn=0;
            }
            nextState=initialState;
        }
        Update_Current_Date_Time();
    }
    return;
}
