/*
 * cppmain.h
 *
 *  Created on: 2018��12��15��
 *      Author: ��Ц��
 */

#ifndef INCLUDES_CPPMAIN_H_
#define INCLUDES_CPPMAIN_H_


#ifdef __cplusplus
extern "C"{
#endif

extern int _cppmain();

/*............�������жϴ�����............*/
volatile void __attribute__ ((noinline)) UART0_IRQHandler();
volatile void __attribute__ ((noinline)) UART1_IRQHandler();
volatile void __attribute__ ((noinline)) UART2_IRQHandler();
#ifdef __cplusplus
}
#endif


#endif /* INCLUDES_CPPMAIN_H_ */
