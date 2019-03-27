#include <REG51F.H>

#define VALOR_TH1 204  //RESULTA EM UMA FREQUENCIA DE 19230 Hz 

/* COMO SMOD ESTÁ COM VALOR 1 E TH1 204 O CALCUO FINAL DO BAUDRATE FOI APROXIMADAMENTE 1202bps  */

static unsigned char BUFFER = 0x61; //Para receber o valor do serial

void timer1_inicializa();  //Inicializa Timer1 MODO 2
void serial1_inicializa();  //Inicializa Serial MODO 1
void serial1_int();

void main() {
	
	serial1_inicializa();
	
	timer1_inicializa();
	
	EA = 1; //não esquecer

	while(1){
		
	}
	
}


void timer1_inicializa(){  //MODO 2

	TR1 = 0; // Desliga Timer0
	
	PCON = (PCON & 0xFF) | 0x80;

	TMOD = (TMOD & 0x0F) | 0x20; // Timer 1 programado como timer de 8 bits

	TH1 = VALOR_TH1;	// Programa contagem do Timer1; Valor de recarga

	TL1 = VALOR_TH1;  //Valor da primeira contagem

	TR1 = 1; // Habilita contagem do timer 1

}

void serial1_inicializa(){ //Inicializa Interrupção Serial Modo 1
	
	ES = 0;
	
	SCON = (SCON & 0x0F) | 0x50; //Ativa Modo 1 Serial SCON = 0101 XXXX
	
	ES = 1;
	
}

void serial1_int() interrupt 4 using 2{
	
	if(TI == 1){  //Terminou uma transmitir
		TI=0;
	}

	if(RI == 1){  //Terminou de receber
		RI = 0;
		BUFFER = SBUF;
		SBUF = BUFFER+1;
	}
}
