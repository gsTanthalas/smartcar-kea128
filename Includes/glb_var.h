/*
 * glb_variables.h
 *
 *  Created on: 2018��12��16��
 *      Author: ��Ц��
 *
 *      ����ȫ�ֱ��������ļ�
 *      ��Ҫ��ʾ����������ʹ��ȫ�ֱ���������ԭ����ֻ����ʹ�����¼���ȫ�ַ���
 *      1.Ӳ�������궨���Ч�����ܾ����Ƶ�ȫ�ֱ���������Ҫ�ڴ��ļ��ж���
 *      2.���������Ͷ��壨��Ȼ��
 *      3.�����и����ֹ��õĹ���������ȷʵ���ڹ����������붨���ڴ��ļ��У�����__glb_variables_init()�г�ʼ��������˴��еĴ��ڡ�����������ȵ�
 *      4.ö��������������Զ��������ռ��У���ֹ��Ⱦȫ�ֺ�std�����ռ�
 *      ���ò������Ա�������ݵ���Ϣ���ö���Ϊȫ�ֱ�������Ҫ���Լ���ͬѧ������ν�Ĺ��������мǣ�
 */

#ifndef INCLUDES_GLB_VAS_H_
#define INCLUDES_GLB_VAS_H_

#include "communicate.h"
#include "adc.h"


extern UARTCommunicator *g_uartc;

extern ADCModule *g_test_adc;


#endif /* INCLUDES_GLB_VAS_H_ */
