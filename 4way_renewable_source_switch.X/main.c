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
int min=0;
int hour=0;
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
int wind_val=0;int solar_val=0;int biogas_val=0;int phcn_val=0;
#define wind_pindir TRISA0
#define solar_pindir TRISA1
#define biogas_pindir TRISA2
#define phcn_pindir TRISA3
//OUTPUT TUPLE
#define wind RD0
#define solar RD1
#define biogas RD2
#define phcn RD3
#define intermittent RD4
int combo;
//STATES
enum state {initialState,morning,afternoon,night,set_rtc,defunct}nextState;
enum state nextState=initialState;
//VIAS
int thresh=920;

#define milli1 4000
#define milli2 250

void WIND(){
    if(wind==1);
    else{
        intermittent=1;
        CURSOR(THIRDROW,0);
        LCDWRITE("switching...");
        __delay_ms(milli1);
        wind=solar=biogas=phcn=0;
        wind=1;
        __delay_ms(milli1);
        solar=0;biogas=0;phcn=0;
        intermittent=0;
    }
}
void SOLAR(){
    if(solar==1);
    else{
        intermittent=1;
        CURSOR(THIRDROW,0);
        LCDWRITE("switching...");
        __delay_ms(milli1);
        wind=solar=biogas=phcn=0;
        solar=1;
        __delay_ms(milli1);
        wind=0;biogas=0;phcn=0;
        intermittent=0;
    }
}
void BIOGAS(){
    if(biogas==1);
    else{
        intermittent=1;
        CURSOR(THIRDROW,0);
        LCDWRITE("switching...");
        __delay_ms(milli1);
        wind=solar=biogas=phcn=0;
        biogas=1;
        __delay_ms(milli1);
        wind=0;solar=0;phcn=0;
        intermittent=0;
    }
}
void PHCN(){
    if(phcn==1);
    else{
        intermittent=1;
        CURSOR(THIRDROW,0);
        LCDWRITE("switching...");
        __delay_ms(milli1);
        wind=solar=biogas=phcn=0;
        phcn=1;
        __delay_ms(milli1);
        wind=0;solar=0;biogas=0;
        intermittent=0;
    }
}


void __interrupt() ISR(void){
    CLRWDT();
    RD5=1;
    __delay_ms(100);
    if(TMR0IF==1){
        int dummy=hour;
        if(hour<=22)hour=dummy+1;
        else hour=0;
    }
    if(TMR1IF==1){
        if(min<=58)min++;
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
    ADCON1=0xC2;
    ADCON0=0xC1;
    TRISA=0xFF;
    TRISD=0xC0;
    PORTD=0x00;
    TMR0=255;
    TMR0IE=0;
    OPTION_REG=0xA8;
    TRISC0=1;
    T1CON=0x06;
    TMR1=65535;
    TMR1IE=0;
    SSPIE=0;
    PEIE=0;
    GIE=0;
    TMR1ON=1;
    I2C_Initialize(100000);
    Set_Time_Date();
    TMR0IE=1;
    TMR1IE=1;
    config();
    CLRDISP();
    CLRDISP();
    CURSOR(FIRSTROW,3);
    LCDWRITE("PHASE SELECTOR");
    wind=solar=biogas=phcn=0;
    PEIE=1;
    GIE=1;
    
    while(1){
        CURSOR(SECONDROW,5);
        LCDNUM(hour);
        LCDWRITE(":");
        LCDNUM(min);
        LCDWRITE(":");
        LCDNUM(sec);
        if(nextState==initialState){
            RC7=RC6=RC5=RC4=1;
            __delay_ms(100);
            ADCON0bits.CHS=0;
             __delay_us(20);
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            wind_val=(ADRESH<<8)+ADRESL;
            __delay_us(100);
            ADCON0bits.CHS=1;
             __delay_us(20);
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            solar_val=(ADRESH<<8)+ADRESL;
            __delay_us(100);
            ADCON0bits.CHS=2;
             __delay_us(20);
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            biogas_val=(ADRESH<<8)+ADRESL;
            __delay_us(100);
            ADCON0bits.CHS=3;
             __delay_us(20);
            ADCON0bits.GO_nDONE=1;
            while(ADCON0bits.GO_nDONE);
            phcn_val=(ADRESH<<8)+ADRESL;
            __delay_us(100);
            Update_Current_Date_Time();//RTC module for time
            if((hour>=0)&&(hour<=7))nextState=night;
            else if((hour>7)&&(hour<12))nextState=morning;
            else if(hour<18)nextState=afternoon;
            else if(hour<23)nextState=night;
        }
        CLRWDT();
        __delay_ms(10);
        combo=phcn;
        combo|=(biogas<<1);
        combo|=(solar<<2);
        combo|=(wind<<3);
        if(nextState==morning){
            if(solar_val<thresh)SOLAR();
            else if(wind_val<thresh)WIND();
            else if(phcn_val<thresh)PHCN();
            else BIOGAS();
            nextState=initialState;
        }
        if(nextState==afternoon){
            if(solar_val<thresh)SOLAR();
            else if(wind_val<thresh)WIND();
            else if(phcn_val<thresh)PHCN();
            else BIOGAS();
            nextState=initialState;
        }
        if(nextState==night){
            if(biogas_val<thresh)BIOGAS();
            else if(wind_val<thresh)WIND();
            else if(phcn_val<thresh)PHCN();
            else SOLAR();
            nextState=initialState;
        }
        Update_Current_Date_Time();
        CURSOR(THIRDROW,0);
        int out=(wind&1)|((solar&1)<<1)|((biogas&1)<<2)|((phcn&1)<<3);
        switch(out){
            case 0:
                LCDWRITE("NULL         ");
                break;
            case 1:
                LCDWRITE("wind         ");
                break;
            case 2:
                LCDWRITE("solar        ");
                break;
            case 4:
                LCDWRITE("biogas       ");
                break;
            case 8:
                LCDWRITE("phcn  ");
                break;
            default:
                LCDWRITE("RESET");
                break;
        }
//        CURSOR(THIRDROW,6);
//        LCDNUM(wind_val);
//        LCDCHAR(' ');
//        LCDNUM(solar_val);
//        LCDCHAR(' ');
//        LCDNUM(biogas_val);
//        LCDCHAR(' ');
//        LCDNUM(phcn_val);
//        LCDCHAR(' ');
        
    }
    return;
}
