#include "includes.h"

u32 Var_Compensate[10]={1,2,3,4,5,6,7,8,9,0};
u32 gVar_Compensate[10];

int main()
{
	SystemInit();
//	Writeflash(0,Var_Compensate,0,6);
	while(1)
	{
		Readflash(gVar_Compensate,0,6);
	}
}
