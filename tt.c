#ifndef F_CPU
#define F_CPU 1200000UL
//#define F_CPU 9600000
#endif

//#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
//#include <avr/interrupt.h>


uint8_t tiempo = 0;
char unidad;

#define BUTTON_D1_CH PB0
#define BUTTON_D2_SH PB1
#define BUTTON_START PB2
#define SHUTTER PB3
#define LED PB4
#define DEBOUNCE_TIME 25
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))

void
blink(){
    _delay_ms(1000);
    SETBIT(PORTB, LED);
    _delay_ms(1000);
    CLEARBIT(PORTB, LED);
}

int
button_is_pressed(int button)
{
    /* the button is pressed when BUTTON_BIT is clear */
    if (bit_is_clear(PINB, button))
    {
        SETBIT(PORTB, LED);
        _delay_ms(DEBOUNCE_TIME);
        CLEARBIT(PORTB, LED);
        if (bit_is_clear(PINB, button)) return 1;
    }
    return 0;
}

void
retardoSegundos(int segundos)
{
  int i;
  for(i = 0; i < segundos; i++)
    _delay_ms(1000);
}
void
retardoMinutos(int minutos)
{
  int i;
  for(i = 0; i < minutos; i++)
    retardoSegundos(60);
}

void
iniciarTimelapse(){
    while(button_is_pressed(BUTTON_START));
    blink();
    while(!button_is_pressed(BUTTON_START)){
        SETBIT (PORTB, SHUTTER);
        _delay_ms(300);
        CLEARBIT (PORTB, SHUTTER);
        if(unidad == 'S')
            retardoSegundos(tiempo);
        else
            retardoMinutos(tiempo);
    }
    blink();
    tiempo = 0;
}


int
main(){
    //RB = 0b00011000; //only SHUTTER and LED as outputs
    // output pins
    DDRB |= (1 << LED);     // LED on pin 11 (PD6)
    DDRB |= (1 << SHUTTER);     // LED on pin 11 (PD6)
    //input pinb
    DDRB &= ~(1 << BUTTON_D1_CH);
    DDRB &= ~(1 << BUTTON_D2_SH);
    DDRB &= ~(1 << BUTTON_START);


    PORTB |= _BV(BUTTON_D1_CH);//enable pull-up resistors
    PORTB |= _BV(BUTTON_D2_SH);
    PORTB |= _BV(BUTTON_START);
    while(1){
        if(button_is_pressed(BUTTON_D1_CH)){
            unidad = 'S';
            while(button_is_pressed(BUTTON_D1_CH));
            break;
        }
        if(button_is_pressed(BUTTON_D2_SH)){
            unidad = 'M';
            while(button_is_pressed(BUTTON_D2_SH));
            break;
        }
    }
    blink();

    while(1){
        if(button_is_pressed(BUTTON_START)){
           iniciarTimelapse();
        }//else
        if(button_is_pressed(BUTTON_D1_CH)){
            tiempo++;
            while(button_is_pressed(BUTTON_D1_CH));
        }//else
        if(button_is_pressed(BUTTON_D2_SH)){
            tiempo = tiempo + 10;
            while(button_is_pressed(BUTTON_D2_SH));
        }
    }
}
