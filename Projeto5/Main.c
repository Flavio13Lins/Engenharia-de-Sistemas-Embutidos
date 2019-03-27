#include <REG51F.H>

#define TAMANHO_VETOR 16

#define VALOR_TH1 204  //RESULTA EM UMA FREQUENCIA DE 19230 Hz 

/* COMO SMOD ESTÁ COM VALOR 1 E TH1 204 O CALCUO FINAL DO BAUDRATE FOI APROXIMADAMENTE 1200bps  */

unsigned char BUFFER_CT[TAMANHO_VETOR], BUFFER_CR[TAMANHO_VETOR]; //Para receber o valor do serial
unsigned char tin = 0, tout = 0, rin = 0, rout = 0;
bit TX_BUSY = 0;

void timer1_inicializa();  //Inicializa Timer1 MODO 2
void serial1_inicializa();  //Inicializa Serial MODO 1
void serial1_int();
void send_char(char c);

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
		
		//TRANSMITIR DO BUFFER_CT
		
	}

	if(RI == 1){  //Terminou de receber
		
		RI = 0;
		
		//RECEBER DO BUFFER_CB
		
	}
}

void send_char(char c){
	
	if( (tout + 1)%TAMANHO_VETOR != tin){
		
		BUFFER_CT[tout] = c;
		
	}	
	if(!TX_BUSY){
		
		TI = 1;
	
		TX_BUSY;
		
	}
	
}

void send_string(char *s){
	
	
	
}

bit rx_buffer_vazio(){
	
	if(rin == rout){
		return 1;
	}
	else{
		return 0;
	}
	
}

void receive_char(){
	
	
	
}

void receive_string(char *s){
	
	
	
}
