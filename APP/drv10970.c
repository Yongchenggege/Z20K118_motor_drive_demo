#include "drv10970.h"


static float motor1_speed;
static float motor2_speed;


//FR1 PTA12  FR2  PTA13
static void FR_init(void)
{
	CLK_ModuleSrc(CLK_PORTB, CLK_SRC_OSC40M); // 配置GPIOB的时钟为CLK_SRC_OSC40M
	SYSCTRL_EnableModule(SYSCTRL_PORTA); // 使能GPIOA
	
	PORT_PinmuxConfig(PORT_A, GPIO_12, PTA12_GPIO); // FR1
	
	PORT_PinmuxConfig(PORT_A, GPIO_13, PTA13_GPIO); // FR2

	
	PORT_PinConfig_t pinConfig;
	
	pinConfig.pullConfig = PORT_PULL_DISABLED; // 无上下拉
	pinConfig.slewRateConfig = PORT_FAST_RATE; // 电平切换速度 快速
	pinConfig.passiveFilterConfig = DISABLE; //
	pinConfig.openDrainConfig = DISABLE; // 不使用开漏输出
	pinConfig.driveStrengthConfig = PORT_HIGH_DRIVE_STRENGTH; // 低驱动能力
	pinConfig.filterEnable = DISABLE; //
	pinConfig.filterConfig = 0; //
	
	// 初始化IO
	PORT_PinInit(PORT_A,GPIO_12,&pinConfig);
	
	PORT_PinInit(PORT_A,GPIO_13,&pinConfig);
	
	// 配置 IO 输出 模式
	GPIO_SetPinDir(PORT_A,GPIO_12,GPIO_OUTPUT);
	
	GPIO_SetPinDir(PORT_A,GPIO_13,GPIO_OUTPUT);
	
	// 初始化IO电平
	GPIO_WritePinOutput(PORT_A,GPIO_12,GPIO_LOW);
	
	GPIO_WritePinOutput(PORT_A,GPIO_13,GPIO_LOW);
	
	
}
//get speed
static float fg_get_spped1(void)
{
	int16_t cnt = get_IC1_cnt();
	float circle,speed;

	
	circle = cnt/(MOTOR_PULSE*4.0);
	
	speed = circle*(1/(SPEED_INTERVAL*0.0001));
	
	return speed;	
}


static float fg_get_speed2(void)
{
	int16_t cnt = get_IC2_cnt();
	float circle,speed;
	
	circle = cnt/(MOTOR_PULSE*4.0);
	
	speed = circle*(1/(SPEED_INTERVAL*0.0001));
	
	return speed;

}

float get_motor_speed(motor_type motor)
{
	switch(motor)
	{
		case motor1:return motor1_speed;
		case motor2:return motor2_speed;
		default :return motor1_speed;
	}
}

static void tim2_ch2_cbf(void)
{
	motor1_speed = fg_get_spped1();
	motor2_speed = fg_get_speed2();
	
}

static void tim2_ch2_init(void)
{
	
	CLK_ModuleSrc(CLK_TIM2, CLK_SRC_OSC40M); // 配置定时器0的时钟为CLK_SRC_OSC40M
	CLK_SetClkDivider(CLK_TIM2, CLK_DIV_1); // 对定时器的时钟进行分频
	
	SYSCTRL_ResetModule(SYSCTRL_TIM2); // 复位定时器0
	SYSCTRL_EnableModule(SYSCTRL_TIM2); // 使能定时器0
	

	 /* callback function */
    TIM_InstallCallBackFunc(TIM2_ID, TIM_INT_CH2, tim2_ch2_cbf); 
    /* counting mode: up counting mode */
    TIM_CountingModeConfig(TIM2_ID, TIM_COUNTING_UP);
	
	TIM_StartCounter(TIM2_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1);	//610hz
	 /* enable the interrupt */
    TIM_IntMask(TIM2_ID, TIM_INT_CH2, UNMASK);
    /* enable IRQ*/
    NVIC_EnableIRQ(TIM2_IRQn);


}


void drv10970_init(void)
{
	
	FR_init();//fr
	pwm1_init();
	tim2_ch2_init();
}


//set dir
void fr_control_dir(fr_dir dir1,fr_dir dir2)
{
	switch(dir1)
	{
		case reverse:GPIO_WritePinOutput(PORT_A,GPIO_12,GPIO_LOW);break;
		case forward:GPIO_WritePinOutput(PORT_A,GPIO_12,GPIO_HIGH);break;
		default:break;
		
	}
	switch(dir2)
	{
		case reverse:GPIO_WritePinOutput(PORT_A,GPIO_13,GPIO_LOW);break;
		case forward:GPIO_WritePinOutput(PORT_A,GPIO_13,GPIO_HIGH);break;
		default:break;		
		
	}
}