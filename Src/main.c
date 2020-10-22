/************************************
* Title		: main.c				*
* Release	: 0.1.B					*
* Creator	: Malik					*
* CPU		: ATMega8				*
* Frequency	: 8 MHz (int)			*
* Created	: 09.07.2015 12:43:51	*
* Changed	: 22.10.2020			*
************************************/
/* Two-channel dimmer control program with display of brightness level on LCD. */
/******************************** Included files ********************************/
#include "main.h"
/********************************** Definition **********************************/

/********************************** Entry point *********************************/
int main(void) {
	cli();
	DDRB  = 0xff;
	PORTB = 0x0;
	DDRC  = 0x30;
	PORTC = 0x0f;
	IDDR  = 0xff;
	IPORT = 0x0;

	setup[0] = EEPROM_read(0x0000);
	if (setup[0] > 250) setup[0] = 250;
	setup[1] = EEPROM_read(0x0001);
	if (setup[1] > 250) setup[1] = 250;

	OCR2  = 40;
	OCR1A = setup[0];
	OCR1B = setup[1];
	TCCR1A= BIT(WGM10)|BIT(COM1A1)|BIT(COM1B1);
	TCCR1B= BIT(WGM12)|BIT(CS10);
	LCD_init();
	sei();

	while(1) {
		if (control&BIT(0)) {			// Checking the loop entry flag
			control &= ~BIT(0);			/* Reset the flag */
			if (!(control&BIT(6))) {	// Check the display initialization completion flag
				cli();
				LCD_init();				/* Call initialization of the display */
				sei();
				continue;
			}
			if (!(control&BIT(5))) {	// Check flag change value
				cli();
				/* Selecting a function (converting a value or displaying a value) */
				if (control&BIT(4)) LCD_show();
				else convert(choice);
				sei();
				continue;
			}
			if (control&BIT(1)) {		// Check the button processing flag
				cli();
				control &= ~BIT(1);		/* Reset the flag */
				time = 125;
				key_scan();
				key_action();
				sei();
			}
		}
	}
}
/***************************** Private functions ********************************/
ISR (TIMER2_COMP_vect) {
	register BYTE t_control = control;
	t_control |= BIT(0);
	if(!(--time)) t_control |= BIT(1);
	control = t_control;
}
/********************************************************************************/
