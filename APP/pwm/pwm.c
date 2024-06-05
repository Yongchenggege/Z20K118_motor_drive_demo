#include "pwm.h"

void pwm1_init(void)
{
	CLK_ModuleSrc(CLK_PORTE, CLK_SRC_OSC40M); // 配置GPIOE的时钟为CLK_SRC_OSC40M
	SYSCTRL_EnableModule(SYSCTRL_PORTE); // 使能GPIOE
	
	CLK_ModuleSrc(CLK_TIM0, CLK_SRC_OSC40M); // 配置定时器0的时钟为CLK_SRC_OSC40M
	CLK_SetClkDivider(CLK_TIM0, CLK_DIV_1); // 对定时器的时钟进行分频
	
	SYSCTRL_ResetModule(SYSCTRL_TIM0); // 复位定时器0
	SYSCTRL_EnableModule(SYSCTRL_TIM0); // 使能定时器0
	
	TIM_PwmChannelConfig_t pwmChConfig; // 定义pwm配置结构体
	
	pwmChConfig.channelId = TIM_CHANNEL_6; // 定时器的通道0输出pwm 配置定时器哪个通道输出pwm
	pwmChConfig.pwmModeConfig = TIM_PWM_LOW_TRUE_PULSE; // pwm模式 与stm32的pwm类同
	pwmChConfig.polarity = TIM_POL_LOW; // 有效电平为低 TIM_PWM_LOW_TRUE_PULSE 模式下 计数值小于比较值输出有效电平 即小于时为高,大于时为低 
	pwmChConfig.compareValue = 0; // 设置pwm的极性反转时的值 即比较值
	pwmChConfig.faultCtrlChannelEnable = DISABLE; // 不配置通道为定时器的故障通道
	pwmChConfig.ccvUpdateEnable = ENABLE; // 不开启ccv重载 ccv中断即当定时器计数值到达compareValue时产生重载
	
	TIM_PwmConfig_t pwmConfig;
	
	pwmConfig.numChannels = 1; // pwm的通道数 即TIM_PwmChannelConfig_t的数量
	pwmConfig.minCounterVal = 0; // 定时器的计数的初始值 设置为0即从0开始计数
	pwmConfig.maxCounterVal = UINT16_MAX; // 配置为16位定时器的最大值
	pwmConfig.pwmChConfig = &pwmChConfig; // 将pwmChConfig指针指向定时器所有pwm配置的地址,初始化函数循环遍历初始化 numChannels的值与pwmChConfig数量保持一直
	
	TIM_OutputEdgeAlignedPwmConfig(TIM0_ID,&pwmConfig); // pwm初始化 这里初始化定时器0的通道0
	
	pwmChConfig.channelId = TIM_CHANNEL_7; // 定时器的通道0输出pwm 配置定时器哪个通道输出pwm
	pwmChConfig.pwmModeConfig = TIM_PWM_LOW_TRUE_PULSE; // pwm模式 与stm32的pwm类同
	pwmChConfig.polarity = TIM_POL_LOW; // 有效电平为低 TIM_PWM_LOW_TRUE_PULSE 模式下 计数值小于比较值输出有效电平 即小于时为高,大于时为低 
	pwmChConfig.compareValue = 0; // 设置pwm的极性反转时的值 即比较值
	pwmChConfig.faultCtrlChannelEnable = DISABLE; // 不配置通道为定时器的故障通道
	pwmChConfig.ccvUpdateEnable = ENABLE; // 不开启ccv重载 ccv中断即当定时器计数值到达compareValue时产生重载
	
	pwmConfig.numChannels = 1; // pwm的通道数 即TIM_PwmChannelConfig_t的数量
	pwmConfig.minCounterVal = 0; // 定时器的计数的初始值 设置为0即从0开始计数
	pwmConfig.maxCounterVal = UINT16_MAX; // 配置为16位定时器的最大值
	pwmConfig.pwmChConfig = &pwmChConfig; // 将pwmChConfig指针指向定时器所有pwm配置的地址,初始化函数循环遍历初始化 numChannels的值与pwmChConfig数量保持一直
	
	TIM_OutputEdgeAlignedPwmConfig(TIM0_ID,&pwmConfig); // pwm初始化 这里初始化定时器0的通道0
	
	TIM_ReloadConfig_t reloadConfig; // 定时器计数器重载配置结构体
	
	reloadConfig.reloadMode = TIM_RELOAD_FULL_CYCLE; // 配置重载模式为maxCounterVal时重载
	reloadConfig.loadFrequency = 0; // 这里表示经过n+1次(计数到重载值时)才触发重载中断 这里设置0即第一次计数到重载值就重载中断
	reloadConfig.numChannels = 0; // 这里表示channelMatchConfig数量为0
	reloadConfig.channelMatchConfig = NULL; // 配置ccv重载中断
	
	TIM_PwmSyncConfig_t pwmSyncConfig; // pwm同步配置
	
	pwmSyncConfig.cntinitSync = TIM_UPDATE_PWM_SYN; // 
	pwmSyncConfig.outswcSync = TIM_UPDATE_PWM_SYN; // 
	pwmSyncConfig.syncSWTriggerEnable = DISABLE; // 
	pwmSyncConfig.syncReloadEnable = ENABLE; // 
	pwmSyncConfig.reloadConfig = &reloadConfig; // 
	
	TIM_SyncConfig(TIM0_ID,&pwmSyncConfig); // 重载配置
	
	TIM_CountingModeConfig(TIM0_ID,TIM_COUNTING_UP); // 计数模式配置 向上计数
	
	PORT_PinmuxConfig(PORT_E, GPIO_8, PTE8_TIM0_CH6); // 把GPIOE的pin8 复位为定时器0的通道6
	PORT_PinmuxConfig(PORT_E, GPIO_9, PTE9_TIM0_CH7); // 把GPIOE的pin9 复位为定时器0的通道7
	
	TIM_ChannelOutputEnable(TIM0_ID,  TIM_CHANNEL_6); // 开始定时器0的通道6输出pwm
	TIM_ChannelOutputEnable(TIM0_ID,  TIM_CHANNEL_7); // 开始定时器0的通道7输出pwm
	
	TIM_StartCounter(TIM0_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1); // 配置计数器的时钟配定时器时钟不分配，40M/UINT16_MAX = 610Hz pwm频率 并且开始计数
	//
}


