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
using namespace std;

State* IdleState::HandleCommand(uint8_t command)
{
	switch(command)
	{
	case cmd::test_uart:
		return new TestUARTState();
		break;

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
	g_uartc->SendString((uint8_t*)(&(msg[0])),strlen(msg) + 1);
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


