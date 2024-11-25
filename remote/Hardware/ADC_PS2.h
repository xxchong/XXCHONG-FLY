#ifndef __ADC_PS2_H
#define __ADC_PS2_H

extern vu16 AD_Value[4];
void ADC_PS2_Init(void);
u16 Get_ADC_Value(u8 ch,u8 times);
void DMA1_Channel1_IRQHandler(void);
void analyze_packet(uint16_t *adcData);
void PackData(void);
vu16 ADC_ValueLimit(vu16 value, vu16 L, vu16 R, vu16 min, vu16 max);
void CAL(void);
void ReceiveDataAnalysis(void);
void ADC_Calibration(void);
#endif
