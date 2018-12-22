/*
 * state_machine.h
 *
 *  Created on: 2018��12��15��
 *      Author: ��Ц��
 * ����״̬��ʵ���ļ�
 */
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "uart.h"
namespace cmd//������Ⱦȫ�������ռ�
{
	enum Event  //���ܵ���״̬��״̬�����仯��һϵ���¼�
	{

		/*..........�����¼�..........*/
		run_start,
		run_entry_curve,
		run_entry_upslope,
		run_entry_downslope,
		run_entry_ring,
		run_leave_special,
		run_finish,
		/*..........�쳣�¼�..........*/
		exp_no_signal,
		exp_speed_abnormal,
		exp_out_of_track,
		exp_blocked,
		/*..........�����¼�..........*/
		noevent //��������û�з����κ��¼�
	};

	enum Command //����λ�����͵�ָ��
	{
		nocommand,
		/*..........ָֹͣ��..........*/
		stop_force, //ǿ��ֹͣ
		stop_ready,
		/*..........����ָ��..........*/
		start,
		/*..........����ָ��..........*/
		test_uart,
		test_adc,
		test_ftm,
		test_gpio,
		/*..........��λָ��..........*/
		reset
	};
}


class State
{

public:
	virtual State* HandleCommand(uint8_t command) {}
	virtual State* HandleInput() {}
	virtual void StateChangeOp()  //������״̬�л�����ǰ״̬ʱ��״̬������
	{
	}
	virtual void StateRemainOp()
	{
	}
	virtual void StateExitOp()
	{

	}

};


class TestADConverterState :public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual State* HandleInput(){}
	virtual void StateChangeOp();  //������״̬�л�����ǰ״̬ʱ��״̬������

	virtual void StateRemainOp();
	virtual void StateExitOp();

};

class TestUARTState :public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual State* HandleInput();
	virtual void StateChangeOp();
};
class IdleState :public State
{
public:
	virtual State* HandleCommand(uint8_t command);
	virtual State* HandleInput();
	virtual void StateChangeOp(){}
};
class StateMachine
{
private:
	State *current_state;
public:
	StateMachine();
	virtual ~StateMachine();
	State* RetrieveState();
	void SendCommand(uint8_t command);
	void SendInput();
	void Reset();
	template <class StateToSet> void ForceSetState();
};

#endif //#ifndef STATE_MACHINE_H
