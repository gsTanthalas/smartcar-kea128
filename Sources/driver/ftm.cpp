/*
 * ftm.cpp
 *
 *  Created on: 2018年12月22日
 *      Author: 张笑语
 */

#include "ftm.h"
#include <cstdint>
#include "system_SKEAZ1284.h"

using namespace std;
using namespace FTMSettings;

//数组，用于控制寄存器寻址
FTM_Type * FTMx[3] = { FTM0,FTM1,FTM2};

FlexTimerModule::FlexTimerModule(FTMSettings::Modules module, FTMSettings::ExternalClockPin clock_pin)
{
	  switch(module)
	  {
	  case mFTM0:
		  SIM->SCGC |= SIM_SCGC_FTM0_MASK;              //开启FTM外设时钟
		  SIM->PINSEL &= ~SIM_PINSEL_FTM0CLKPS_MASK;    //清除外部时钟引脚选择
		  if(clock_pin == FTMSettings::clk_0_A5)       //开启内部上拉，避免时钟输入引脚上出现高阻态
			  PORT->PUE0 |= (uint32_t)(1<<5);
		  else if(clock_pin == FTMSettings::clk_1_E0)
			  PORT->PUE1 |= (uint32_t)(1<<0);
		  else if(clock_pin == FTMSettings::clk_2_E7)
			  PORT->PUE1 |= (uint32_t)(1<<7);
		  SIM->PINSEL |= SIM_PINSEL_FTM0CLKPS(clock_pin);       //选择外部时钟输入引脚
		  break;
	  case mFTM1:
		  SIM->SCGC |= SIM_SCGC_FTM1_MASK;
		  SIM->PINSEL &= ~SIM_PINSEL_FTM1CLKPS_MASK;    //清除外部时钟引脚选择
		  if(clock_pin == FTMSettings::clk_0_A5)       //开启内部上拉，避免时钟输入引脚上出现高阻态
			  PORT->PUE0 |= (uint32_t)(1<<5);
		  else if(clock_pin == FTMSettings::clk_1_E0)
			  PORT->PUE1 |= (uint32_t)(1<<0);
		  else if(clock_pin == FTMSettings::clk_2_E7)
			  PORT->PUE1 |= (uint32_t)(1<<7);
		  SIM->PINSEL |= SIM_PINSEL_FTM1CLKPS(clock_pin);       //选择外部时钟输入引脚
		  break;
	  case mFTM2:
		  SIM->SCGC  |= SIM_SCGC_FTM2_MASK;
		  SIM->PINSEL &= ~SIM_PINSEL_FTM2CLKPS_MASK;    //清除外部时钟引脚选择
		  if(clock_pin == FTMSettings::clk_0_A5)       //开启内部上拉，避免时钟输入引脚上出现高阻态
			  PORT->PUE0 |= (uint32_t)(1<<5);
		  else if(clock_pin == FTMSettings::clk_1_E0)
			  PORT->PUE1 |= (uint32_t)(1<<0);
		  else if(clock_pin == FTMSettings::clk_2_E7)
			  PORT->PUE1 |= (uint32_t)(1<<7);
		  SIM->PINSEL |= SIM_PINSEL_FTM2CLKPS(clock_pin);       //选择外部时钟输入引脚
		  break;
	  }
	  FTMx[module]->SC = 0;
	  this->module = module;
}

FlexTimerModule::FlexTimerModule(FTMSettings::Modules module)
{
	  //首先向模块引入时钟信号
	  switch(module)
	  {
	  case mFTM0:
		  //
		  SIM->SCGC |= SIM_SCGC_FTM0_MASK;
		  break;
	  case mFTM1:
		  SIM->SCGC |= SIM_SCGC_FTM1_MASK;
		  break;
	  case mFTM2:
		  SIM->SCGC  |= SIM_SCGC_FTM2_MASK;
		  break;
	  }
	  this->module = module;
	  FTMx[module]->SC = 0;

}
void FlexTimerModule::SetFrequency(uint16_t freq)
{
	uint32_t sys_clk = SystemCoreClock;
	//SystemCoreClock是定义在system_SKEAZ1284.h中的全局变量
	bool freq_is_supported = ( (sys_clk >> (16+2) ) < frequency) && ((sys_clk >> 2) > frequency);
	//FTM计数器的宽度为16位；我们使用的计数时钟信号为系统时钟的4分频，即10MHz（在EnableBusClock函数中设置）
	//16对应16位寄存器，2对应4分频，即2的2次方。系统时钟的20分频便是PWM模块允许输出的最低频率,4分频便是系统的最大允许频率
	//计数器开始计数时，为一种电平
	//计数值达到mod时，电平转换。根据我们在构造函数中的设置，此时由低电平转换为高电平。
	//计数器到达满值时，重置回零，电平转换。
	if(!freq_is_supported)
	{
		return;
		//TODO:实现错误提示
	}
	//事实上，即使可以支持，太低的频率也没有任何意义。这样做的目的只是为了下面的计算不要出现溢出和非法数据。
	uint16_t counter_max_value = (uint16_t)((sys_clk >> 2) / frequency) ;
	//写入计数器最大值前先将计数器的值清零
	FTMx[this->module]->CNT = 0;
	//向计数器内写入任意值，重置为CNTIN中给定的初始值。修改0为其他数不能改变计数器初值！
	FTMx[this->module]->MOD = counter_max_value ;
	//我们仍未引入计数时钟信号，因此此时计数器仍然不会开始计数
	//注意到占空比是由每个通道的寄存器决定的，因此一个FTM模块的各个通道可以输出不同的占空比。
	//详见FlexTimerChannel类
	this->frequency = frequency;
}

void FlexTimerChannel::SetDutyCycle(uint16_t duty_cyc)
{
	uint16_t counter_max_value = FTMx[this->module->GetModuleNo()]->MOD;
	uint16_t chn_match_value = (uint16_t)(((uint32_t)(10000 - duty_cyc) * (counter_max_value - 0 + 1)) / 10000);
	//计数器计数的总值是  最大值-初值+1，其中0是初值（可以改的）
	//已经定义了占空比是duty_cycle除以1万，因此可以计算出电平翻转时计数器的值
	FTMx[this->module->GetModuleNo()]->MOD = counter_max_value ;
	FTMx[this->module->GetModuleNo()]->CONTROLS[this->channel].CnV = chn_match_value  ;
}

FlexTimerChannel::FlexTimerChannel(FlexTimerModule *module,FTMSettings::Channels channel_no,FTMSettings::PortRemapType port_remap_type)
{
	FTMSettings::Modules modno = module->GetModuleNo();
	  this->PinSet(modno,channel_no,port_remap_type);
	  //调用函数设置对应的通道和引脚
	  FTMx[modno]->CONTROLS[channel_no].CnSC = 0 ;
	  //清空模块和通道的寄存器，即重置整个通道功能
	  //将模块和通道信息保存至成员变量
	  this->channel = channel_no;
	  this->module = module;
}


void FlexTimerChannel::PinSet(FTMSettings::Modules module,FTMSettings::Channels channel,FTMSettings::PortRemapType port_remap_type)
{
	  switch(module)
	  {
	  case mFTM0:
	    if (channel==FTM_CH0)                        //FTM模块1
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
	        SIM->PINSEL &= ~SIM_PINSEL_FTM1PS0_MASK ;  // PWM0 PTC4 （JTAG）
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


