/*
 * pit.h
 *
 *  Created on: 2018��12��21��
 *      Author: ��Ц��
 */

#ifndef INCLUDES_DRIVER_PIT_H_
#define INCLUDES_DRIVER_PIT_H_

#include <cstdint>
#include "system_SKEAZ1284.h"
#include "SKEAZ1284.h"

namespace PITsettings
{
	enum Channel
	{
		channel_0,
		channel_1
	};
}

class PeriodicInterruptTimer
{
private:
	Channel channel;
public:
	PeriodicInterruptTimer(PITsettings::Channel channel);
	inline void SetPeriod(uint32_t period_in_us)
	{
		PIT->CHANNEL[channel].LDVAL  = period_in_us * (BUS_CLOCK_HZ/1000000);
	}
	inline void EnableIntr()
	{
	    PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK;
	}
	inline void DisableIntr()
	{
	    PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TIE_MASK;
	}
};






#endif /* INCLUDES_DRIVER_PIT_H_ */
