#include <REG51F.H>

#define FrClk 12000000

#define FreqTimer0_emHz 100

#define CORRECAO 60000

#define VALOR_TH0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) >>8)

#define VALOR_TL0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

//Calcule o valor de CORRECAO!!!

sbit cl = P2^0;  //Primeiro bit da Porta2
sbit ch = P2^1;  //Segundo bit da Porta2
static unsigned char t = 0, globaltimer = 0;

void firstMachine();  //Acompanha mudanças no Segundo bit da Porta2
void secondMachine();  //Acompanha mudanças no Primeiro bit da Porta2
void timer0_inicializa();
void timer0_int();

void main() {
	
	timer0_inicializa();

	EA=1; //Habilita o tratamento de interrupções

	while(1) {
		firstMachine();
		secondMachine();
	}
}


void firstMachine() {
	static unsigned char state = 0;  //Considera o estado inicial na primeira inicialização
	static unsigned int counter, countermult = 0; 
	switch (state) {
		case 0:
			if (ch == 0) {
				state = 1;
			}
			break;
		case 1:
			if (ch == 1) {
				P1 = (P1 & 0x0F) | (P0 & 0xF0);
				counter = 0;
				state = 2;
			}
			break;	
		case 2:
			if (counter++ == 65000) {
				countermult+=1;
				if (countermult==5) {
					P1 = P1 & 0x0F;
					countermult = 0;
					state = 0;
				}
				counter=0;
			}
			break;
	}
}

void secondMachine() {
	static unsigned char state = 0;  //Considera o estado inicial na primeira inicialização
    static unsigned char counter;
		switch (state) {
		case 0:
			if (cl == 0) {
				state = 1;
			}
			break;
		case 1:
			if (cl == 1) {
				P1 = (P1 & 0xF0) | (P0 & 0x0F);
				timer0_int();
				counter = t;
				state = 2;
			}
			break;
		case 2:
			if (counter > 0xFA){
				if(t == 0){
					state = 0;
					P1 = P1 & 0xF0;
				}
			}
			if (counter == t+5){
				state = 0;
				P1 = P1 & 0xF0;
			}
			/*
			if (counter++ == 65000) {
				countermult+=1;
				if (countermult==5) {
					
					countermult = 0;
					
				}
				counter=0;
			}
			*/
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
	
	if(t == 0xFF){ //calcular tempo de variaçao
		t=0;
	}
	if(globaltimer == 0xFF){
		t ++;
		globaltimer = 0;
	}
	else{
		globaltimer++;
	}
	
}