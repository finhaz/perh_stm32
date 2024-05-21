#include "Flash.h"
u32 StartAddr = 0x0801F800;  //Ҫд��Flash�����ݵ��׵�ַ--FLASH��ʼ��ַ1F800=1024*126
u32 EndAddr  = 0x0801FFFF;  //Ҫд��Flash�����ݵ�ĩ��ַ--FLASH������ַ1FFFF=1024*127
u32 FlashAddress=0x00;//Flash���ڲ���ַ	 
vu32 NbrOfPage = 0x00; //Ҫ������ҳ������
u32 data = 0;
//u32 *p=(u32 *)0x08008000; //����ָ��ָ��Ҫ���͵����ݵĵ�ַ
volatile FLASH_Status FLASHStatus;
volatile TestStatus MemoryProgramStatus;
ErrorStatus HSEStartUpStatus;

/*******************************************************************************
* Function Name  : Writeflash
* Description    : д���� �����ݴ�CPUд��FLASH��
*                
* Input          : u8 Erasenumber,u32 *Data������Ҫ������ҳ���Ҫд������ݵĵ�ַ
* Output         : None
* Return         : None
*******************************************************************************/
void Writeflash(u8 Erasenumber ,u32 *p,u8 start,u8 end)
{
		int i = start;
		FLASHStatus = FLASH_COMPLETE;
		MemoryProgramStatus = PASSED;

		/* Unlock the Flash Program Erase controller */
		FLASH_Unlock();	//FLASH����

		/* Clear All pending flags */
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//���־λ

		//������д����		
		//*******************************************************************************  
		//һ�β���1ҳ
		/* Define the number of page to be erased *///����Ҫ������ҳ�������
		NbrOfPage = (EndAddr - StartAddr) / FLASH_PAGE_SIZE;
		/* Erase the FLASH pages *///ҳ������ӳ���
		FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * NbrOfPage));
		//д����
		FlashAddress = StartAddr+4*start;

		while((FlashAddress < EndAddr) && (FLASHStatus == FLASH_COMPLETE) && i<end)
		{
			FLASHStatus = FLASH_ProgramWord(FlashAddress, *(p+i));//*p);
			i++;
			FlashAddress = FlashAddress + 4;
		}
}

/*******************************************************************************
* Function Name  : Readflash
* Description    : �����ݣ���FLASH�ж�����Ҫ������
*                
* Input          : None
* Output         : Data���Ҫȡ��������
* Return         : None
*******************************************************************************/

void Readflash(u32 *p,u8 start,u8 end) //
{
		int j = start;
		FlashAddress = StartAddr+4*start;
		//������

		while(j<end)
		{
			*(p+j) = *(u32*)(FlashAddress+4*j);
			j++;
		}
}
