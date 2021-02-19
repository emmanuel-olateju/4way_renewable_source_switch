/*DISPLAY LIBRARY FOR A 16X4 LCD DISPLAY*/

//#include <xc.h>
//#include <string.h>
#include <math.h> 
#include <stdio.h>
//#define _XTAL_FREQ 20000000      // Tell the compiler that we are useing 16MHz

#define   l   PORTB & 0xf0   // used to retain the upper bit of portD.
#define FIRSTROW 0x80   // first row starts with 08H.
#define SECONDROW 0xC0 // second row starts with 0CH.
#define THIRDROW 0x94  // third row starts with 09H.
#define FOURTHROW 0xD4  // forth row starts with 0DH.


char logo[] = "ELECTRONICS";

long NUM;

/*
void ALA ()
     {
        PORTE = 2;
        PORTB= 4;
        CLOCK();
        PORTB= 1;
        CLOCK();
    }
*/

void delay1()
{
    __delay_ms(5);
    
    return ;
}

void delay2()
{
    __delay_ms(5);
    
    return ;
}

void CLOCK()
{
    PORTBbits.RB6 =1;     //ENABLE ON  
    delay2();
    PORTBbits.RB6 =0;       //ENABLE OFF
    delay2();
    
    return ;
}

void LCDCHAR(char character){
    unsigned char k = (0x0f&(character>>4)) ;
    unsigned char j = ( 0x0f&character );
    PORTBbits.RB4 =1;  //R/S = 1
    PORTB=k|l;
    CLOCK();
    PORTB=j|l;
    CLOCK();
}

void LCDNUM(int number){
    if(number>9){
        unsigned char t=48+(number/10);
        unsigned char u=48+(number%10);
        LCDCHAR(t);
        LCDCHAR(u);
    }else{
        LCDCHAR(number+48);
    }
}

void LCDWRITE( const char* P) ////subroutine to read a string from the program
{  
    unsigned char i ,j,k;
    for(i=0; P[i]!='\0'; i++)
    {
        k = (0x0f  &  (P[i] >> 4)) ;
        j = ( 0x0f &   P[i] );
        PORTBbits.RB4 =1;  //R/S = 1
        
        PORTB =  k | l ;
        CLOCK();
        PORTB =  j | l ;
        CLOCK();
    }
    return ;
}

void addition(long i)
{
    long k , m , n ,o ;
    int j= 1 ;
    o = i ;
    
   
    
    while(o >= 1 )
    {
        j++;
        o = o/10;
    }
    j--;
    
    for (j-- ; j>=0 ; j-- )
    {
        m =  pow (10 , j);
        k = i / m;
        n = k % 10;
                
        PORTBbits.RB4 =1;  //R/S = 1
        PORTB= 3|l;
        CLOCK();
        PORTB = n|l ;
        CLOCK();
    }
    return;
}


void NUMDISP(int num){
    
}

void CURSOR(char a, char b )
{
            RB4 = RB6 = 0;  //CLEAR RB4 and RB6
            if((a==0x80)||(0xC0)){
                PORTB=(a+b)>>4;
                CLOCK();
                PORTB=(a+b)&0x0F;
                CLOCK();
            }else{
                PORTB=(a+4+b)>>4;
                CLOCK();
                PORTB=(a+4+b)&0x0F;
                CLOCK();
            }
            
            return ;
}

void CLRDISP()
{
    RB4 = RB6 = 0 ;   // CLEAR DISPISPLAY
    PORTB= 0 | l;  //
    CLOCK();    //
    PORTB= 0x1 | l ;    //
    CLOCK();    //
    delay1();   // 
    
    return;
}

void SETCURSORTYPE()
{
    RB4 = RB6 = 0 ;   // SET THE CURSOR CHARACTERISTICS...
    PORTB= 0 | l;  //
    CLOCK();    //
    PORTB= 0xE | l ;    //
    CLOCK();    //
    delay1();   //
    
    return;
}
void config()
{
    TRISBbits.TRISB4=0;PORTBbits.RB5=0;
    PORTB= 3;
    CLOCK();
    delay1();

    PORTB= 2;
    CLOCK();
    delay1();

    PORTB= 2;
    CLOCK();
    delay1();

    PORTB= 0x8;
    CLOCK();
    delay1();

    PORTB= 0;
    CLOCK();
    PORTB= 0X0C;
    CLOCK();
    delay1();

    PORTB= 0;
    CLOCK();
    PORTB= 6;
    CLOCK();
    delay1();
    
    return ;
}


