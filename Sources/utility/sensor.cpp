/*
 * sensor.cpp
 *
 *  Created on: 2018��12��21��
 *      Author: ��Ц��
 */

#include "sensor.h"
#include "adc.h"

using namespace std;

InductorSensor *g_sensor;


InductorSensor::InductorSensor()
{
	adcm = new ADCModule[3]{ADCModule(ADC_CHANNEL_AD0,ADC_12BIT),ADCModule(ADC_CHANNEL_AD1,ADC_12BIT),ADCModule(ADC_CHANNEL_AD0,ADC_12BIT)};
	for(int i=0;i<3;i++)
	{
		adcm[i].EnableIntrOnCovertFinished();//��ת������ж�
	}
	inductor_data = new InductorData();

}

InductorSensor::~InductorSensor()
{
	delete[] adcm;
	delete inductor_data;
}

void InductorSensor::StartConvert()
{
	if(sys_has_ongoing_convert)
		return;
	adcm[0].StartConversion(7);
	//��ʼ��һ��ת����֮��ľͽ����жϴ�������
	current_adc_m = 0;
	sys_has_ongoing_convert = true;
}

volatile void __attribute__((interrupt ("IRQ"))) PIT_CH0_IRQHandler()
{//��ʱ�жϣ���ʼADת�������ݸ���
	g_sensor->StartConvert();
}

volatile void __attribute__((interrupt ("IRQ"))) ADC_IRQHandler()
{
	uint8_t finished_adc_id =  g_sensor->current_adc_m;
	g_sensor->inductor_data->values[finished_adc_id] =  g_sensor->adcm[finished_adc_id].TryFetchResult();
	if(finished_adc_id <= 2)
	{
		g_sensor->adcm[++finished_adc_id].StartConversion(7);
	}
	else//=3
	{
		g_sensor->sys_has_ongoing_convert = false;
		g_sensor->inductor_data->flag_data_updated = true; //ָʾ�����Ѿ��������
	}

}
