#ifndef __DELAY_H
#define __DELAY_H


void Delay_init();

	//延时 nus
//nus:要延时的 us 数.
//nus:0~204522252(最大值即 2^32/fac_us@fac_us=168)
void Delay_us(u32 nus);

//延时 nms,会引起任务调度
//nms:要延时的 ms 数
//nms:0~65535
void Delay_ms(u32 nms);

//延时 nms,不会引起任务调度
//nms:要延时的 ms 数
void Delay_xms(u32 nms);

#endif


