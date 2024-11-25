#include "SYS.h"

extern void xPortSysTickHandler(void);
u32 fac_us,fac_ms;
//systick中断服务函数,使用os时用到
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}

//初始化延迟函数
//SYSTICK 的时钟固定为 AHB 时钟，基础例程里面 SYSTICK 时钟频率为 AHB/8
//这里为了兼容 FreeRTOS，所以将 SYSTICK 的时钟频率改为 AHB 的频率！
//SYSCLK:系统时钟频率
void Delay_init(void)
{
    u32 reload;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//选择外部时钟 HCLK
    fac_us=SystemCoreClock/1000000;              //不论是否使用 OS,fac_us 都需要使用
    reload=SystemCoreClock/1000000;              //每秒钟的计数次数 单位为 M
    reload*=1000000/configTICK_RATE_HZ;          //根据 configTICK_RATE_HZ 设定溢出
     //时间 reload 为 24 位寄存器,最大值；16777216,在 72M 下,约合 0.233s 左右
    fac_ms=1000/configTICK_RATE_HZ;               //代表 OS 可以延时的最少单位
    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;      //开启 SYSTICK 中断
    SysTick->LOAD=reload;                         //每 1/configTICK_RATE_HZ 秒中断一次
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;       //开启 SYSTICK
}
	
	//延时 nus
//nus:要延时的 us 数.
//nus:0~204522252(最大值即 2^32/fac_us@fac_us=168)
void Delay_us(u32 nus)
{
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD; //LOAD 的值
    ticks=nus*fac_us; //需要的节拍数
    told=SysTick->VAL; //刚进入时的计数器值
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            //这里注意一下 SYSTICK 是一个递减的计数器就可以了.
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break; //时间超过/等于要延迟的时间,则退出.
        }
    };
}

//延时 nms,会引起任务调度
//nms:要延时的 ms 数
//nms:0~65535
void Delay_ms(u32 nms)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        if(nms>=fac_ms) //延时的时间大于 OS 的最少时间周期
        {
            vTaskDelay(nms/fac_ms); //FreeRTOS 延时
        }
        nms%=fac_ms; //OS 已经无法提供这么小的延时了,
        //采用普通方式延时
    }
    Delay_us((u32)(nms*1000)); //普通方式延时
}

//延时 nms,不会引起任务调度
//nms:要延时的 ms 数
void Delay_xms(u32 nms)
{
    u32 i;
    for(i=0;i<nms;i++) Delay_us(1000);
}