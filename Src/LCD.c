/********************************
* Название:	LCD.c				*
* Версия: beta					*
* Автор	: Malik					*
* Дата 	: 09.07.2015 12:43:51	*
* Изменён:27.07.2019 18:44:16	*
********************************/
//
//=============================== Included files =================================
#include <LCD.h>
//=================================  Variables ===================================
BYTE font[] = { 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
				0xb1,0x70,0xba,0x6f,0x63,0xbf,0xc4,0x4b,0x61,0xbd,
				0xbb,0x25,0xcc,0x20,0xac,0xa5,0x4d};
BYTE LCD_buffer[4]={0x00,0x00,0x00,0x01}; //
BYTE readbyte;
extern BYTE control; //Регистр управления
extern BYTE setup[2]; //Каналы управления
//===================================== API ======================================
void LCD_init (void) { //инициализация
	static BYTE init;
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
		case 6: //Вывод начальных значений индикатора
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
} //==============================================================================
void LCD_show (void) {
	static BYTE show,error;
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
}  //==============================================================================
void LCD_write (BYTE dat_com, BYTE data) {
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
} //==============================================================================
BYTE LCD_read (BYTE dat_com) {
	register BYTE data=0,temp=0;
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
} //==============================================================================
void convert (register BYTE znach) {
	register BYTE temp=0,num2=0;
	temp=(BYTE) setup[znach-1]/5;
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
} //==============================================================================
