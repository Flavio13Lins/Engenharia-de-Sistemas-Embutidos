#include <REG51F.H>

sbit P2_0 = P2^0;
sbit P2_1 = P2^1;

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

	while(1)
	{
		
		checkbit0();
		checkbit1();
		
	}
	
}