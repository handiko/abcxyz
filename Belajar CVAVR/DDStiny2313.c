/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
� Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 10/11/2018
Author  : Handiko Gesang
Company : SDL Labs
Comments: 


Chip type               : ATtiny2313
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Tiny
External RAM size       : 0
Data Stack size         : 32
*****************************************************/

#include <tiny2313.h>
#include <stdint.h>
#include <delay.h>
#include <alcd.h>

#define DATA    PORTB.0
#define W_CLK   PORTB.1
#define FQ_UD   PORTB.2
#define RST     PORTB.3

//#define DDS_CONST   4294967296
//#define SYS_CLK     180000000

#define LOW 0
#define HI  1
#define GAP_TIME    70
#define MAKE_GAP    delay_ms(GAP_TIME);

eeprom uint32_t FREQ=4234568;
char lcd[40];

void dds_reset(void)
{
    W_CLK=LOW;   
    FQ_UD=LOW;
    DATA=LOW;    
    
    RST=LOW;    MAKE_GAP;
    RST=HI;     MAKE_GAP;   delay_ms(500);
    RST=LOW; 
    
    W_CLK=LOW;  MAKE_GAP;
    W_CLK=HI;   MAKE_GAP;
    W_CLK=LOW;
    
    FQ_UD=LOW;  MAKE_GAP;
    FQ_UD=HI;   MAKE_GAP;
    FQ_UD=LOW;    
}

void write_freq(uint32_t in_freq)
{
    char i;
    
    uint32_t data_word = ((in_freq * 4294967295) / 180000000);
    
    for(i=0;i<40;i++)
    {
        DATA=((data_word >> i) & 0x01);  
        if(DATA)lcd_putchar('1');
        else    lcd_putchar('0');
        MAKE_GAP;
        W_CLK=HI;
        MAKE_GAP;
        W_CLK=LOW;
    }       
    MAKE_GAP;
    FQ_UD=HI;
    MAKE_GAP;
    FQ_UD=LOW;
}

void main(void)
{
#pragma optsize-
    CLKPR=0x80;
    CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif 
    PORTB=0x00;
    DDRB=0xFF;
    
    PORTD=0x00;
    DDRD=0xFF;

    ACSR=0x80;
    DIDR=0x00;
                    
    lcd_init(16);   
    dds_reset(); 
    
    while (1)
    {         
        write_freq(FREQ);     
        delay_ms(500);
        dds_reset(); 
        delay_ms(500);
    }
}
