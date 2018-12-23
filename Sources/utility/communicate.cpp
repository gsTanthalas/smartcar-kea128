/*
 * communicate.cpp
 *
 *  Created on: 2018��12��15��
 *      Author: ��Ц��
 */

#include <cstdint>
#include "communicate.h"
#include "uart.h"
#include "core_cm0plus.h"

using namespace std;

UARTCommunicator *cmtr_uart[3] = {nullptr,nullptr,nullptr};//����Ĺ��캯�����д��

UARTCommunicator::UARTCommunicator(uint8_t rx_bufferlen,uint8_t tx_bufferlen,UART_settings::UARTn uartn,bool is_port_remap,uint16_t baud_rate)
{
	if(uartn == UART_settings::UARTR0)
		NVIC_DisableIRQ(UART0_IRQn);// ���жϿ�����IRQ�ж�
	else if (uartn == UART_settings::UARTR1)
		NVIC_DisableIRQ(UART1_IRQn);
	else if (uartn == UART_settings::UARTR0)
		NVIC_DisableIRQ(UART2_IRQn);
	//���캯��ʱ��ؼ������������ж�

	//��һ����ע�������жϴ�������
	cmtr_uart[(uint8_t)uartn] = this;
	//�ڶ�������ʼ��ѭ��������
	rxbuffer = new uint8_t[rx_bufferlen];
	rxb_head_ptr = rxb_tail_ptr = rxbuffer ;
	txbuffer = new uint8_t[tx_bufferlen];
	txb_head_ptr = txb_tail_ptr = txbuffer ;
	rxb_tail_ptr += 1;
	txb_tail_ptr += 1;
	rxbufferlen = rx_bufferlen;
	txbufferlen = tx_bufferlen;
	//����������ʼ��UARTģ��
	this->uart = new UARTModule(uartn,is_port_remap,baud_rate,true,true);

	if(uartn == UART_settings::UARTR0)
		NVIC_EnableIRQ(UART0_IRQn);// ���жϿ�����IRQ�ж�
	else if (uartn == UART_settings::UARTR1)
		NVIC_EnableIRQ(UART1_IRQn);
	else if (uartn == UART_settings::UARTR0)
		NVIC_EnableIRQ(UART2_IRQn);
}

UARTCommunicator::~UARTCommunicator()
{
	delete[] rxbuffer;
	delete[] txbuffer;
	rxb_head_ptr = rxb_tail_ptr = rxbuffer = nullptr;
	txb_head_ptr = txb_tail_ptr = txbuffer = nullptr;
	cmtr_uart[static_cast<uint8_t>(this->uart->GetChannel())] = nullptr;
	delete uart;
}

void UARTCommunicator::SendString(char *buffer,uint8_t length/*���Ȱ����ս��ַ�\0*/)
{
	if(!length)
		return;
	DisableNVICIntr();
	for(uint8_t i = 0; i < length; i++)
	{
		*(txb_tail_ptr - 1) = buffer[i];
		++txb_tail_ptr;
		if(txb_tail_ptr - txbuffer >= (txbufferlen + 1))//βָ�볬��������ĩβ
			txb_tail_ptr = txbuffer;
		if(txb_tail_ptr == txb_head_ptr)
			++txb_head_ptr;
		//�������������ǵ��ʼ��һ���ַ���
	}
	EnableNVICIntr();
	//���շ��ͼĴ����ж�
	this->uart->EnableIntrOnTxRegEmpty();

}
void UARTCommunicator::SendChar(char send)
{
	SendString(&send,1);
}

uint8_t UARTCommunicator::GetChar()
{
	DisableNVICIntr();
	if((rxb_head_ptr + 1 == rxb_tail_ptr) || ((rxb_head_ptr - rxb_tail_ptr) == (rxbufferlen - 1)))//��������
		return 0;
	uint8_t recieve = *rxb_head_ptr;
	++rxb_head_ptr;
	if(rxb_head_ptr - rxbuffer >= rxbufferlen )//ͷָ�볬��������ĩβ
		rxb_head_ptr = rxbuffer;
	EnableNVICIntr();
	return recieve;
}

uint8_t UARTCommunicator::PeekChar()
{
	return *rxb_head_ptr;
}

void UARTCommunicator::CleanRxBuffer()
{
	rxb_head_ptr = rxb_tail_ptr = rxbuffer ;
}

/*..............�жϴ����������ڷ��ͻ�������ʱ������һ���ַ�.............*/
void UARTCommunicator::OnIntrSendNext()
{
	DisableNVICIntr();
	if((txb_head_ptr + 1 == txb_tail_ptr) || ((txb_head_ptr - txb_tail_ptr) == (txbufferlen - 1)))//��������
	{
		this->uart->DisableIntrOnTxRegEmpty();
		return;
	}
	this->uart->SendChar(*txb_head_ptr);
	++txb_head_ptr;
	if(txb_head_ptr - txbuffer >= txbufferlen)//ָ�볬��������ĩβ
		txb_head_ptr = txbuffer;
	EnableNVICIntr();
	this->uart->EnableIntrOnTxRegEmpty();
}
/*..............�жϴ����������ڽ��ջ�������ʱ��ȡ��һ���ַ�.............*/
void UARTCommunicator::OnIntrRecieveNext()
{
	*(rxb_tail_ptr - 1) = this->uart->RecieveChar();
	++rxb_tail_ptr;
	if(rxb_tail_ptr - rxbuffer >= (rxbufferlen + 1) )//βָ�볬��������ĩβ
		rxb_tail_ptr = rxbuffer;
	if(rxb_tail_ptr == rxb_head_ptr)//��������
		++rxb_head_ptr;
}


