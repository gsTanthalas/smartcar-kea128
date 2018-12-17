/*
 * gpio.c
 *
 *  Created on: 2018��12��15��
 *      Author: ��Ц��
 */

#include <cstdint>
#include "gpio.h"

using namespace std;
volatile GPIO_Type *GPIOx[3]={GPIOA,GPIOB,GPIOC};

GPIOModule::GPIOModule(PTxn port,bool direction_is_output)
{
	if (direction_is_output)   //���
	{
	    GPIO_PDDR_REG(GPIOX_BASE(port)) |= (1 << PTn(static_cast<uint8_t>(port)));  //���ö˿ڷ���Ϊ���
	    this->SetState(0);   //����gpio_set�������趨�������״̬
	}
	else
	{
	    GPIO_PDDR_REG(GPIOX_BASE(port)) &= ~(1 << PTn(port));  //���ö˿ڷ���Ϊ����
	    //GPIO_PIDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));  //���ö˿ڷ���Ϊ����
	    //����Ŀ���ΪʲôҪ�������������أ���ע�͵�������û�����⡣
	}
	this->port = port;
}

GPIOModule::~GPIOModule()
{
	//���ڵ����������ƺ�ûʲô��������
}

