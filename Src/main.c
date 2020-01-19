/********************************
* ��������: Indicator.c			*
* ������  : beta				*
* �����	  : Malik				*
* ������  : 09.07.2015 12:43:51	*
* ������ : 27.07.2019 18:44:16	*
* ��� AVR: ATMEGA8				*
* �������� �������: 8��� (int)	*
********************************/
//��������� ���������� ������������� �������� � ������������ ������ ������� �� LCD
//=============================== Included files =================================
#include "main.h"
//================================= Entry point ==================================
int main(void) {
	cli();
	DDRB = 0xff;
	PORTB = 0x0;
	DDRC = 0x30;
	PORTC = 0x0f;
	IDDR = 0xff;
	IPORT = 0x0;
	setup[0]=EEPROM_read(0x0000);
	if (setup[0]>250) setup[0]=250;
	setup[1]=EEPROM_read(0x0001);
	if (setup[1]>250) setup[1]=250;
	OCR2=40;
	OCR1A=setup[0];
	OCR1B=setup[1];
	TCCR1A=BIT(WGM10)|BIT(COM1A1)|BIT(COM1B1);
	TCCR1B=BIT(WGM12)|BIT(CS10);
	LCD_init();
	sei();
	while(1) {
		if (control&BIT(0)) { //�������� ����� ����� � ����
			control&=~BIT(0); //����� �����
			if (!(control&BIT(6))) { //�������� ����� ���������� ������������� �������
				cli();
				LCD_init(); //����� ������������� �������
				sei();
				continue;
			}
			if (!(control&BIT(5))) { //�������� ����� ��������� ��������
				cli();
				//����� ������� (����������� �������� ��� ����� �������� �� �������)
				if (control&BIT(4)) LCD_show();
				else convert(choice);
				sei();
				continue;
			}
			if (control&BIT(1)) {
				cli();
				control&=~BIT(1);
				time=125;
				key_scan();
				key_action();
				sei();
			}
		}
	}
} //=========================== Private functions ================================
ISR (TIMER2_COMP_vect) {
	register BYTE temp_control =control;
	temp_control|=BIT(0);
	if(!(--time)) temp_control|=BIT(1);
	control = temp_control;
} //==============================================================================