/*
 * gpio.h
 *
 *  Created on: 2018��12��15��
 *      Author:
 */

#ifndef INCLUDES_GPIO_H_
#define INCLUDES_GPIO_H_

#include <cstdint>
#include "gpio_pindef.h"
#include "SKEAZ1284.h"

extern volatile GPIO_Type *GPIOx[];


#define PTX(PTxn)           ((PTxn)>>5)              //PORTX PORTA PORTB
#define PTn(PTxn)           ((PTxn)&0x1f)           //PIN   0~31
#define GPIOX_BASE(PTxn)     GPIOx[PTX(PTxn)]       //GPIOxģ��ĵ�ַ

#define GPIO_SET(PORTx,n,x)           GPIO_SET_##x((PORTx),(n))                                 //���������ƽx��xΪ0��1   ���� GPIO_SET(PORTA,1,1)  PA1����ߵ�ƽ
#define GPIO_TURN(PORTx,n)           (GPIO_PDOR_REG(GPIOx[(PORTx)]) ^=  (1<<(n)))               //��ת�����ƽ
#define GPIO_GET(PORTx,n)            ((GPIO_PDIR_REG(GPIOx[(PORTx)])>>(n))&0x1)                 //��ȡ��������״̬
//GPIO  1λ����
#define GPIO_SET_1bit(PORTx,n,data)   GPIO_PDOR_REG(GPIOx[(PORTx)])=(( GPIO_PDOR_REG(GPIOx[(PORTx)])& ~(0x1<<(n)) )|(((data)&0x01)<<(n)))   //д1λ���ݣ�nΪ���λ���źţ�
#define GPIO_DDR_1bit(PORTx,n,ddr)    GPIO_PDDR_REG(GPIOx[(PORTx)])=(( GPIO_PDDR_REG(GPIOx[(PORTx)])& ~(0x1<<(n)) )|(((ddr)&0x01)<<(n)))    //����1λ�����������nΪ���λ���źţ�
#define GPIO_GET_1bit(PORTx,n)        (( GPIO_PDIR_REG(GPIOx[(PORTx)])>>(n) ) & 0x1)                                                        //��1λ���ݣ�nΪ���λ���źţ�
//GPIO  8λ����
#define GPIO_SET_8bit(PORTx,n,data)   GPIO_PDOR_REG(GPIOx[(PORTx)])=(( GPIO_PDOR_REG(GPIOx[(PORTx)])& ~(0xff<<(n)) )|(((data)&0xff)<<(n)))  //д8λ���ݣ�nΪ���λ���źţ�
#define GPIO_DDR_8bit(PORTx,n,ddr)    GPIO_PDDR_REG(GPIOx[(PORTx)])=(( GPIO_PDDR_REG(GPIOx[(PORTx)])& ~(0xff<<(n)) )|(((ddr)&0x0ff)<<(n)))  //����8λ�����������nΪ���λ���źţ�
#define GPIO_GET_8bit(PORTx,n)        (( GPIO_PDIR_REG(GPIOx[(PORTx)])>>(n) ) & 0xff)                                                       //��8λ���ݣ�nΪ���λ���źţ�
/****************************�ڲ�ʹ�ã��û�����Ҫ����****************************/
#define GPIO_SET_1(PORTx,n)          GPIO_PDOR_REG(GPIOx[(PORTx)]) |=  (1<<(n))      //�������Ϊ�ߵ�ƽ        ���磺GPIO_SET_H(PORTA,1)   PA1����ߵ�ƽ
#define GPIO_SET_0(PORTx,n)          GPIO_PDOR_REG(GPIOx[(PORTx)]) &= ~(1<<(n))      //�������Ϊ�͵�ƽ        ���磺GPIO_SET_L(PORTA,1)   PA1����͵�ƽ



class GPIOModule
{//TODO:������ڶ�ȡ�����������ģʽ��ȷ��ĳ�����Ƿ��Ѿ���ռ�õĹ���
private:
	PTxn port;
public:
	GPIOModule(PTxn port,bool direction_is_output);
	~GPIOModule();
	inline void SetState(bool state)
	{
		//��Ҫָ�������if else�����Ƕ����
		//��ΪGPIO_SET�����ֻ�ܽ���0����1����������ȥ��λ����Ļ��ͻ�������������ϵĵ�ƽ״̬
		if(state)
		{
			GPIO_SET(PTX(port), PTn(port), 1);
		}
		else
		{
		    GPIO_SET(PTX(port), PTn(port), 0);
		}
	}
	inline void ToggleState()//��ת״̬
	{
		GPIO_TURN(PTX(port), PTn(port));
	}
	inline bool GetState()//��������ģʽ�¿���
	{
		return GPIO_GET(PTX(port),PTn(port));
	}
	inline void SetDirection(bool direction_is_output)
	{
		if (direction_is_output)   //���
		{
		    GPIO_PDDR_REG(GPIOX_BASE(this->port)) |= (1 << PTn(static_cast<uint8_t>(this->port)));  //���ö˿ڷ���Ϊ���
		}
		else
		{
			GPIO_PDDR_REG(GPIOX_BASE(this->port)) &= ~(1 << PTn(this->port));  //���ö˿ڷ���Ϊ����
		}

	}
};

//TODO:�������Ҫ����Ӱ�8λ��������

#endif /* INCLUDES_GPIO_H_ */
