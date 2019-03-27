#include <REG51F.H>

sbit P2_0 = P2^0;  //Primeiro bit da Porta2
sbit P2_1 = P2^1;  //Segundo bit da Porta2

void checkBit1();  //Acompanha mudanças no Segundo bit da Porta2
void checkBit0();  //Acompanha mudanças no Primeiro bit da Porta2

void main() {

	while(1) {
		checkBit1();
		checkBit0();
	}
}


void checkBit1() {
	static unsigned char estado = 0;  //Considera o estado inicial na primeira inicialização
	static unsigned int counter, countermult = 0; 
	switch (estado) {
		case 0:
			if (P2_1 == 0) {
				estado = 1;
			}
			break;
		case 1:
			if (P2_1 == 1) {
				P1 = (P1 & 0x0F) | (P0 & 0xF0);
				counter = 0;
				estado = 2;
			}
			break;	
		case 2:
			if (counter++ == 65000) { //Loop de 1 segundo
				countermult+=1;
				if (countermult==5) {
					P1 = P1 & 0x0F;
					countermult = 0;
					estado = 0;
				}
				counter=0;
			}
			break;
	}
}

void checkBit0() {
	static unsigned char estado = 0;  //Considera o estado inicial na primeira inicialização
    static unsigned int counter, countermult = 0;
		switch (estado) {
		case 0:
			if (P2_0 == 0) {
				estado = 1;
			}
			break;
		case 1:
			if (P2_0 == 1) {
				P1 = (P1 & 0xF0) | (P0 & 0x0F);
				counter = 0;
				estado = 2;
			}
			break;
		case 2:
			
			if (counter++ == 65000) { //Loop de 1 segundo
				countermult+=1;
				if (countermult==5) {
					P1 = P1 & 0xF0;
					countermult = 0;
					estado = 0;
				}
				counter=0;
			}
			break;
	}
}
