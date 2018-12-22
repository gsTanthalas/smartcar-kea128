#include <cstdint>
#include "communicate.h"
#include "gpio.h"
#include "state_machine.h"
#include "cppmain.h"
#include "glb_var.h"
#include "UART.h"
#include "core_cm0plus.h"
using namespace std;


int _cppmain()
{

	bool vehicle_has_piority_command = false;
	//������ģ���ʼ������
	//���ڲ���ģ���ʼ������ʱ��
	g_test_adc = new ADCModule(ADC_CHANNEL_AD0,ADC_8BIT);
	//���ڳ�ʼ��
	g_uartc = new UARTCommunicator(16, 32, UART_settings::UARTR0, false, 19200);
	//״̬����ʼ��
	StateMachine *fsm = new StateMachine();
	char msg[] = "startup\n";
	g_uartc->SendString((uint8_t*)(&(msg[0])),9);
	while(1)//������ѭ��
	{
		/*............���ͨѶ���Ƿ�����Ӧ�����............*/
		uint8_t command = g_uartc->GetChar();
		/*...........����жϴ�������Ƿ��Ѿ���������..........*/


		/*.............״̬�����²�ִ����Ӧ����.............*/
		if(command!=0)
			fsm->SendCommand(command);

	}

	while(1);
	return 0;//��������
}



volatile void __attribute__((interrupt ("IRQ"))) UART0_IRQHandler()
{
	NVIC_DisableIRQ(UART0_IRQn);
	if(cmtr_uart[0] ==nullptr)
	{
		uint32_t some = UARTx[0]->S1 ;
		NVIC_EnableIRQ(UART0_IRQn);
		return;
	}

	if(UARTx[0]->S1 & UART_S1_TDRE_MASK);//������ͼĴ���Ϊ��
		cmtr_uart[0]->OnIntrSendNext();
	if(UARTx[0]->S1 & UART_S1_RDRF_MASK)//������ܼĴ���Ϊ��
		cmtr_uart[0]->OnIntrRecieveNext();
	NVIC_EnableIRQ(UART0_IRQn);
}

volatile void __attribute__((interrupt ("IRQ"))) UART1_IRQHandler()
{
	NVIC_DisableIRQ(UART1_IRQn);
	if(cmtr_uart[1] ==nullptr)
	{
		uint32_t some = UARTx[1]->S1 ;
		NVIC_EnableIRQ(UART0_IRQn);
		return;
	}


	if(UARTx[1]->S1 & UART_S1_TDRE_MASK);//������ͼĴ���Ϊ��
		cmtr_uart[1]->OnIntrSendNext();
	if(UARTx[1]->S1 & UART_S1_RDRF_MASK)//������ܼĴ���Ϊ��
		cmtr_uart[1]->OnIntrRecieveNext();
	NVIC_EnableIRQ(UART1_IRQn);
}

volatile void __attribute__((interrupt ("IRQ"))) UART2_IRQHandler()
{
	NVIC_DisableIRQ(UART2_IRQn);
	if(cmtr_uart[2] ==nullptr)
	{
		uint32_t some = UARTx[2]->S1 ;
		NVIC_EnableIRQ(UART0_IRQn);
		return;
	}


	if(UARTx[2]->S1 & UART_S1_TDRE_MASK);//������ͼĴ���Ϊ��
		cmtr_uart[2]->OnIntrSendNext();
	if(UARTx[1]->S1 & UART_S1_RDRF_MASK)//������ܼĴ���Ϊ��
		cmtr_uart[2]->OnIntrRecieveNext();
	NVIC_EnableIRQ(UART1_IRQn);
}


