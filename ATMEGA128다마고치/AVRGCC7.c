/*
 * AVRGCC1.c
 *
 * Created: 2019-11-28 오후 7:58:06
 *  Author: LG
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

static char imo1[] = ">__<";
static char imo2[] = "-__-";
static char imo3[] = "o__o";
static char imo4[] = "T__T";
static char imo5[] = "^__^";
static char boring1[] = "[boring..]";
static char hungry1[] = "[hungryTT]";
static char pow[] = "[ddong!!!]";
static char tired[] = "[tired-_-]";
static char thank[] = "[thanks^^]";
static char g_over[] = "x__x   GAME OVER";
static char morning[]="<good morning>";
static int score1="score : ";
static char socrechar[15]="0";
static int score=0;


ISR(TIMER0_OVF_vect)
{
	static int N_count = 1;
	N_count++;
	
	if(N_count%3500==0)
	{
		int count4 = 1;
		while(PINB&0x08)
		{
			count4++;
			LCDCommand(ALLCLR);
		    LCDMove(0,0);
			LCDPuts(imo2);
			LCDMove(0,6);
			LCDPuts(tired);
			_delay_ms(50);
	        PORTB=0x80;
			
		    if(count4==200)
			{
				while(1)
				{
					LCDCommand(ALLCLR);
		     		LCDMove(0,0);
					LCDPuts(g_over);
					
					LCDMove(1,0);
			        LCDPuts(score1);
			        LCDMove(1,6);
			        LCDPuts(socrechar);
					_delay_ms(1000);
				}
			}
		}
		count4= 1;
		PORTB=0x00;
		LCDCommand(ALLCLR);
		LCDCommand(DISP_OFF);
		_delay_ms(5000);
		LCDCommand(DISP_ON);
		LCDMove(0,0);
		LCDPuts(morning);
		_delay_ms(1000);
		score+=40;
		sprintf(socrechar,"%d",score);
		
	}	
	else if(N_count%2500==0)//배아파
	{
		int count3 = 1;
		while(PINB&0x04)
		{
			count3++;
			LCDCommand(ALLCLR);
		    LCDMove(0,0);
			LCDPuts(imo4);
			LCDMove(0,6);
			LCDPuts(pow);
			_delay_ms(50);
	        PORTB=0x40;
			
			
		    if(count3==200)
			{
				while(1)
				{
					LCDCommand(ALLCLR);
		     		LCDMove(0,0);
					LCDPuts(g_over);
					
					LCDMove(1,0);
			LCDPuts(score1);
			LCDMove(1,6);
			LCDPuts(socrechar);
			_delay_ms(1000);
				}
			}
		}
		count3= 1;
		PORTB=0x00;
		LCDCommand(ALLCLR);
		    LCDMove(0,0);
			LCDPuts(imo5);
			LCDMove(0,6);
			LCDPuts(thank);
			_delay_ms(1000);
			score+=30;
			sprintf(socrechar,"%d",score);
		
		
	}
	else if(N_count%1500==0)//배고파
	{
		int count2 = 1;
		while(PINB&0x02)
		{
			count2++;
			LCDCommand(ALLCLR);
		    LCDMove(0,0);
			LCDPuts(imo4);
			LCDMove(0,6);
			LCDPuts(hungry1);
			_delay_ms(50);
	        PORTB=0x20;
			
		    if(count2==200)
			{
				while(1)
				{
					LCDCommand(ALLCLR);
		     		LCDMove(0,0);
					LCDPuts(g_over);
					
					LCDMove(1,0);
			LCDPuts(score1);
			LCDMove(1,6);
			LCDPuts(socrechar);
			_delay_ms(1000);
				}
			}
		}
		count2 = 1;
		PORTB=0x00;
		LCDCommand(ALLCLR);
		
		    LCDMove(0,0);
			LCDPuts(imo5);
			LCDMove(0,6);
			LCDPuts(thank);
			_delay_ms(1000);
			score+=20;
			sprintf(socrechar,"%d",score);
			
		
	}
	else if(N_count%500==0)//심심해
	{
		int count = 1;
		while(PINB&0x01)
		{
			count++;
			LCDCommand(ALLCLR);
		    LCDMove(0,0);
			LCDPuts(imo2);
			LCDMove(0,6);
			LCDPuts(boring1);
			_delay_ms(50);
			PORTB=0x10;
		    if(count==200)
			{
				while(1)
				{
					LCDCommand(ALLCLR);
		     		LCDMove(0,0);
					LCDPuts(g_over);
					LCDMove(1,0);
					LCDPuts(score1);
			LCDMove(1,6);
			LCDPuts(socrechar);
			_delay_ms(1000);
				}
			}
			
		}
		count = 1;
		PORTB=0x00;
		LCDCommand(ALLCLR);
		    LCDMove(0,0);
			LCDPuts(imo5);
			LCDMove(0,6);
			LCDPuts(thank);
			_delay_ms(1000);
		score+=10;
		sprintf(socrechar,"%d",score);
	}
	
	TCNT0 = 100;
	EIFR=1;
}

int main(void)
{
    
		DDRB=0xF0;
		PORTB=0x00;
		TCCR0=0x00;
		TCNT0=100;
		TIMSK=(1<<TOIE0);
		sei();
		TCCR0 |= (7<<CS00);
		MCU_Init();
		LCDInit();
		
		while(1)
		{
			
			
			LCDCommand(ALLCLR);
			LCDMove(0,0);
			LCDPuts(imo3);
			LCDMove(1,0);
			LCDPuts(score1);
			LCDMove(1,6);
			LCDPuts(socrechar);
			_delay_ms(2000);

			LCDCommand(ALLCLR);
			LCDMove(0,0);
			LCDPuts(imo1);
			LCDMove(1,0);
			LCDPuts(score1);
			LCDMove(1,6);
			LCDPuts(socrechar);
			_delay_ms(2000);
			

		}  
    
}

