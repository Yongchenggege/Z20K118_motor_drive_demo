#include "pwm.h"
#include "led.h"
#include "uart.h"
#include "delay.h"
#include "drv10970.h"

uint8_t color[7] = {RED,YELLOW,GREEN,CYAN,BLUE,PURPEL,WHITE};
extern uint8_t p_flag;

void PWM_Confic(float pluse1,float pluse2);
//extern uint32_t __vector_table;

int main(void)
{
//	SCB->VTOR = ((uint32_t)&__vector_table) & 0xFFFFFF00U;
//	__DSB();
	
	CLK_OSC40MEnable2(CLK_OSC_FREQ_MODE_HIGH, ENABLE, CLK_OSC_XTAL); // 系统时钟配置
	while(ERR == CLK_SysClkSrc(CLK_SYS_OSC40M)); // 选择系统时钟
	
	WDOG_Disable(); // 关闭看门口狗
	
	SYSCTRL_EnableModule(SYSCTRL_GPIO); // 使能GPIO
	
	CLK_SetClkDivider(CLK_CORE, CLK_DIV_1); // 配置内核时钟为
	led_init();	

	
	delay_init();
//	uart0_init(115200);
	uart1_init(115200);
	drv10970_init();


	

	
//	TIM_SetCCVal(TIM0_ID,TIM_CHANNEL_6,(UINT16_MAX*(30/100.0))); // 设置定时器0通道1的占空比为50%
//	TIM_SetCCVal(TIM0_ID,TIM_CHANNEL_7,(UINT16_MAX*(70/100.0))); // 设置定时器0通道1的占空比为50%
//	
//	TIM_ReloadSyncCmd(TIM0_ID, ENABLE);	
	
	while(1)
	{
		switch(p_flag)
		{
			case 1:PWM_Confic(0,0);fr_control_dir(reverse,reverse);p_flag =0;break;//AA 00 BB
			case 2:PWM_Confic(0.2,0.2);fr_control_dir(forward,forward);p_flag =0;break;//AA 11 BB
			case 3:PWM_Confic(0.4,0.4);fr_control_dir(forward,forward);p_flag =0;break;//AA 12 BB
			case 4:PWM_Confic(0.6,0.6);fr_control_dir(forward,forward);p_flag =0;break;//AA 13 BB
			case 5:PWM_Confic(0.8,0.8);fr_control_dir(forward,forward);p_flag =0;break;	//AA 14 BB
			case 6:PWM_Confic(0.9,0.9);fr_control_dir(forward,forward);p_flag =0;break;//AA 15 BB
			case 7:PWM_Confic(1,1);fr_control_dir(forward,forward);p_flag =0;break;	//AA 16 BB
			
			case 8:PWM_Confic(0.2,0.2);fr_control_dir(reverse,reverse);p_flag =0;break;//AA 01 BB
			case 9:PWM_Confic(0.4,0.4);fr_control_dir(reverse,reverse);p_flag =0;break;//AA 02 BB
			case 10:PWM_Confic(0.6,0.6);fr_control_dir(reverse,reverse);p_flag =0;break;//AA 03 BB
			case 11:PWM_Confic(0.8,0.8);fr_control_dir(reverse,reverse);p_flag =0;break;//AA 04 BB
			case 12:PWM_Confic(0.9,0.9);fr_control_dir(reverse,reverse);p_flag =0;break;//AA 05 BB
			case 13:PWM_Confic(1,1);fr_control_dir(reverse,reverse);p_flag =0;break;//AA 06 BB			
			default:break;
		
		}
		
//		for(int i=0;i<sizeof(color);i++)
//		{
//			RGB_Set_Color(color[i]);
//			
////			uart0_printf("color[%d]=%b\n",i,color[i]);
//			uart1_printf("color[%d]=%b\n",i,color[i]);
//			
//			switch(color[i])
//			{
//				case RED:printf("RED\n");break;
//				case YELLOW:printf("YELLOW\n");break;
//				case GREEN:printf("GREEN\n");break;
//				case CYAN:printf("CYAN\n");break;
//				case BLUE:printf("BLUE\n");break;
//				case PURPEL:printf("PURPEL\n");break;
//				case WHITE:printf("WHITE\n");break;
//				default:break;
//			}
//			
//			delay_ms(1000);
//		}
	}
}


void PWM_Confic(float pluse1,float pluse2)
{
		TIM_SetCCVal(TIM0_ID,TIM_CHANNEL_6,UINT16_MAX*pluse1); // 设置定时器0通道1的占空比为50%
		TIM_SetCCVal(TIM0_ID,TIM_CHANNEL_7,UINT16_MAX*pluse2); // 设置定时器0通道1的占空比为50%
		TIM_ReloadSyncCmd(TIM0_ID,ENABLE);
}
