/************************************
 * Название	: main.h				*
 * Автор	: Malik					*
 * Дата 	: 17.07.2015 16:52:05	*
 * Изменён	: **.**.2019 **:**:**	*
 ***********************************/

//Настройка МК, индикатора и датчиков
#define BIT _BV
#define IPORT PORTD
#define IPIN PIND
#define IDDR DDRD
#define BTNPORT PORTC
#define BTNPIN PINC
#define BTNDDR DDRC
#define btn_up 0
#define btn_down 1
#define btn_left 2
#define btn_right 3
#define _COMM 0
#define _DATA 1
#define RS 0
#define RW 1
#define E  2
#define DB4 4
#define DB5 5
#define DB6 6
#define DB7 7
#define IB 0
#define BF 7
//Символы дисплея
#define f_ya 10
#define f_r 11
#define f_km 12
#define f_o 13
#define f_s 14
#define f_t 15
#define f_mz 16
#define f_kb 17
#define f_a 18
#define f_n 19
#define f_l 20
#define f_proc 21
#define f_nomer 22
#define f_prob 23
#define f_sh 24
#define f_i 25
#define f_m 26
unsigned char font[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
						0xb1,0x70,0xba,0x6f,0x63,0xbf,0xc4,0x4b,0x61,0xbd,
						0xbb,0x25,0xcc,0x20,0xac,0xa5,0x4d};
//Различные переменные
volatile unsigned char control=0; //Регистр управления
volatile unsigned char time=125; //Переменная задающая время
volatile unsigned char choice=1; //Переменная выбора канала
unsigned char readbyte,key,btn_speed=50; //Различные переменные
unsigned char setup[2]={}; //Каналы управления
unsigned char LCD_buffer[4]={0x00,0x00,0x00,0x01}; //