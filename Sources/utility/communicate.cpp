/*
 * communicate.cpp
 *
 *  Created on: 2018年12月15日
 *      Author: 张笑语
 */

#include <cstdint>
#include "communicate.h"
#include "uart.h"
#include "core_cm0plus.h"

using namespace std;

UARTCommunicator *cmtr_uart[3] = {nullptr,nullptr,nullptr};//下面的构造函数里会写入

UARTCommunicator::UARTCommunicator(uint8_t rx_bufferlen,uint8_t tx_bufferlen,UART_settings::UARTn uartn,bool is_port_remap,uint32_t baud_rate)
{
	if(uartn == UART_settings::UARTR0)
		NVIC_DisableIRQ(UART0_IRQn);// 关中断控制器IRQ中断
	else if (uartn == UART_settings::UARTR1)
		NVIC_DisableIRQ(UART1_IRQn);
	else if (uartn == UART_settings::UARTR0)
		NVIC_DisableIRQ(UART2_IRQn);
	//构造函数时序关键，必须屏蔽中断

	//第一步：注册对象给中断处理函数
	cmtr_uart[(uint8_t)uartn] = this;
	//第二步：初始化循环缓冲区
	rxbuffer = new uint8_t[rx_bufferlen];
	rxb_head_ptr = rxb_tail_ptr = rxbuffer ;
	txbuffer = new uint8_t[tx_bufferlen];
	txb_head_ptr = txb_tail_ptr = txbuffer ;
	rxb_tail_ptr += 1;
	txb_tail_ptr += 1;
	rxbufferlen = rx_bufferlen;
	txbufferlen = tx_bufferlen;
	//第三步：初始化UART模块
	this->uart = new UARTModule(uartn,is_port_remap,baud_rate,true,true);

	if(uartn == UART_settings::UARTR0)
		NVIC_EnableIRQ(UART0_IRQn);// 开中断控制器IRQ中断
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

void UARTCommunicator::SendString(char *buffer,uint8_t length/*长度包括终结字符\0*/)
{
	if(!length)
		return;
	for(uint8_t i = 0; i < length; i++)
	{
		this->uart->DisableIntrOnTxRegEmpty();
		++txb_tail_ptr;
		*(txb_tail_ptr - 1) = buffer[i];

		if(txb_tail_ptr - txbuffer >= txbufferlen)//尾指针超出缓冲区末尾
			txb_tail_ptr = txbuffer;
		if(txb_tail_ptr == txb_head_ptr)
			++txb_head_ptr;
		//缓冲区满，覆盖掉最开始的一个字符。
		this->uart->EnableIntrOnTxRegEmpty();
	}
	//开空发送寄存器中断
	this->uart->EnableIntrOnTxRegEmpty();

}
void UARTCommunicator::SendChar(char send)
{
	SendString(&send,1);
}

uint8_t UARTCommunicator::GetChar()
{
	if(rxb_head_ptr == rxb_tail_ptr)//缓冲区空
		return 0;
	uint8_t recieve = *rxb_head_ptr;
	++rxb_head_ptr;
	if(rxb_head_ptr - rxbuffer >= rxbufferlen )//头指针超出缓冲区末尾
		rxb_head_ptr = rxbuffer;
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

/*..............中断处理函数，在发送缓冲区空时发送下一个字符.............*/
void UARTCommunicator::OnIntrSendNext()
{
	if(txb_head_ptr == txb_tail_ptr)//缓冲区空
	{
		this->uart->DisableIntrOnTxRegEmpty();
		return;
	}
	this->uart->SendChar(*txb_head_ptr);
	++txb_head_ptr;
	if(txb_head_ptr - txbuffer >= txbufferlen )//指针超出缓冲区末尾
		txb_head_ptr = txbuffer;
	this->uart->EnableIntrOnTxRegEmpty();
}
/*..............中断处理函数，在接收缓冲区满时读取下一个字符.............*/
void UARTCommunicator::OnIntrRecieveNext()
{
	++rxb_tail_ptr;
	*(rxb_tail_ptr - 1) = this->uart->RecieveChar();
	if(rxb_tail_ptr - rxbuffer >= rxbufferlen  )//尾指针超出缓冲区末尾
		rxb_tail_ptr = rxbuffer;
	if(rxb_tail_ptr == rxb_head_ptr)//缓冲区满
		++rxb_head_ptr;
}



