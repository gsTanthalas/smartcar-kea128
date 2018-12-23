/*
 * pid.h
 *
 *  Created on: 2018��12��22��
 *      Author: ��Ц��
 */

#ifndef INCLUDES_UTILITY_PID_H_
#define INCLUDES_UTILITY_PID_H_

#include <cstdint>
#include "sensor.h"



class PIDController
{
private:
/*PID Parameters*/
	uint32_t p;
	uint32_t i;
	uint32_t d;
	uint32_t storage[10];
	uint8_t index;
public:
	PIDController(uint32_t p,uint32_t i,uint32_t d);
	/*
	 * PID�������㷨����
	 * ���������������ⲿʹ�������㷨��д��ɣ����������ú�����
	 * �����:һ��0-10000���������ڿ���PWM����ռ�ձ�
	 */
	uint16_t GetControlOutput(uint32_t error);
	void SetParameters(uint32_t p,uint32_t i,uint32_t d);
};

class AngleController : private PIDController
{
private:
/*historical data begins*/
	uint16_t left_max;
	uint16_t left_min;
	uint16_t mid_max;
	uint16_t mid_min;
	uint16_t right_max;
	uint16_t right_min;
/*historical data ends*/
public:
	void DoControl(InductorData *data);
};

extern AngleController *angle_controller;

#endif /* INCLUDES_UTILITY_PID_H_ */
