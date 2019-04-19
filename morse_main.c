#include <xmc_common.h>	//includes device header file and telling names for registers
#include <xmc_gpio.h>
#include <xmc4_gpio.h>
#include <stdio.h>

static size_t systick = 0; 
size_t timer2 = 0;
size_t timer1 = 0;
size_t timer0 = 0;
unsigned int button1_pressed = 0;
enum LEDstate {off,on};
 
void init(void)
{
	PORT1->IOCR0 |= XMC_GPIO_MODE_OUTPUT_PUSH_PULL << 8;	//initialize LED1
	PORT1->IOCR12 |= XMC_GPIO_MODE_INPUT_TRISTATE << 2*8;	//initialize Button P1.14 and P1.15, returns 0 if pressed
	PORT1->IOCR12 |= XMC_GPIO_MODE_INPUT_TRISTATE << 3*8;	
	SysTick_Config(SystemCoreClock/1000);					//120,000 ticks = 1ms
}

void morsing_code(void);
void LED(enum LEDstate state, unsigned int factor);			//set Pinstate and waiting-time in ms
void waittime_calculation(unsigned int operand);
void morsing_numbers(unsigned int x);

int main(void)
{
	SystemInit();		// check --> CMSIS/Infineon/XMC4500_series/Source
	init();

	do{
		PORT1->OUT = XMC_GPIO_OUTPUT_LEVEL_LOW << 1;
		if( !(PORT1->IN & PORT1_IN_P14_Msk))
		{	
			if( button1_pressed == 0)
			{
				timer0 = systick;			
				button1_pressed++;
			}
			else if(button1_pressed != 0)
			{	
				timer1 = timer2;	
				timer2 = systick;
				button1_pressed++;
			}
			
			morsing_code();
		}
		else if( !(PORT1->IN & PORT1_IN_P15_Msk))
		{
			if( button1_pressed == 0)
			{
				morsing_numbers(0);
			}
			else if( button1_pressed == 1)
			{
				waittime_calculation(timer0);
			}
			else
			{
				waittime_calculation((unsigned int)(timer2 - timer1)); 
			}			
		}	
		
	}while(true);

	return 0;
}


void SysTick_Handler(void)
{
	++systick;
}


void LED(enum LEDstate state, unsigned int factor)
{
	unsigned int temp;
	if(state == on)
	{
		PORT1->OUT = XMC_GPIO_OUTPUT_LEVEL_HIGH << 1;
	}
	else
	{
		PORT1->OUT = XMC_GPIO_OUTPUT_LEVEL_LOW << 1;
	}
	temp = systick;
	systick = 0; 
	while(systick < factor);
	systick += (temp+1);
}

void morsing_code(void)
{
	// I
	LED(on,100);
	LED(off,100);
	LED(on,100);

	//space
	LED(off,700);	

	//CAN
	LED(on,300);
	LED(off,100);
	LED(on,100);
	LED(off,100);
	LED(on,300);
	LED(off,100);
	LED(on,100);

	LED(off,300);

	LED(on,100);
	LED(off,100);
	LED(on,300);

	LED(off,300);

	LED(on,300);
	LED(off,100);
	LED(on,100);

	//space
	LED(off,700);

	//MORSE
	LED(on,300);
	LED(off,100);
	LED(on,300);

	LED(off,300);

	LED(on,300);
	LED(off,100);
	LED(on,300);
	LED(off,100);
	LED(on,300);

	LED(off,300);
	
	LED(on,100);
	LED(off,100);
	LED(on,300);
	LED(off,100);
	LED(on,100);

	LED(off,300);

	LED(on,100);
	LED(off,100);
	LED(on,100);
	LED(off,100);
	LED(on,100);

	LED(off,300);
	
	LED(on,100);
	
}

//timer waittime_ calculation
void waittime_calculation(unsigned int operand)
{
	char c_digits[6];					
	sprintf(c_digits,"%d",operand);
	unsigned int digits = strlen(c_digits);
	unsigned int x = 0;
	while( x < digits )
	{
		morsing_numbers((unsigned int)c_digits[x]-48);
		++x;	
	}
}

//Morse-numbers:
void morsing_numbers(unsigned int x)
{
	switch(x) {
		case 0	: 
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,300);
				break;
		
		case 1	:
				LED(on,100);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,300);
				break;
		case 2	:
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,300);
				break;
		case 3	:
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,300);
				break;

		case 4	: 
			 	LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,300);
				LED(off,300);
				break;
		case 5	:
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,300);
				break;
		case 6	:
				LED(on,300);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,300);
				break;
		case 7	:
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,300);
				break;
		case 8	: 	
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,100);
				LED(off,100);
				LED(on,100);
				LED(off,300);
				break;
		case 9	:
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,300);
				LED(off,100);
				LED(on,100);
				LED(off,300);
				break;

		default :	break;
		}
}

