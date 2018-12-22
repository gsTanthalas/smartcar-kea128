#include "adc.h"
#include "SKEAZ1284.h"
#include <cstdint>
#include <cassert>
using namespace std;
/*
 * ��Ц�� 2018��12��8r�� ��д
 */
/*
ADC_CHANNEL_AD0 -------------- A0   
ADC_CHANNEL_AD1 -------------- A1
ADC_CHANNEL_AD2 -------------- A6 
ADC_CHANNEL_AD3 -------------- A7
ADC_CHANNEL_AD4 -------------- B0
ADC_CHANNEL_AD5 -------------- B1  
ADC_CHANNEL_AD6 -------------- B2
ADC_CHANNEL_AD7 -------------- B3
ADC_CHANNEL_AD8 -------------- C0
ADC_CHANNEL_AD9 -------------- C1 
ADC_CHANNEL_AD10 ------------- C2
ADC_CHANNEL_AD11 ------------- C3
ADC_CHANNEL_AD12 ------------- F4
ADC_CHANNEL_AD13 ------------- F5 
ADC_CHANNEL_AD14 ------------- F6
ADC_CHANNEL_AD15 ------------- F7

UARTR0:PTA3 TX ,PTA2 RX
UARTR1:PTF3 TX ,PTF2 RX
UARTR2:PTI1 TX ,PTI0 RX
*/
uint32_t ADCModule::chn_occupation_flag = 0;

int8_t ADCModule::StartConversion(uint8_t count)
{//count:����ת��count��ȡƽ��ֵ��ֻ����1-8��Χ�ڵ�ֵ
	if(ADCModule::GetIsConversionOngoing())//����������ڽ��е�ת��û�����
		return -1;//���ش�����-1
//	if(ADC->SC1 & ADC_SC1_COCO_MASK )//����Ѿ�����ɵ�ת��������û�б���ȡ
//		return -2;//���ش�����-2
	this->channel_active = true;//��ǰͨ����Ծ
	this->active_convert_count = count;

	ADC->SC3  = (0
		|ADC_SC3_ADICLK(BUS_CLOCK)   //ѡ��ϵͳʱ��
		|ADC_SC3_MODE(this->bit)      //ADλ��
		//|ADC_SC3_ADLSMP_MASK         //�͹��Ĳɼ�
		|ADC_SC3_ADIV(ADC_ADIV_1)      //��ƵΪ1
		//|ADC_SC3_ADLPC_MASK            //������ʱ��
	) ;
	ADC->SC2  = (0
	// |ADC_SC2_ADTRG_MASK             //1Ӳ������,0�������
	//  |ADC_SC2_ACFE_MASK            //�ɼ��Ƚ�
	                    //  |ADC_SC2_ACFGT_MASK           //���ڱȽ�ֵ���Ƚϴ���
	                    ) ;
	ADC->APCTL1 = ADC_APCTL1_ADPC(1<< this->channel) ;
    ADC->SC4 =(0
    	|ADC_SC4_AFDEP(7) //FIFO���
    );
    //��FIFO����ѭ������count��ADC����FIFO�ʹ˴�������Ӣ�İ��û��ֲ�344ҳ�����İ�326ҳ
    for(int8_t i=1;i<=count;i++)
	{
    	ADC->SC1  = (0
    			|ADC_SC1_ADCH(this->channel)             //ѡ��ɼ�ͨ��
				// |ADC_SC1_ADCO_MASK            //�����ɼ�
			    //|ADC_SC1_AIEN_MASK           //�ж�
    	) ;
	}

    return 0;
}

ADCModule::ADCModule(ADCHn channel/*ADCת����ͨ��*/,ADC_nbit bit/*ADCת����λ����֧��8��10��12λ*/)//���캯��
{
	//�����
	this->channel = channel;
	this->bit = bit;
	//����ʹ�ã��ñ�־
	uint32_t mask = (1<<(int)channel);
	ADCModule::chn_occupation_flag |= mask;

	SIM->SCGC |= SIM_SCGC_ADC_MASK;   //ΪADC�ṩʱ��Դ
}

ADCModule::~ADCModule()//��������
{
	//�Ƴ�ͨ��ռ�ñ�־λ
	uint32_t mask = (1<<(int)channel);
	this->chn_occupation_flag ^= mask; //��λ���Ȼ��ֵ������Ӧ��־λ���㣬���಻��
}

uint16_t ADCModule::TryFetchResult()
{
	//if(!this->channel_active)//ͨ��δ���˵����ǰ��ŵĽ��Ҫô�����ڣ�Ҫô�����ڴ�ͨ��
	//	return 0xFFFF;//����ת�����12λ��������ֲ����ܳ��֡��������롣
	if(!(ADC->SC1 & ADC_SC1_COCO_MASK))//ת��δ���
		return 0xFFFE;
	int16_t sum = 0;

	for(int8_t count = this->active_convert_count; count>=1; count--)
	{
		sum += ADC->R;
	}
	return sum / this->active_convert_count;//����ƽ��ֵ
}





