/*
 * ftm.cpp
 *
 *  Created on: 2018��12��22��
 *      Author: ��Ц��
 */

#include "ftm.h"
#include <cstdint>
#include "system_SKEAZ1284.h"

using namespace std;
using namespace FTMSettings;

//���飬���ڿ��ƼĴ���Ѱַ


FlexTimerModule::FlexTimerModule(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type)
{
	  //������ģ������ʱ���ź�
	  switch(module)
	  {
	  case mFTM0:
		  SIM->SCGC |= SIM_SCGC_FTM0_MASK;
		  break;
	  case mFTM1:
		  SIM->SCGC |= SIM_SCGC_FTM1_MASK;
		  break;
	  case mFTM2:
		  SIM->SCGC  |= SIM_SCGC_FTM2_MASK;
		  break;
	  }

	  this->PinSet(module,channel,port_remap_type);
	  //���ú������ö�Ӧ��ͨ��������
	  FTMx[module]->SC = 0 ;
	  FTMx[module]->CONTROLS[channel].CnSC = 0 ;
	  //�������ģ���ͨ���ļĴ���������������PWM�������
	  //��������ͨ���Ŀ��ƼĴ���CnSc����������һ�����ض��룬������PWM������û��ֲ�379ҳ��26-69�����¸�λ����ֵ��
	  FTMx[module]->CONTROLS[channel].CnSC |= (0
	                                  //   |FTM_CnSC_ELSA_MASK
	                                       |FTM_CnSC_ELSB_MASK  //�ߵ�ƽ��Ч���壬�������ﵽmatch(CnV)֮������͵�ƽ
	                                  //   |FTM_CnSC_MSA_MASK
	                                       |FTM_CnSC_MSB_MASK   //��Ե����PWM
	                                  //   |FTM_CnSC_CHIE_MASK  //�����ж�
	                                  //   |FTM_CnSC_CHF_MASK   //�����ñ�־λ
	                                       );
	  FTMx[module]->SC &= ~FTM_SC_CPWMS_MASK;
	  //�������ù涨���������ϼ����������ǼӼ����ؼ�����

	  //��ģ���ͨ����Ϣ��������Ա����
	  this->channel = channel;
	  this->module = module;

	  //���ǻ�û������PWMģ��ļ���ʱ��Դ��������ڻ����Ὺʼ���PWM����
}

void FlexTimerModule::SetPWMParam(uint32_t frequency,uint32_t duty_cycle)
{
	uint32_t sys_clk = SystemCoreClock;
	//SystemCoreClock�Ƕ�����system_SKEAZ1284.h�е�ȫ�ֱ���
	bool freq_is_supported = ( (sys_clk >> (16+4) ) > frequency) && ((sys_clk >> 4) < frequency);
	//FTM�������Ŀ��Ϊ16λ������ʹ�õļ���ʱ���ź�Ϊϵͳʱ�ӵ�16��Ƶ����2.5MHz����EnablePWMOutput���������ã�
	//16��Ӧ16λ�Ĵ�����4��Ӧ16��Ƶ����2���Ĵη���ϵͳʱ�ӵ�20��Ƶ����PWMģ��������������Ƶ��,4��Ƶ����ϵͳ���������Ƶ��
	//��������ʼ����ʱ��Ϊ�ߵ�ƽ
	//����ֵ�ﵽmodʱ����ƽת�������������ڹ��캯���е����ã���ʱ�ɸߵ�ƽת��Ϊ�͵�ƽ��
	//������������ֵʱ�����û��㣬��ƽת����
	if(!freq_is_supported)
	{
		return;
		//TODO:ʵ�ִ�����ʾ
	}
	//��ʵ�ϣ���ʹ����֧�֣�̫�͵�Ƶ��Ҳû���κ����塣��������Ŀ��ֻ��Ϊ������ļ��㲻Ҫ��������ͷǷ����ݡ�
	uint16_t counter_max_value = (uint16_t)(sys_clk >> 4) / frequency ;
	uint16_t chn_match_value = (uint16_t)(duty_cycle * (counter_max_value - 0 + 1)) / 10000;
	//��������������ֵ��  ���ֵ-��ֵ+1������0�ǳ�ֵ�����Ըĵģ�
	//�Ѿ�������ռ�ձ���duty_cycle����1����˿��Լ������ƽ��תʱ��������ֵ
	FTMx[this->module]->MOD = counter_max_value ;
	FTMx[this->module]->CONTROLS[this->channel].CnV = chn_match_value  ;
	//ע�⵽ռ�ձ�����ÿ��ͨ���ļĴ��������ģ����һ��FTMģ��ĸ���ͨ�����������ͬ��ռ�ձ�
	//������δ�������ʱ���źţ���˴�ʱ��������Ȼ���Ὺʼ����
}

