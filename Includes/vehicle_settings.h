/*
 * vehicle_settings.h
 *
 *  Created on: 2018��12��18��
 *      Author: ��Ц��
 *      ���ܳ�������������á�����Ĭ���ٶȵȵ�
 */

#ifndef INCLUDES_VEHICLE_SETTINGS_H_
#define INCLUDES_VEHICLE_SETTINGS_H_

#include <cstdint>

struct PIDParams
{
	uint16_t p;
	uint16_t i;
	uint16_t d;
};

static class VehicleSettings
{
public:
	static uint16_t default_speed;
	static uint16_t steer_mid_freq;//�����λ��Ӧ��PWM�����ź�Ƶ��
	static uint16_t steer_max_turn_freq_change; //�������λ��һ���ߵ���ͷ����Ҫ��Ƶ�ʱ仯��
	static PIDParams steer_pid_params; //���PID����ϵ��
	static PIDParams motor_pid_params; //���PID����ϵ��

	static inline void GetVehicleSettings()
	{

	}
};




#endif /* INCLUDES_VEHICLE_SETTINGS_H_ */
