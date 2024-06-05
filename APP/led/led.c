#include "led.h"

void led_init(void)
{
	CLK_ModuleSrc(CLK_PORTB, CLK_SRC_OSC40M); // 配置GPIOB的时钟为CLK_SRC_OSC40M
	SYSCTRL_EnableModule(SYSCTRL_PORTB); // 使能GPIOB
	
	CLK_ModuleSrc(CLK_PORTD, CLK_SRC_OSC40M); // 配置GPIOD的时钟为CLK_SRC_OSC40M
	SYSCTRL_EnableModule(SYSCTRL_PORTD); // 使能GPIOD
	
	PORT_PinmuxConfig(PORT_B, GPIO_5, PTB5_GPIO); // R
	
	PORT_PinmuxConfig(PORT_B, GPIO_4, PTB4_GPIO); // G
	
	PORT_PinmuxConfig(PORT_D, GPIO_16, PTD16_GPIO); // B
	
	PORT_PinConfig_t pinConfig;
	
	pinConfig.pullConfig = PORT_PULL_DISABLED; // 无上下拉
	pinConfig.slewRateConfig = PORT_FAST_RATE; // 电平切换速度 快速
	pinConfig.passiveFilterConfig = DISABLE; //
	pinConfig.openDrainConfig = DISABLE; // 不使用开漏输出
	pinConfig.driveStrengthConfig = PORT_HIGH_DRIVE_STRENGTH; // 低驱动能力
	pinConfig.filterEnable = DISABLE; //
	pinConfig.filterConfig = 0; //
	
	// 初始化IO
	PORT_PinInit(PORT_B,GPIO_5,&pinConfig);
	
	PORT_PinInit(PORT_B,GPIO_4,&pinConfig);
	
	PORT_PinInit(PORT_D,GPIO_16,&pinConfig);
	
	// 配置 IO 输出 模式
	GPIO_SetPinDir(PORT_B,GPIO_5,GPIO_OUTPUT);
	
	GPIO_SetPinDir(PORT_B,GPIO_4,GPIO_OUTPUT);
	
	GPIO_SetPinDir(PORT_D,GPIO_16,GPIO_OUTPUT);
	
	// 初始化IO电平
	GPIO_WritePinOutput(PORT_B,GPIO_5,GPIO_LOW);
	
	GPIO_WritePinOutput(PORT_B,GPIO_4,GPIO_LOW);
	
	GPIO_WritePinOutput(PORT_D,GPIO_16,GPIO_LOW);
}

// color xxxx xrgb
void RGB_Set_Color(LED_COLOR color)
{
	color & 0x04 ? GPIO_WritePinOutput(PORT_B,GPIO_5,GPIO_HIGH) : GPIO_WritePinOutput(PORT_B,GPIO_5,GPIO_LOW);
	
	color & 0x02 ? GPIO_WritePinOutput(PORT_B,GPIO_4,GPIO_HIGH) : GPIO_WritePinOutput(PORT_B,GPIO_4,GPIO_LOW);
	
	color & 0x01 ? GPIO_WritePinOutput(PORT_D,GPIO_16,GPIO_HIGH) : GPIO_WritePinOutput(PORT_D,GPIO_16,GPIO_LOW);
}
