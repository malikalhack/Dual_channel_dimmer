/********************************
* Название: Indicator.c			*
* Версия  : alpha				*
* Автор	  : Malik				*
* Создан  : 09.07.2015 12:43:51	*
* Изменён : **.**.2019 **:**:**	*
* Для AVR: ATMEGA8				*
* Тактовая частота: 8МГц (int)	*
********************************/

#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"

ISR (TIMER2_COMP_vect) {
	control|=BIT(0);
	if(!(--time)) control|=BIT(1);
}

unsigned char EEPROM_read(unsigned int Addr) {
	while(EECR & (1<<EEWE));
	EEAR = Addr;
	EECR |= (1<<EERE);
	return EEDR;
}

void EEPROM_write(unsigned int Addr, unsigned char Data) {
	while(EECR & (1<<EEWE));
	EEAR = Addr;
	EEDR = Data;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}

void LCD_write (unsigned char dat_com, unsigned char data) {
	IPORT=0;
	IPORT|=(data&0xf0);
	if (dat_com) IPORT|=BIT(RS);
	else IPORT&=~BIT(RS);
	IPORT|=BIT(E);
	asm("nop");
	asm("nop");
	IPORT&=~BIT(E);
	IPORT&=0x0f;
	IPORT|=(data&0x0f)*16;
	IPORT|=BIT(E);
	asm("nop");
	asm("nop");
	IPORT&=~BIT(E);
	_delay_us(1);
	return;
}

unsigned char LCD_read (unsigned char dat_com) {	
	register unsigned char data=0,temp=0;
	IDDR=0b00001111;
	IPORT=0b11110010;
	if (dat_com) IPORT|=BIT(RS);
	else IPORT&=~BIT(RS);
	IPORT|=BIT(E);
	asm("nop");
	temp=IPIN;
	IPORT&=~BIT(E);
	data=(temp&0xf0);
	_delay_us(1);
	IPORT|=BIT(E);
	asm("nop");
	temp=IPIN;
	IPORT&=~BIT(E);
	temp/=16;
	data|=temp;
	_delay_us(1);
	IPORT=0;
	IDDR=0b11111111;
	return data;
}

void LCD_init (void) { //инициализация
	static unsigned char init;
	switch (init) {
	case 0: //установка параметров
		_delay_ms(40);
		TIMSK=BIT(OCIE2);
		TCCR2=BIT(CS21);
		IPORT = 0b00110000;
		IPORT|=BIT(E);
		asm("nop");
		asm("nop");
		IPORT&=~BIT(E);
		init++;
		TCNT2=0;
	break;
	case 1 ... 2: //установка параметров
		//0b0010NF**
		//бит 3-кол-во линий (0/1 - одна/две)
		//бит 2-тип шрифта (0/1 - 5*8/5*11)
		LCD_write(_COMM,0b00101100);
		init++;
		TCNT2=0;
	break;
	case 3: //включение дисплея
		//0b00001DCB
		//бит 2-вкл/откл дисплей
		//бит 1-вкл/откл курсор
		//бит 0-мерцание курсора
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_COMM,0b00001100);
		init++;
		OCR2=37;
		TCNT2=0;
	break;
	case 4: //очистка дисплея
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_COMM,0b00000001);
		init++;
		TCNT2=0;
	break;
	case 5: //установка режима работы
		//0b000001IS
		//бит 1-назначить курсору направление движения (0/1-влево/вправо)
		//бит 0-назначить дисплею направление сдвига (0-сдвига нет)
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_COMM,0b00000110);
		init++;
		TCNT2=0;
	break;
	//Вывод начальных значений индикатора
	case 6:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_sh]);
		init++;
		TCNT2=0;
	break;
	case 7:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_i]);
		init++;
		TCNT2=0;
	break;
	case 8:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_m]);
		init++;
		TCNT2=0;
	break;
	case 9:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_COMM,0b10000111);
		init++;
		TCNT2=0;
	break;
	case 10:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_proc]);
		init++;
		TCNT2=0;
	break;
	case 11:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_COMM,0b11000000);
		init++;
		TCNT2=0;
	break;
	case 12:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_kb]);
		init++;
		TCNT2=0;
	break;
	case 13:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_a]);
		init++;
		TCNT2=0;
	break;
	case 14:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_n]);
		init++;
		TCNT2=0;
	break;
	case 15:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_a]);
		init++;
		TCNT2=0;
	break;
	case 16:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_l]);
		init++;
		TCNT2=0;
	break;
	case 17:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_prob]);
		init++;
		TCNT2=0;
	break;
	case 18:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_DATA,font[f_nomer]);
		init++;
		TCNT2=0;
	break;
	case 19:
		readbyte=LCD_read(_COMM);
		if (readbyte&BIT(BF)) break; //Проверка флага занятости
		LCD_write(_COMM,0b10000100);
		control|=BIT(6);
		init=0;
		TCNT2=0;
	break;
	default: 
		init=0;
		TCNT2=0;
	}
	return;
}

