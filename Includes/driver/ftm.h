/*
 * ftm.h
 *
 *  Created on: 2018��12��18��
 *      Author: ��Ц��
 *      ֻʵ����FTM��PWM�������
 */

#ifndef INCLUDES_DRIVER_FTM_H_
#define INCLUDES_DRIVER_FTM_H_


#include <cstdint>
#include "system_SKEAZ1284.h"
#include "SKEAZ1284.h"


namespace FTMSettings
{
	enum Modules
	{
		mFTM0,
		mFTM1,
		mFTM2
	};

	enum Channels
	{
		FTM_CH0,
		FTM_CH1,
		FTM_CH2,
		FTM_CH3,
		FTM_CH4,
		FTM_CH5,
	};
	enum PortRemapType
	{
		remap_none,
		remap_first,
		remap_second
	};

}


/*
ͨ����                              Ĭ������                      ��ӳ������[first,second]
FTM0_CH0      PTA0         PTB2
FTM0_CH1      PTA1         PTB3
FTM1_CH0      PTC4         PTH2
FTM1_CH1      PTC5         PTE7
FTM2_CH0      PTC0         PTH0 , PTF0
FTM2_CH1      PTC1         PTH1 , PTF1
FTM2_CH2      PTC2         PTD0 , PTG4
FTM2_CH3      PTC3         PTD1 , PTG5
FTM2_CH4      PTB4         PTG6
FTM2_CH5      PTB5         PTG7

��ע�ⲻͬģ����õ�ͨ����Ŀ��ͬ
*/

extern FTM_Type * FTMx[];

class FlexTimerModule
{
private:
	FTMSettings::Modules module;
	FTMSettings::Channels channel;
/*additional channels start*/
	//һ��FTMͨ���ĸ���ģ��������ͬƵ�ʲ�ͬռ�ձȵ�PWM�źţ������һЩ�����channel�Թ��������ø���channel
	//�˹�����δʵ��
	FTMSettings::Channels *module_all_channels;
/*private functions used in constructor begin*/
	void PinSet(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type);


/*private functions end*/
public:
	FlexTimerModule(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type);
	void SetPWMParam(uint32_t frequency,uint32_t duty_cycle/*ռ�ձȣ���������10000Ϊռ�ձ�(С��)*/);
	inline void EnablePWMOutput()
	{
		FTMx[this->module]->CNT = 0;
		//���������д������ֵ������ΪCNTIN�и����ĳ�ʼֵ���޸�0Ϊ���������ܸı��������ֵ��
		FTMx[this->module]->SC = FTM_SC_CLKS(1)|FTM_SC_PS(2) ;
		//FTM_SC_CLKS����ģ���ṩϵͳʱ�ӣ�4��Ƶ  10MHz
	}
	inline void DisablePWMOutput()
	{
		FTMx[this->module]->SC = FTM_SC_CLKS(0);
		//����������ṩ�κ�ʱ�ӣ���ֹ����
	}
	~FlexTimerModule();

};


#endif /* INCLUDES_DRIVER_FTM_H_ */
