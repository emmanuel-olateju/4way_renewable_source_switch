#include <math.h>
#define RS RB4
#define EN RB5
#define D4 RB0
#define D5 RB1
#define D6 RB2
#define D7 RB3

void clock(void){
    EN = 0;
    __delay_us(500);
    EN = 1;
    __delay_us(500);
}

void lcd_port(unsigned char DATA){
    if(DATA & 1) D4 = 1;
    else D4 = 0;
    if(DATA & 2) D5 = 1;
    else D5 = 0;
    if(DATA & 4) D6 = 1;
    else D6 = 0;
    if(DATA & 8) D7 = 1;
    else D7 = 0; 
}
void lcd_cmd(unsigned char DATA){
    RS = 0;
    lcd_port(DATA);
    clock();
}
void LCD_CLR(){
    lcd_cmd(0);
    lcd_cmd(1);
}
void LCD_CURSOR(unsigned char r, unsigned char c){
    unsigned char temp,high4,low4;
    if(r == 1){
        temp = 128 + c -1;
        high4 = (temp & 0xF0)>>4;
        low4 = (temp & 0x0F);
        lcd_cmd(high4);
        lcd_cmd(low4);
    }
    if(r==2){
    temp = 192 + c -1;
        high4 = (temp & 0xF0)>>4;
        low4 = (temp & 0x0F);
        lcd_cmd(high4);
        lcd_cmd(low4);
    }
}
void LCD_INIT(){
    TRISD = 0;
    TRISD4 = 0;
    TRISD5 = 0;
    lcd_port(0);
    __delay_ms(30);
    __delay_us(500);
    lcd_cmd(3);
    __delay_ms(5);
    lcd_cmd(3);
    __delay_us(150);
    lcd_cmd(3);
    lcd_cmd(2);
    lcd_cmd(2);
    lcd_cmd(8);
    lcd_cmd(0);
    lcd_cmd(8);
    lcd_cmd(0);
    lcd_cmd(12);
    lcd_cmd(0);
    lcd_cmd(6);
}
void LCD_WRITE_CHAR(char Data)
{
  char Low4,High4;
  Low4 = Data & 0x0F;
  High4 = Data & 0xF0;
  RS = 1;
  lcd_port(High4>>4);
  clock();
  lcd_port(Low4);
  clock();
}
void LCD_WRITE_STRING(char* stringy)
{
  unsigned char j;
  for(j=0;stringy[j]!='\0';j++)
    LCD_WRITE_CHAR(stringy[j]);
  
}
void LCD_WRITE_NUM(long i){
    char j,k,l,m;
    int n = 0;
    j=i;
    
    if(i == 0){
        RS=1;
        lcd_port(3);
        clock();
        lcd_port(0);
        clock();
    }
    while(j >= 1){
        n++;
        j = j/10;
    }
    for(n--; n>=0; n--){
        l = pow(10, n);
        k = i/l;
        m = k%10;
        
        RS = 1;
        lcd_port(3);
        clock();
        lcd_port(m);
        clock();
    }
}
void LCD_SCROLL(unsigned char dir,unsigned char spd, int ti){
    int i = 0;
    int j = 0;
    if(dir == 0){
    for(i=0; i<=ti; i++){
        for(j = 0; j<(10-spd); j++){
        __delay_ms(300);
        }
        lcd_cmd(1);
        lcd_cmd(12);     //scroll right
        j=0;
    }
    i=0;
    }
    else if(dir){
    for(i = 0; i<=ti; i++){
        for(j = 0; j<10-spd; j++){
        __delay_ms(300);
        }
        lcd_cmd(1);
        lcd_cmd(8);  // scroll left
        j=0;
    }
    }
}

