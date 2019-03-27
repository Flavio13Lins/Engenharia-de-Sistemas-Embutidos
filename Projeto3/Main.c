#include <REG51F.H>

#define FrClk 12000000

#define FreqTimer0_emHz 100

#define CORRECAO 10

#define VALOR_TH0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) >>8)

#define VALOR_TL0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

//Calcule o valor de CORRECAO!!!

sbit P2_0 = P2^0;  //Primeiro bit da Porta2
sbit P2_1 = P2^1;  //Segundo bit da Porta2
static unsigned char globalTimer = 0, globalTimer2 = 0;

void checkBit1();  //Acompanha mudanças no Segundo bit da Porta2
void checkBit0();  //Acompanha mudanças no Primeiro bit da Porta2
void timer0_inicializa();
void timer0_int();

void main() {
	
	timer0_inicializa();

	EA=1; //Habilita o tratamento de interrupções

	while(1) {
		checkBit1();
		checkBit0();
	}
}


void checkBit1() {
	static unsigned char estado = 0;  //Considera o estado inicial na primeira inicialização
	static unsigned int counter; 
	switch (estado) {
		case 0:
			if (P2_1 == 0) {
				estado = 1;
			}
			break;
		case 1:
			if (P2_1 == 1) {
				P1 = (P1 & 0x0F) | (P0 & 0xF0);
				timer0_int();
				counter = globalTimer2;
				estado = 2;
			}
			break;	
		case 2:
			if (counter >= 246){  //Caso gambiarra exclusivo para globalTimer2+10 estourar o reg
				if(globalTimer2 == 0x10){
					estado = 0;
					P1 = P1 & 0x0F;
				}
			}
			if (counter == globalTimer2-10){  //Aproximadamente 1 segundo
				estado = 0;
				P1 = P1 & 0x0F;
			}
			break;
	}
}

void checkBit0() {
	static unsigned char estado = 0;  //Considera o estado inicial na primeira inicialização
    static unsigned char counter;
		switch (estado) {
		case 0:
			if (P2_0 == 0) {
				estado = 1;
			}
			break;
		case 1:
			if (P2_0 == 1) {
				P1 = (P1 & 0xF0) | (P0 & 0x0F);
				timer0_int();
				counter = globalTimer2;
				estado = 2;
			}
			break;
		case 2:
			if (counter >= 246){
				if(globalTimer2 == 0x10){
					estado = 0;
					P1 = P1 & 0xF0;
				}
			}
			if (counter == globalTimer2-10){
				estado = 0;
				P1 = P1 & 0xF0;
			}
			break;
	}
}


void timer0_inicializa(){

	TR0 = 0; // Desliga Timer0

	TMOD = (TMOD & 0xF0) | 0x01; // Timer 0 programado como timer de 16 bits

	TH0 = VALOR_TH0; // Programa contagem do Timer0

	TL0 = VALOR_TL0;

	ET0 = 1; // Habilita interrupcao do timer 0

	TR0 = 1; // Habilita contagem do timer 0

}

void timer0_int (void) interrupt 1 using 2{
	
	TR0 = 0; // Desliga Timer0
		
	TL0 += VALOR_TL0;
		
	TH0 += VALOR_TH0 + (unsigned char) CY; // Programa contagem do Timer0

	TR0 = 1; // Habilita contagem do timer 0
	

	if(globalTimer == 0xFF){
		globalTimer2 ++;
		globalTimer = 0;
		if(globalTimer2 == 0xFF){ 
			globalTimer2=0;
		}
	}
	else{
		globalTimer++;
	}
	
}
