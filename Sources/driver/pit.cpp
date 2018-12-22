/*
 * ��ʱ�ж�ģ������
 * ��Ц�� 12��21�����
 * pit.cpp
 */
#include "pit.h"
#include "core_cm0plus.h"

using namespace std;

PeriodicInterruptTimer::PeriodicInterruptTimer(PITsettings::Channel channel)
{
    SIM->SCGC |= SIM_SCGC_PIT_MASK;     /*!< enable clock to PIT */

    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    PIT->CHANNEL[channel].LDVAL = 0xFFFFFFFF ;//�û�û������֮ǰ��Ĭ��Ϊ����жϼ��
    PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
    PIT->CHANNEL[channel].TFLG  |= PIT_TFLG_TIF_MASK;
    this->channel = channel;
}
