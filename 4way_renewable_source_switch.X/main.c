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
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = ON         // Data EEPROM Memory Code Protection bit (Data EEPROM code-protected)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = ON          // Flash Program Memory Code Protection bit (All program memory code-protected)

#define _XTAL_FREQ 20000000

//INPUTS
int wind_val,solar_val,biogas_val,phcn_val;
#define wind_pindir TRISA0
#define solar_pindir TRISA1
#define biogas_pindir TRISA2
#define phcn_pindir TRISA3
//OUTPUT TUPLE
#define wind RC0
#define solar RC1
#define biogas RC2
#define phcn RC3
//STATES
enum state {initialState,morning,afternoon,night,defunct}nextState;
enum state nextState=initialState;
//VIAS
int thresh=878;
void main(void){
    ADCON1=0x80;
    ADCON0=0x41;
    wind_pindir=1;
    solar_pindir=1;
    biogas_pindir=1;
    phcn_pindir=1;
    TRISC=0x00;
    PORTC=0xF0;
    while(1){
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
            //RTC module for time
            //check time from RTC to determine nextState, for now default nextState below is morning
            nextState=morning;
            RC7=RC6=RC5=RC4=0;
            __delay_ms(100);
        }
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
    }
    return;
}
