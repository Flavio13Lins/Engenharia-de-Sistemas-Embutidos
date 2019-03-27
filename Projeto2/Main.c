#include <REG51F.H>

sbit P2_0 = P2^0;
sbit P2_1 = P2^1;

int transicao01(int a, int b){
	if(a & !b) {
		//val = P2_1;
		return 1;
	}
	else
		return 0;
}

void checkbit0(){

	if(P2_0 == 1)
	{
		//P1L = P0L;
		P1 = (P1 & 0xF0)|(P0 & 0x0F);
		
	}
	else
	{
		//P1L = 0;
		P1 = (P1 & 0xF0);
		
	}
	
}
		
void checkbit1(){
	if(P2_1 == 1)
	{
		//P1H = P0H;
		P1 = (P1 & 0x0F)|(P0 & 0xF0);
		
	}
	else
	{
		//P1H = 0;
		P1 = (P1 & 0x0F);
		
	}
	
}
	
void main()
{
	int val = P2_1;
	int estado = 0;
	while(1)
	{
		
		switch (estado) {
			case 0:
				if (transicao01(P2_1, val)) {
					P1 = 0x01;
					val = P2_1;
					estado = 1;
				}
				
				break;
			case 1:
				if (transicao01(P2_1, val)) {
					P1 = 0x00;
					val = P2_1;
					estado = 0;
				}
				break;	
			case 2:
				val = P2_1;
				if (transicao01(P2_1, val)) {
					P1 = 0xF0;
					estado = 0;
				}
				break;
		}
		
		//checkbit0();
		//checkbit1();
		
	}
	
}