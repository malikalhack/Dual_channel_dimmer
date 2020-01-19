/************************************
 * ��������	: main.h				*
 * �����	: Malik					*
 * ������	: alpha					*
 * ���� 	: 09.07.2015 12:43:51	*
  * �������	: 27.07.2019 18:44:16	*
 ***********************************/
#ifndef MAIN_H
#define MAIN_H
//=============================== Included files =================================
#include <standart.h>
#include <EEPROM.h>
#include <LCD.h>
#include <Keyboard.h>
//================================= Definition ===================================

//=============================== Prototypes API =================================
ISR (TIMER2_COMP_vect);
//=================================  Variables ===================================
BYTE control = 0; //������� ����������
volatile BYTE time = 125; //���������� �������� �����
BYTE choice = 1; //���������� ������ ������
BYTE setup[2] = {}; //������ ����������
//================================================================================
#endif /* MAIN_H */