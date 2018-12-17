
#ifndef _UART_H___
#define _UART_H___

#include <cstdint>
#include "SKEAZ1284.h"
extern volatile UART_Type *UARTx[];
namespace UART_settings
{
	enum  UARTn
	{
		//��ʼ��Ĭ������       --TXD--      --RXD--     ���Ը�������ͨ�����������޸� uart_init
		UARTR0 = 0,    //
		UARTR1 = 1,    //
		UARTR2 = 2,    //
	};

}

class UARTModule
{
private:
	UART_settings::UARTn channel;
	static uint8_t chn_occupation_flag;
	bool is_pin_remap = false;
public:
	inline void DisableIntrOnRecieve()
	{
		UARTx[channel]->C2 &= ~UART_C2_RIE_MASK;       // ��ֹUART�����ж�
	}
	inline void EnableIntrOnRecieve()
	{
		UARTx[channel]->C2 |= UART_C2_RIE_MASK;       // ����UART�����ж�

	}
	inline void EnableIntrOnTxRegEmpty()
	{
		UARTx[channel]->C2 |= UART_C2_TIE_MASK;
	}
	inline void DisableIntrOnTxRegEmpty()
	{
		UARTx[channel]->C2 &= ~UART_C2_TIE_MASK;

	}
	UARTModule(UART_settings::UARTn uartn, bool is_port_remap, uint16_t baud_rate,bool enable_recieve_intr = false,bool enable_txreg_empty_intr=false);
	~UARTModule();
	void SendChar(uint8_t send);
	uint8_t RecieveChar();
	inline UART_settings::UARTn GetChannel()
	{
		return channel;
	}
};

#endif
