#include "sys.h"                  // Device header

#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"
#include "task.h"
#endif


//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӣ�������������SYSTICKʱ��Ƶ��ΪAHB/8
//����Ϊ�˼���FreeRTOS�����Խ�SYSTICK��ʱ��Ƶ�ʸ�ΪAHB��Ƶ�ʣ�
//SYSCLK:ϵͳʱ��Ƶ��
extern void xPortSysTickHandler(void);
u32 fac_us,fac_ms;
//systick�жϷ�����,ʹ��osʱ�õ�
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        xPortSysTickHandler();	
    }
}

void delay_init(void)
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//ѡ���ⲿʱ��  HCLK
	fac_us=SystemCoreClock/1000000;				//�����Ƿ�ʹ��OS,fac_us����Ҫʹ�� 72HZ
	reload=SystemCoreClock/1000000;				//ÿ���ӵļ������� ��λΪM        72HZ
	reload*=1000000/configTICK_RATE_HZ;			//����configTICK_RATE_HZ�趨���ʱ��
												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��0.233s����	
	fac_ms=1000/configTICK_RATE_HZ;				//����OS������ʱ�����ٵ�λ	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 						//ÿ1/configTICK_RATE_HZ���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
	
	
	/*
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK)��ѡ���ⲿʱ����ΪSysTick��ʱ��Դ��
	fac_us = SystemCoreClock / 1000000������΢�루us����ϵͳʱ������֮���ת�����ӡ�
	reload = SystemCoreClock / 1000000������ÿ���ӵļ�����������λΪM��
	reload *= 1000000 / configTICK_RATE_HZ���������õ�configTICK_RATE_HZ���趨���ʱ�䡣
	fac_ms = 1000 / configTICK_RATE_HZ��������루ms����ϵͳʱ������֮���ת�����ӡ�
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk��ʹ��SysTick�жϡ�
	SysTick->LOAD = reload������SysTick������ֵ����ÿ1/configTICK_RATE_HZ���ж�һ�Ρ�
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk��ʹ��SysTick��ʱ����
	*/
}	

//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=168)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};										    
}  
//��ʱnms
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(u32 nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
	{		
		if(nms>=fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS��ʱ
		}
		nms%=fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));				//��ͨ��ʽ��ʱ
}

//��ʱnms,���������������
//nms:Ҫ��ʱ��ms��
void delay_xms(u32 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}




