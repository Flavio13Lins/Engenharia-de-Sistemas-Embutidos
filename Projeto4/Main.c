#include <REG51F.H>

#define FrClk 12000000

#define FreqTimer0_emHz 100

#define CORRECAO 10

#define VALOR_TH0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) >>8)

#define VALOR_TL0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)

#define VALOR_TH1 204  //RESULTA EM UMA FREQUENCIA DE 19230 Hz 

/* COMO SMOD ESTÁ COM VALOR 1 E TH1 204 O CALCUO FINAL DO BAUDRATE FOI APROXIMADAMENTE 1202bps  */


//Calcule o valor de CORRECAO!!!

sbit P2_0 = P2^0;  //Primeiro bit da Porta2
sbit P2_1 = P2^1;  //Segundo bit da Porta2
// sbit SMOD = PCON^7; //não acessivel
// sbit BIT_TRASMISSAO = TB8; //não acessivel
// sbit STOP_BIT_RECEBIDO = RB8; //não acessivel
// sbit FLAG_TRANSMISSAO = TI;  //não acessivel
// sbit FLAG_RECEPCAO = RI; //não acessivel

static unsigned char globalTimer = 0, globalTimer2 = 0;

void checkBit1();  //Acompanha mudanças no Segundo bit da Porta2
void checkBit0();  //Acompanha mudanças no Primeiro bit da Porta2
void timer0_inicializa(); //Inicializa Timer0 MODO 1
void timer0_int();
void timer1_inicializa();  //Inicializa Timer1 MODO 2
void serial1_inicializa();  //Inicializa Serial MODO 1

void main() {
	
	serial1_inicializa();
	
	timer1_inicializa();
	
	while(1){
			
	}
	
}

void timer0_inicializa(){  //MODO 1

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
		if(globalTimer2 == 0xFF){ //calcular tempo de variaçao
			globalTimer2=0;
		}
	}
	else{
		globalTimer++;
	}
	
}

void timer1_inicializa(){  //MODO 2

	TR1 = 0; // Desliga Timer0

	TMOD = (TMOD & 0x0F) | 0x20; // Timer 1 programado como timer de 8 bits

	TH1 = VALOR_TH1;	// Programa contagem do Timer1; Valor de recarga

	TL1 = VALOR_TH1;  //Valor da primeira contagem
	
	ET1 = 1; // Habilita interrupcao do timer 1

	TR1 = 1; // Habilita contagem do timer 1

}

void serial1_inicializa(){ //Inicializa Interrupção Serial Modo 1
	
	ES = 0;
	
	SCON = (SCON & 0x0F) | 0x50; //Ativa Modo 1 Serial SCON = 0101 XXXX
	
	ES = 1;
	
}
