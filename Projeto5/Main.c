#include <REG51F.H>

#define TAMANHO_VETOR 16

#define VALOR_TH1 204  //RESULTA EM UMA FREQUENCIA DE 19230 Hz 

/* COMO SMOD ESTÁ COM VALOR 1 E TH1 204 O CALCUO FINAL DO BAUDRATE FOI APROXIMADAMENTE 1200bps  */

unsigned char BUFFER_CT[TAMANHO_VETOR], BUFFER_CR[TAMANHO_VETOR]; //Para receber o valor do serial
unsigned char tin = 0, tout = 0, rin = 0, rout = 0;
bit TX_BUSY = 0;

void timer1_inicializa();  //Inicializa timer1 no MODO 2
void serial1_inicializa();  //Inicializa Serial MODO 1
void serial1_int();
void send_char(char c); 
void send_string(char *s);  //Utiliza send_char para enviar uma string
bit rx_buffer_vazio();  //Verifica se o BUFFER_CR está vazio (não existe o que recever)
unsigned char receive_char();
void receive_string(char *s); //Utiliza send_char para enviar uma string

void main() {
	unsigned char a = 'p';
	serial1_inicializa();
	
	timer1_inicializa();
	
	//BUFFER_CR[0] = 'a';
	//rout = 1;
	//BUFFER_CT[0] = 'x';
	
	EA = 1; //não esquecer

	while(1){
		
		a = receive_char();
		
		send_char(a);
		
		/*if (a =! '\0'){
			
			P1 = a;
			
			send_char(a);
			
		}*/
		
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
		
		if(tout != tin){
			
			SBUF = BUFFER_CT[tout];
			
			tout = (tout + 1) % TAMANHO_VETOR;
			
		}else{
			TX_BUSY = 0;
		}
		
		//TRANSMITIR DO BUFFER_CT
		
	}

	if(RI == 1){  //Terminou de receber
		
		RI = 0;
		if (!rx_buffer_vazio()){
			rout = (rout + 1) % TAMANHO_VETOR;
			BUFFER_CR[rout] = SBUF;
			
			
			
		}
		
	}
}

void send_char(char c){
	
	if( (tout + 1) % TAMANHO_VETOR != tin){
		tout = (tout + 1) % TAMANHO_VETOR;
		BUFFER_CT[tout] = c;
		
		
		
	}	
	if(!TX_BUSY){
		
		TX_BUSY = 1;
		
		TI = 1;
		
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

unsigned char receive_char(){
	
	char c;
	
	if (!rx_buffer_vazio()){
		rin = (rin + 1) % TAMANHO_VETOR;
		c = BUFFER_CR[rin];
		
		
			
		return c;
		
	}else {
		return '\0';
	}
	
}

void receive_string(char *s){
	
	
	
}
