/* -------------------------------------------------------------------------- */
/*  ATmega128_v20.H                                                                	 */
/*  I/O Address Definition & User Function for ATmega128 V20 Target Board  */
/* -------------------------------------------------------------------------- */
#define LCD_DATABUS		*((volatile unsigned char *)0x2000) 	// LCD data output address
#define LCD_CONTROL		*((volatile unsigned char *)0x2100) 	// LCD control signal output address

#define RTC_SECOND		*((unsigned char *)0x2200) 	// DS12C887 RTC
#define RTC_SEC_ALM		*((unsigned char *)0x2201)
#define RTC_MINUTE		*((unsigned char *)0x2202)
#define RTC_MIN_ALM		*((unsigned char *)0x2203)
#define RTC_HOUR		*((unsigned char *)0x2204)
#define RTC_HOUR_ALM		*((unsigned char *)0x2205)
#define RTC_WEEKDAY		*((unsigned char *)0x2206)
#define RTC_DATE		*((unsigned char *)0x2207)
#define RTC_MONTH		*((unsigned char *)0x2208)
#define RTC_YEAR		*((unsigned char *)0x2209)
#define RTC_A 			*((unsigned char *)0x220A)
#define RTC_B			*((unsigned char *)0x220B)
#define RTC_C			*((unsigned char *)0x220C)
#define RTC_D			*((unsigned char *)0x220D)
#define RTC_FLAG		*((unsigned char *)0x220E)   
#define RTC_Value		*((unsigned int  *)0x220F)   

#define PPI_PORTA		*((unsigned char *)0x2300) 	// 8255-1 address  
#define PPI_PORTB		*((unsigned char *)0x2301)
#define PPI_PORTC		*((unsigned char *)0x2302)
#define PPI_CW			*((unsigned char *)0x2303)

void MCU_initialize(void)	/* initialize ATmege128 MCU */
{ 
     MCUCR = 0x80; 	// Enable external memory and I/O, Disable SLEEP mode
     XMCRA = 0x44; 	// 0x1100 - 0x7FFF (1 wait), 0x8000 - 0xFFFF (0 wait)
     XMCRB = 0x80; 	// Enable bus keeper. Use PC0-PC7 as high byte of address bus

     DDRB  = 0xF0;  	// PORTB.0~3 = input, PORTB.4~7 = output 
     PORTB = 0x00;

     DDRD  = 0xFF;  	// PORTD = output 
     PORTD = 0x00;

     DDRE  = 0xFF;  	// PORTE = output 
     PORTE = 0x00;

     DDRF  = 0x00;  	// PORTF = Input
     PORTF = 0x00;             
     
     DDRG  = 0x1F;      
     
     LCD_CONTROL = 0x00;
     LCD_DATABUS = 0x00;
}

void Delay_us(unsigned char time_us)	/* time delay for us */
{ 
     register unsigned char i;

     for(i = 0; i < time_us; i++) {     // 4 cycle +      
        asm (" PUSH  R0 ");     	// 2 cycle +       
        asm (" POP   R0 ");       	// 2 cycle +
        asm (" PUSH  R0 ");       // 2 cycle +
        asm (" POP   R0 ");       	// 2 cycle +
        asm (" PUSH  R0 ");       // 2 cycle +
        asm (" POP   R0 ");	// 2 cycle = 16 cycle = 1 us for 16MHz
     }
}

void Delay_ms(unsigned int time_ms)       	/* time delay for ms */
{ 
     register unsigned int i;

     for(i = 0; i < time_ms; i++) {
        Delay_us(250);
        Delay_us(250);
        Delay_us(250);
        Delay_us(250);
     }
}    

void LCD_command(unsigned char command)      /* write a command(instruction) to text LCD */
{
     LCD_CONTROL = 0x00;			// E = 0, Rs = 0
     LCD_DATABUS = command;		// output command
	 LCD_CONTROL = 0x01;			// E = 1
     asm (" PUSH  R0 ");			// delay for about 250 ns
     asm (" POP   R0 ");
     LCD_CONTROL = 0x00;			// E = 0
     Delay_us(50);
}

void LCD_data(unsigned char data)	      /* display a character on text LCD */
{
	
     LCD_CONTROL = 0x02;			// E = 0, Rs = 1
     LCD_CONTROL = 0x03;			// E = 1
	 LCD_DATABUS = data;			// output data
     asm (" PUSH  R0 ");			// delay for about 250 ns
     asm (" POP   R0 ");
     LCD_CONTROL = 0x02;			// E = 0
     Delay_us(50);
}

void LCD_string(unsigned char command, unsigned char *string) /* display a string on LCD */
{
     LCD_command(command);	// start position of string
     while(*string != '\0') {		// display string
        LCD_data(*string);
        string++;
     }
}

void LCD_string_one(unsigned char command, unsigned char *string) /* display a string on LCD */
{
     LCD_command(command);	// start position of string
     while(*string != '\0') {		// display string
        LCD_data(*string);
		Delay_ms(1000);
        string++;
     }
}

void LCD_initialize(void)	/* initialize text LCD module */
{
     LCD_CONTROL = 0x03;	// E = 1, Rs = 1 (dummy write)
     LCD_CONTROL = 0x02;	// E = 0, Rs = 1
     Delay_ms(2);

     LCD_command(0x38);	// function set(8 bit, 2 line, 5x7 dot)
     LCD_command(0x0C);	// display control(display ON, cursor OFF)
     LCD_command(0x06);	// entry mode set(increment, not shift)
     LCD_command(0x01);	// clear display
     Delay_ms(2);
}

void Beep(void)	/* beep for 50 ms */
{ 
  PORTG |= _BV(3);	// buzzer on
  Delay_ms(50);	// delay 50 ms
  PORTG &= ~_BV(3);	// buzzer off
}

unsigned char KEY_INPUT(void)  		 
{
     register unsigned char key;        
     
     key = PINB & 0x0F;
     if(key == 0x0F) return key; 
     
     while(!((PINB & 0x0F) != 0x0F)) {     	
        Delay_ms(1); 
	//key = PINB & 0x0F;
     }
     Delay_ms(50);                       
     return key;
} 