void FlexTimerModule::PinSet(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type)
{
	  switch(module)
	  {
	  case mFTM0:
	    if (channel==FTM_CH0)                        //FTMģ��1
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL &=~SIM_PINSEL_FTM0PS0_MASK ;  // PWM0 PTA0
	      else
	        SIM->PINSEL |= SIM_PINSEL_FTM0PS0_MASK ;  // PWM0 PTB2
	    }
	    else
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL &=~SIM_PINSEL_FTM0PS1_MASK ;  // PWM1 PTA1
	      else
	        SIM->PINSEL |= SIM_PINSEL_FTM0PS1_MASK ;  // PWM1 PTB3
	    }

	    break ;
	  case mFTM1:
	    if (channel==FTM_CH0)
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL &= ~SIM_PINSEL_FTM1PS0_MASK ;  // PWM0 PTC4 ��JTAG��
	      else
	        SIM->PINSEL |= SIM_PINSEL_FTM1PS0_MASK ;  // PWM0 PTH2
	    }
	    else
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL &= ~SIM_PINSEL_FTM1PS1_MASK ;  // PWM1 PTC5
	      else
	        SIM->PINSEL |= SIM_PINSEL_FTM1PS1_MASK ;  // PWM1 PTE7
	    }
	    break ;
	  case mFTM2:
	    if (channel==FTM_CH0)
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS0(0); //PWM0 PTC0
	      else if (port_remap_type == FTMSettings::remap_first)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS0(1); //PWM0 PTH0
	      else if (port_remap_type == FTMSettings::remap_second)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS0(2); //PWM0 PTF0
	    }
	    else if (channel==FTM_CH1)
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS1(0); //PWM1 PTC1
	      else if (port_remap_type == FTMSettings::remap_first)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS1(1); //PWM1 PTH1
	      else if (port_remap_type == FTMSettings::remap_second)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS1(2); //PWM1 PTF1
	    }

	    else if (channel==FTM_CH2)
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS2(0); //PWM2 PTC2
	      else if (port_remap_type == FTMSettings::remap_first)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS2(1); //PWM2 PTD0
	      else if (port_remap_type == FTMSettings::remap_second)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS2(2); //PWM2 PTG4
	    }

	    else if (channel==FTM_CH3)
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS3(0); //PWM3 PTC3
	      else if (port_remap_type == FTMSettings::remap_first)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS3(1); //PWM3 PTD1
	      else if (port_remap_type == FTMSettings::remap_second)
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS3(2); //PWM3 PTG5
	    }

	    else if (channel==FTM_CH4)
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL1 &= ~SIM_PINSEL1_FTM2PS4_MASK; //PWM4 PTB4
	      else
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS4_MASK; //PWM4 PTG6
	    }
	    else if (channel==FTM_CH5)
	    {
	      if (port_remap_type == FTMSettings::remap_none)
	        SIM->PINSEL1 &= ~SIM_PINSEL1_FTM2PS5_MASK; //PWM5 PTB5
	      else
	        SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS5_MASK; //PWM5 PTG7
	    }
	    break ;
	  default:
	    break;
	  }
}


