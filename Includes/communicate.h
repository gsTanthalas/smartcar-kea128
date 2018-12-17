/*
 * communicate.h
 *
 *  Created on: 2018��12��15��
 *      Author: ��Ц��
 *
 *      ��Ƭ���������豸����Ҫ��PC��������ͨѶ�ķ���
 */

#ifndef INCLUDES_COMMUNICATE_H_
#define INCLUDES_COMMUNICATE_H_

#include <cstdint>
#include <cstring>
#include "SKEAZ1284.h"
#include "uart.h"


#define VALID_UART_CHANNEL_WITH_BUFFER_1



class UARTCommunicator
{//TODO:ʵ��printf
private://ѭ��������

	uint8_t rxbufferlen;
	volatile uint8_t *rxbuffer;
	volatile uint8_t *rxb_head_ptr;//head�ǻ������ڵ�һ���ַ��ĵ�ַ
	volatile uint8_t *rxb_tail_ptr;//tail��ѭ�������������һ���ַ��ĵ�ַ+1
	volatile uint8_t txbufferlen;
	volatile uint8_t *txbuffer;
	volatile uint8_t *txb_head_ptr;
	volatile uint8_t *txb_tail_ptr;

protected:
	UARTModule *uart;
public:
	UARTCommunicator(uint8_t rx_bufferlen,uint8_t tx_bufferlen,UART_settings::UARTn uartn,bool is_port_remap,uint16_t baud_rate);
	~UARTCommunicator();
	void SendString(uint8_t *buffer,uint8_t length/*���Ȱ����ս��ַ�\0*/);
	void SendChar(uint8_t send);
	//uint8_t* GetString();
	//TODO: ʵ��GetString()
	uint8_t GetChar();
	uint8_t PeekChar();
    void CleanRxBuffer();


	/*..............���жϴ��������ã��ڷ��ͻ�������ʱ������һ���ַ�.............*/
    void OnIntrSendNext();
	/*..............���жϴ��������ã��ڽ��ջ�������ʱ��ȡ��һ���ַ�.............*/
    void OnIntrRecieveNext();
    /*�жϴ�������intr.h*/


};

/*..�����������Ѿ���û�г�ʼ��������ͨ����Ӧ��thisָ�롣�ṩ���жϴ�����ʹ��..*/
extern UARTCommunicator *cmtr_uart[];



#endif /* INCLUDES_COMMUNICATE_H_ */
