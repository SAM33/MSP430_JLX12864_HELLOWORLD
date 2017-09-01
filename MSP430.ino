#include "font6x8.h"

#define set(_register,_bit) _register|=(0x01<<_bit);
#define unset(_register,_bit) _register&=~(0x01<<_bit);


#define P1_LED       0 //P1.0
#define P1_JLX_BGLED 6 //P1.6
#define P1_JLX_CLK   5 //P1.5
#define P1_JLX_SDA   4 //P1.4
#define P1_JLX_RS    3 //P1.3
#define P1_JLX_CS    1 //P1.1
#define P1_JLX_RST   2 //P1.2

void JLX_write_command(char c){
    unset(P1OUT,P1_JLX_CS)       
    unset(P1OUT,P1_JLX_RS)   
    for(int i=0 ; i<8 ; i++)
    {
        unset(P1OUT,P1_JLX_CLK) 
        if(c&0x80)
            set(P1OUT,P1_JLX_SDA) 
        else
            unset(P1OUT,P1_JLX_SDA) 
        set(P1OUT,P1_JLX_CLK) 
        c<<=1;
    }
}

void JLX_write_data(char c){
    unset(P1OUT,P1_JLX_CS)       
    set(P1OUT,P1_JLX_RS)   
    for(int i=0 ; i<8 ; i++)
    {
        unset(P1OUT,P1_JLX_CLK) 
        if(c&0x80)
            set(P1OUT,P1_JLX_SDA) 
        else
            unset(P1OUT,P1_JLX_SDA) 
        set(P1OUT,P1_JLX_CLK) 
        c<<=1;
    }
}

void JLX_init(){
    unset(P1OUT,P1_JLX_CS)      
    unset(P1OUT,P1_JLX_RST)   
    delay(100);
    set(P1OUT,P1_JLX_RST)   
    delay(20);
    JLX_write_command(0xe2);
    delay(5);
    JLX_write_command(0x2c);
    delay(5);
    JLX_write_command(0x2e);
    delay(5);
    JLX_write_command(0x2f);
    delay(5);
    JLX_write_command(0x23);
    JLX_write_command(0x81);
    JLX_write_command(0x28);
    JLX_write_command(0xa2);
    JLX_write_command(0xc8);
    JLX_write_command(0xa0);
    JLX_write_command(0x40);
    JLX_write_command(0xaf);
    set(P1OUT,P1_JLX_CS)  
}

void JLX_address(unsigned char page , unsigned char column){
    unset(P1OUT,P1_JLX_CS) 
    JLX_write_command(0xb0+page);
    JLX_write_command(((column>>4)&0x0f)+0x10);
    JLX_write_command(column&0x0f+0x00);
}


void JLX_fullscreen()
{
    for(int i=0 ; i<8 ; i++)
    {
        unset(P1OUT,P1_JLX_CS) 
        JLX_address(i,0);
        for(int j=0 ; j<128 ; j++)
            JLX_write_data(0xff);
    }  
}

void JLX_clearscreen()
{
    for(int i=0 ; i<8 ; i++)
    {
        unset(P1OUT,P1_JLX_CS) 
        JLX_address(i,0);
        for(int j=0 ; j<128 ; j++)
            JLX_write_data(0x00);
    }  
}

void JLX_writechar(unsigned char line , unsigned char column, char c){
    JLX_address(line,column*6);
    c-=32;
    unset(P1OUT,P1_JLX_CS) 
    for(int i=0 ; i<6 ; i++)
        JLX_write_data(font6x8[c][i]);
    set(P1OUT,P1_JLX_CS) 
}

void setup() {
    // GPIOs
    unset(P1SEL,P1_LED)           // enable
    set(P1DIR,P1_LED)             // output
    unset(P1SEL,P1_JLX_BGLED)     // enable
    set(P1DIR,P1_JLX_BGLED)       // output
    unset(P1SEL,P1_JLX_CLK)       // enable
    set(P1DIR,P1_JLX_CLK)         // output
    unset(P1SEL,P1_JLX_SDA)       // enable
    set(P1DIR,P1_JLX_SDA)         // output
    unset(P1SEL,P1_JLX_SDA)       // enable
    set(P1DIR,P1_JLX_SDA)         // output
    unset(P1SEL,P1_JLX_RS)        // enable
    set(P1DIR,P1_JLX_RS)          // output
    unset(P1SEL,P1_JLX_CS)        // enable
    set(P1DIR,P1_JLX_CS)          // output
    unset(P1SEL,P1_JLX_RST)       // enable
    set(P1DIR,P1_JLX_RST)         // output
    P1OUT = 0x00;                 // unset all bits in P1OUT
    // JLX
    JLX_init();
}

void HelloWorld(){
    JLX_clearscreen();
    delay(500);   
    set(P1OUT,P1_JLX_BGLED)
    JLX_writechar(1,5,'H');
    delay(500);
    JLX_writechar(1,6,'E');
    delay(500);
    JLX_writechar(1,7,'L');
    delay(500);
    JLX_writechar(1,8,'L');
    delay(500);    
    JLX_writechar(1,9,'O');
    delay(500);    
    JLX_writechar(3,7,'W');
    delay(500);
    JLX_writechar(3,8,'O');
    delay(500);
    JLX_writechar(3,9,'R');
    delay(500);
    JLX_writechar(3,10,'L');
    delay(500);    
    JLX_writechar(3,11,'D');
    delay(500);        
    unset(P1OUT,P1_JLX_BGLED)
    delay(500);   
  
}

void loop() {
    HelloWorld();
    delay(3000);   
}