void convert (register unsigned char znach) {
	register unsigned char temp=0,num2=0;
 	temp=(unsigned char) setup[znach-1]/5;
	temp*=2;
	if (temp==100) {
		LCD_buffer[0]=1;
		LCD_buffer[1]=LCD_buffer[2]=0;
	}
	else {
		LCD_buffer[0]=0;
		while (temp>=10) {
			temp-=10;
			num2++;
		}
		LCD_buffer[1]=num2;
		LCD_buffer[2]=temp;
	}
	LCD_buffer[3]=znach;
	control|=BIT(4); //Установить флаг разрешения вывода на дисплей
	return;
}

void LCD_show (void) {
	static unsigned char show,error;
	if (error==10) {
		control|=BIT(6);
		return;
	}
	switch (show) {
	case 0:
		readbyte=LCD_read(_COMM); //Проверка флага занятости
		if (readbyte&BIT(BF)) {
			++error;
			break;
		}
		if(LCD_buffer[0]==0) {
			LCD_write(_DATA,font[f_prob]);
		}
		else {
			LCD_write(_DATA,font[1]);
		}
		show++;
		TCNT2=0;
	break;
	case 1:
		readbyte=LCD_read(_COMM); //Проверка флага занятости
		if (readbyte&BIT(BF)) {
			++error;
			break;
		}
		if((LCD_buffer[0]==0)&&(LCD_buffer[1]==0)) {
			LCD_write(_DATA,font[f_prob]);
		}
		else {
			LCD_write(_DATA,font[LCD_buffer[1]]);
		}
		show++;
		TCNT2=0;
	break;
	case 2:
		readbyte=LCD_read(_COMM); //Проверка флага занятости
		if (readbyte&BIT(BF)) {
			++error;
			break;
		}
		LCD_write(_DATA,font[LCD_buffer[2]]);
		show++;
		TCNT2=0;
	break;
	case 3:
		readbyte=LCD_read(_COMM); //Проверка флага занятости
		if (readbyte&BIT(BF)) {
			++error;
			break;
		}
		LCD_write(_COMM,0b11000111);
		show++;
		TCNT2=0;
	break;
	case 4:
		readbyte=LCD_read(_COMM); //Проверка флага занятости
		if (readbyte&BIT(BF)) {
			++error;
			break;
		}
		LCD_write(_DATA,font[LCD_buffer[3]]);
		show++;
		TCNT2=0;
	break;
	case 5:
		readbyte=LCD_read(_COMM); //Проверка флага занятости
		if (readbyte&BIT(BF)) {
			++error;
			break;
		}
		LCD_write(_COMM,0b10000100);
		show=0;
		control|=BIT(5);
		TCNT2=0;
	break;
	}
}

void key_scan (void) {
	register unsigned char pin=0;
	key=0;
	static unsigned char anti_dr;
	pin=BTNPIN|0xf0;
	if (pin!=0xff) {
		if (++anti_dr==5) key=~pin;
		if (anti_dr==btn_speed) {
			anti_dr=0;
			btn_speed=10;
		}
	}
	else {
		if (control&BIT(3)) {
			if (choice==1) 	{
				EEPROM_write(0x0000, setup[choice-1]);
			}
			else {
				EEPROM_write(0x0001, setup[choice-1]);
			}
		}
		btn_speed=50;
		anti_dr=0;
	}
	return;
}

void key_action (void) {
	if (key) {
		if (key&BIT(btn_down)) {
			if (setup[choice-1]>=5) {
				setup[choice-1]-=5;
				control|=BIT(3);
				control&=~(BIT(5)|BIT(4));
				if(choice-1) OCR1B=setup[1];
				else OCR1A=setup[0];
			}
			return;
		}
		if (key&BIT(btn_up)) {
			if (setup[choice-1]<=245) {
				setup[choice-1]+=5;
				control|=BIT(3);
				control&=~(BIT(5)|BIT(4));
				if(choice-1) OCR1B=setup[1];
				else OCR1A=setup[0];
			}
			return;
		}
		if (key&BIT(btn_right)) {
			if (choice<2) {
				choice++;
				control&=~(BIT(5)|BIT(4));
			}
			return;
		}
		if (key&BIT(btn_left)) {
			if (choice>1) {
				choice--;
				control&=~(BIT(5)|BIT(4));
			}
			return;
		}
	}
	return;
}

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
			if (control&BIT(0)) { //Проверка флага входа в цикл
					control&=~BIT(0); //Сброс флага
					if (!(control&BIT(6))) { //Проверка флага завершения инициализации дисплея
						cli();
						LCD_init(); //Вызов инициализации дисплея
						sei();
						continue;
					}
					if (!(control&BIT(5))) { //Проверка флага изменение значения
						cli();
						//Выбор функции (конвертация значения или вывод значения на дисплей)
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
}