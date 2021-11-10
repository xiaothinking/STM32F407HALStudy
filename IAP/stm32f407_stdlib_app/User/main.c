/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "sys.h"
#include "io.h"
#include "stmflash.h"
#include <stdio.h>
#include <string.h>

u8 FlashBuf[12] = {0};
u32  UpgreadeSign = 0;
u32  Chk_Sum = 0;
u32  Bin_Lengths = 0;


/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{ 
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x20000); 
  RCC_Configuration();   
	
	delay_init(168);
	
	IO_Init();
		
//	WDTFeed();
	
//	STMFLASH_Read(UPGRATDE_SIGN_ADDRESS,FlashBuf,12);

//	memcpy((u8*)&UpgreadeSign,FlashBuf,4);
//	memcpy((u8*)&Chk_Sum,&FlashBuf[4],4);
//	memcpy((u8*)&Bin_Lengths,&FlashBuf[8],4);
//	if(UpgreadeSign == 0x55555555) //升级标志置位了，则将升级区的代码拷贝到应用区
//	{
//		  STMFLASH_EraseSec(5,5);//擦除APP区
//	    Copy_Bin_Data_To_App(APP_START_ADDRESS,UPGRADE_START_ADDRESS,Bin_Lengths);
//      STMFLASH_EraseSec(7,7);//擦除扇区7  将标志位清除		  
//	}
//	iap_load_app(APP_START_ADDRESS);

	while(1)
	{
//	   WDTFeed();	
		Beep_ON;
		delay_ms(100);
		Beep_OFF;
		delay_ms(100);
	}
}




/************************END OF FILE****/
