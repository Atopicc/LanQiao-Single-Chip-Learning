#include "main.h"
#include "seg.h"
#include "onewire.h"
#include "kbd.h"
#include "sonic.h"
#include "uart.h"
#include "pwm.h"
#include "frequence.h"
#include "iic.h"

unsigned int cnt_uart;
unsigned int cnt_frequence;
unsigned int temperature;
unsigned int frequence;
unsigned char duty;

//void kbd_three_line(void);
void timer1_init(void);

void timer1_init(void)         //time1定时计数器配置为100ms的定时中断
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;	    	//清除TF1标志
	TR1 = 1;	    	//定时器1开始计时
	ET1 = 1;        //time1中断分允许
	EA = 1;         //中断总允许
}

void tm1_isr() interrupt 3
{
	cnt_frequence++;
	cnt_uart++;
	display();
}

void main()
{
	unsigned char temp;
	timer1_init();
	Uart1_Int_T2();
	while(1)
	{
//		if(pwm(duty)) {latch(4,0x00);}
//		else {latch(4,0xFF);}
		if(pwm(duty)) {latch(5,0x00);}
		else {latch(5,0x04);}
		if(cnt_frequence == 1) Timer0_Init_MeasureFreq();
		else if(cnt_frequence >= 500)
		{
			cnt_frequence = 0;
			frequence = (GetFrequence());
		}
		if(cnt_uart > 200)
		{
			cnt_uart = 0;
			temperature = (GetTemputerature()*100);
			sendstr(&temperature);
			duty = PCF8591_ADC(3)/2.56;
		}
		dispbuf[0] = frequence/10000;
		dispbuf[1] = frequence%10000/1000;
		dispbuf[2] = frequence%1000/100;
		dispbuf[3] = frequence%100/10;
		dispbuf[4] = frequence%10;
		
		dispbuf[6] = duty/10;
		dispbuf[7] = duty%10;
		
	}
}

