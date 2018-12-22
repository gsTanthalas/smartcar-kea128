/*
 * state_machine.c
 *
 *  Created on: 2018��12��15��
 *      Author: ��Ц��
 *
 *      ע�⣺���ļ��в���ʵ��StateMachine���漰��С��������Ƶĺ���
 */

#include <cstdint>
#include <cstring>
#include "state_machine.h"
#include "glb_var.h"
#include <cstdio>
using namespace std;

State* IdleState::HandleCommand(uint8_t command)
{
	switch(command)
	{
	case cmd::test_uart:
		return new TestUARTState();
		break;
	case cmd::test_adc:
		return new TestADConverterState();
	default:
		return nullptr;
	}
}

State* TestUARTState::HandleCommand(uint8_t command)
{
	if(command == cmd::stop_force)//��������Ϊǿ��ֹͣ��0��ʱ����������˳����ڲ���״̬�������յ�������ԭ����������λ��
		return new IdleState();
	g_uartc->SendChar(command);
	//ע:gǰ׺��ͳһ������glb_var.h�ڵ�ȫ�ֱ���
	return nullptr;
}
State* TestUARTState::HandleInput()
{

}
void TestUARTState::StateChangeOp()
{
	char msg[] = "entered UART test mode\n";
	g_uartc->SendString(msg,strlen(msg) + 1);
}
State* IdleState::HandleInput()
{

}


StateMachine::StateMachine()
{
	this->current_state = new IdleState();
}
StateMachine::~StateMachine()
{
	delete current_state;
}
void StateMachine::SendCommand(uint8_t command)
{
	State* nxtstate = this->current_state->HandleCommand(command);
	if(nxtstate != nullptr)
	{
		delete this->current_state;
		this->current_state = nxtstate;
		this->current_state->StateChangeOp();
	}
	this->current_state->StateRemainOp();
}



/*
 * member function of ADC Conveter test state
 */

State* TestADConverterState::HandleCommand(uint8_t command)
{
	if(command == cmd::stop_force)//��������Ϊǿ��ֹͣ��0��ʱ����������˳�ADC����״̬�������յ�������ԭ����������λ��
		return new IdleState();
}

void TestADConverterState::StateChangeOp()
{
	char msg[] = "entered ADC CHANNEL 0 test mode\n";
	g_uartc->SendString(msg,strlen(msg) + 1);
}

void TestADConverterState::StateRemainOp()
{
	g_test_adc->StartConversion(8);
	uint32_t counter = 0;
	uint16_t result;
	do
	{
		result = g_test_adc->TryFetchResult();
		++counter;
	}while(result > 0xFFF);
	size_t strl;
	char msg[50];
	strl = snprintf(msg,50,"Convert Finished\n result:%d",result);
	g_uartc->SendString(msg,strl);
	strl = snprintf(msg,50,"\ntime consumption:%d",counter);
	g_uartc->SendString(msg,strl);

	//ע:gǰ׺��ͳһ������glb_var.h�ڵ�ȫ�ֱ���
}


void TestADConverterState::StateExitOp()
{
	char msg[] = "left ADC CHANNEL 0 test mode\n";
	g_uartc->SendString(msg,strlen(msg) + 1);
}









