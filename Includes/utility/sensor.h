


#include <cstdint>
#include "adc.h"
#include "pit.h"




struct InductorData
{
	volatile bool flag_data_updated;
	volatile uint16_t values[3];
};




class InductorSensor
{
private:
	//num_of_adc = 3

public://�����жϺ�����Ե�ʣ����ò������к���ȫ����¶Ϊ����
	   //�Ժ����õ���ģʽ�ķ���ʵ�ְ�װ
	ADCModule *adcm;                      //���飬������adת����
	volatile InductorData *inductor_data;
	volatile uint8_t current_adc_m;       //���ڽ���ת����adcģ��
	volatile bool sys_has_ongoing_convert = false;
	InductorSensor();
	~InductorSensor();
	void StartConvert();
};



#ifdef __cplusplus
extern "C"
{
#endif

volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler();
volatile void __attribute__((interrupt ("IRQ"))) ADC_IRQHandler();

#ifdef __cplusplus
}


extern InductorSensor *g_sensor;



#endif
