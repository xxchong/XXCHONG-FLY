#ifndef __DELAY_H
#define __DELAY_H


void Delay_init();

	//��ʱ nus
//nus:Ҫ��ʱ�� us ��.
//nus:0~204522252(���ֵ�� 2^32/fac_us@fac_us=168)
void Delay_us(u32 nus);

//��ʱ nms,�������������
//nms:Ҫ��ʱ�� ms ��
//nms:0~65535
void Delay_ms(u32 nms);

//��ʱ nms,���������������
//nms:Ҫ��ʱ�� ms ��
void Delay_xms(u32 nms);

#endif


