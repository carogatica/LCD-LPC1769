/*
===============================================================================
 Name        : LCD-VF.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#define busDato 	LPC_GPIO0->FIODIR
#define busControl 	LPC_GPIO1->FIODIR

#define cont_Set 	LPC_GPIO1->FIOSET
#define control_Clear 	LPC_GPIO1->FIOCLR
#define RS     		20
#define RW     		19
#define EN      	18
#define datoSet		LPC_GPIO0->FIOSET
#define datoClr     LPC_GPIO0->FIOCLR
#define D0      	6
#define D1      	7
#define D2      	8
#define D3      	9
#define D4      	10
#define D5      	11
#define D6      	15
#define D7      	16
#define controlMask	((1<<RS)|(1<<RW)|(1<<EN))
#define datoMask	((1<<D7)|(1<<D6)|(1<<D5)|(1<<D4)|(1<<D3)|(1<<D2)|(1<<D1)|(1<<D0))

void retardo(int);
void LCD_Config(void);
void LCD_EnviarComando(unsigned char c);
void LCD_EnviarDato(unsigned char d);
void LCD_EnviarBit(char);

int main(void){
	SystemInit();
	LCD_Config();
	int i=0;
	char d[] = { "Hola56789012345"};
	while(1){
		/*LCD_EnviarComando(0x80);
		LCD_EnviarDato('H');
			LCD_EnviarDato('O');
			LCD_EnviarDato('L');
			LCD_EnviarDato('A');
			LCD_EnviarDato('A');
			LCD_EnviarComando(0xC0);*/
		LCD_EnviarComando(0x80);
		for (i = 0; i < 15; i++) {
				LCD_EnviarDato(d[i]);
			}
		LCD_EnviarComando(0xC0);
		i=0;
	}
	return 0;
}

void LCD_Config(){
	busDato=datoMask;
	busControl=controlMask;

	LCD_EnviarComando(0x38);  // 8 bits
	LCD_EnviarComando(0x0C); //Display ON, cursor no se ve, sin parpadeo
	LCD_EnviarComando(0x01); //borrar display
	LCD_EnviarComando(0x06);
	LCD_EnviarComando(0x80); //inicio de linea
}

void LCD_EnviarComando(unsigned char c){
	LCD_EnviarBit(c);
	control_Clear=(1<<RS)|(1<<RW);
	cont_Set=(1<<EN);
	retardo(40000);
	control_Clear=(1<<EN);
}

void LCD_EnviarBit(char b){
	datoClr=datoMask;
	datoSet|=(((b>>0)&0x01)<<D0);
	datoSet|=(((b>>1)&0x01)<<D1);
	datoSet|=(((b>>2)&0x01)<<D2);
	datoSet|=(((b>>3)&0x01)<<D3);
	datoSet|=(((b>>4)&0x01)<<D4);
	datoSet|=(((b>>5)&0x01)<<D5);
	datoSet|=(((b>>6)&0x01)<<D6);
	datoSet|=(((b>>7)&0x01)<<D7);
}

void LCD_EnviarDato(unsigned char d){
	LCD_EnviarBit(d);
	control_Clear=(1<<RS)|(1<<RW);
	cont_Set=(1<<RS)|(1<<EN);
	retardo(40000);
	control_Clear=(1<<EN);
	control_Clear=(1<<RS);
}
void retardo(int tiempo) {
	int i;
	for (i = 0; i < tiempo; i++);
	return;
}
