#include "enconder.h"

static volatile uint32_t eage_cnt = 0;

void Ex_TimCallback(void)
{
	eage_cnt++;
}

void input_capture(void)
{
	CLK_ModuleSrc(CLK_TIM1, CLK_SRC_OSC40M);
    SYSCTRL_ResetModule(SYSCTRL_TIM1);
    SYSCTRL_EnableModule(SYSCTRL_TIM1);
	
	CLK_ModuleSrc(CLK_PORTB, CLK_SRC_OSC40M);
    SYSCTRL_EnableModule(SYSCTRL_PORTB);
    SYSCTRL_EnableModule(SYSCTRL_GPIO);
	
	PORT_PinmuxConfig(PORT_B, GPIO_2,PTB2_TIM1_CH0);
	
	TIM_InputChannelConfig_t inputChConfig;
	
	inputChConfig.channelId = TIM_CHANNEL_0; // ??0
	inputChConfig.edgeConfig = TIM_INPUT_EDGE_BOTH; // ????
	inputChConfig.filter = TIM_INPUT_FILTER_DISABLED; // ???
	inputChConfig.resetEnable = DISABLE; // ??????????????
	
	TIM_InputCaptureConfig_t config;
	
	config.numChannels = 1;
	config.maxCounterVal = 65535;
	config.inputChConfig = &inputChConfig;
	
	TIM_InputCaptureInit(TIM1_ID,&config);
	
	TIM_SetCounterInitialVal(TIM1_ID, 0x0000);
	
	TIM_CountingModeConfig(TIM1_ID, TIM_COUNTING_UP);
	
	TIM_IntClear(TIM1_ID, TIM_INT_RLD);
	
	TIM_IntClear(TIM1_ID, TIM_INT_CH0);
	
	TIM_InstallCallBackFunc(TIM1_ID, TIM_INT_CH0, Ex_TimCallback);
	
	TIM_IntMask(TIM1_ID, TIM_INT_CH0, UNMASK);
	
	NVIC_EnableIRQ(TIM1_IRQn);
	
	TIM_StartCounter(TIM1_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1);
}


void IC1_init(void)
{
	CLK_ModuleSrc(CLK_PORTB, CLK_SRC_OSC40M);
    SYSCTRL_EnableModule(SYSCTRL_PORTB);
    SYSCTRL_EnableModule(SYSCTRL_GPIO);
	CLK_SetClkDivider(CLK_CORE, CLK_DIV_1);
    CLK_ModuleSrc(CLK_TIM1, CLK_SRC_OSC40M);
    SYSCTRL_ResetModule(SYSCTRL_TIM1);
    SYSCTRL_EnableModule(SYSCTRL_TIM1);
	
	TIM_DualEdgeChannelConfig_t chconfig1;
	chconfig1.pairId = TIM_PAIR_CHANNEL_0;
	chconfig1.captureMode = TIM_CONTINUOUS_PULSE_CAPTURE;
	chconfig1.mainEdgeConfig = TIM_INPUT_EDGE_RISING;
	chconfig1.secondEdgeConfig = TIM_INPUT_DISABLED;
	chconfig1.filter = TIM_INPUT_FILTER_DISABLED;
	
	TIM_DualEdgeCaptureConfig_t config1;
	config1.numPairs = 1;
	config1.maxCounterVal = UINT16_MAX;
	config1.config = &chconfig1;
	
	
	TIM_DualEdgeCaptureInit(TIM1_ID,&config1);
	TIM_SetCounterInitialVal(TIM1_ID,0x0000);
	TIM_SetCCVal(TIM1_ID,TIM_CHANNEL_0,0x7fff);
	TIM_CountingModeConfig(TIM1_ID,TIM_COUNTING_UP); // 计数模式配置 向上计数
	PORT_PinmuxConfig(PORT_B,GPIO_2,PTB2_TIM1_CH0);		
	TIM_StartCounter(TIM1_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1);

	/* reload flag clear */
    TIM_IntClear(TIM1_ID, TIM_INT_RLD);
    /* clear CHF before dual edge capture */
    TIM_IntClear(TIM1_ID, TIM_INT_CH0);
	/* enable the interrupt */
    TIM_IntMask(TIM2_ID, TIM_INT_CH0, UNMASK);
	
}

void IC2_init(void)
{
	CLK_ModuleSrc(CLK_PORTC, CLK_SRC_OSC40M);
    SYSCTRL_EnableModule(SYSCTRL_PORTC);
    SYSCTRL_EnableModule(SYSCTRL_GPIO);
	
	CLK_SetClkDivider(CLK_CORE, CLK_DIV_1);
    CLK_ModuleSrc(CLK_TIM1, CLK_SRC_OSC40M);
    SYSCTRL_ResetModule(SYSCTRL_TIM2);
    SYSCTRL_EnableModule(SYSCTRL_TIM2);
	
	TIM_DualEdgeChannelConfig_t chconfig1;
	chconfig1.pairId = TIM_PAIR_CHANNEL_0;
	chconfig1.captureMode = TIM_CONTINUOUS_PULSE_CAPTURE;
	chconfig1.mainEdgeConfig = TIM_INPUT_EDGE_RISING;
	chconfig1.secondEdgeConfig = TIM_INPUT_DISABLED;
	chconfig1.filter = TIM_INPUT_FILTER_DISABLED;
	
	TIM_DualEdgeCaptureConfig_t config1;
	config1.numPairs = 1;
	config1.maxCounterVal = UINT16_MAX;
	config1.config = &chconfig1;
	
	TIM_DualEdgeCaptureInit(TIM2_ID,&config1);
	TIM_SetCounterInitialVal(TIM2_ID,0x0000);
	
	TIM_SetCCVal(TIM2_ID,TIM_CHANNEL_0,0x7fff);
	TIM_CountingModeConfig(TIM2_ID,TIM_COUNTING_UP); // 计数模式配置 向上计数
	PORT_PinmuxConfig(PORT_C,GPIO_5,PTC5_TIM2_CH0);	
	TIM_StartCounter(TIM2_ID,TIM_CLK_SOURCE_SYSTEM,TIM_CLK_DIVIDE_1);//40m/65536 = 610HZ
	/* reload flag clear */
    TIM_IntClear(TIM2_ID, TIM_INT_RLD);
    /* clear CHF before dual edge capture */
    TIM_IntClear(TIM2_ID, TIM_INT_CH0);
	/* enable the interrupt */
    TIM_IntMask(TIM2_ID, TIM_INT_CH0, UNMASK);
	
}


uint16_t get_IC1_cnt(void)
{
	uint16_t cnt;
	cnt = (TIM_GetCCVal(TIM1_ID,TIM_CHANNEL_0)-0x7fff);
	
	TIM_SetCCVal(TIM1_ID,TIM_CHANNEL_0,0x7fff);
	return cnt;

}


uint16_t get_IC2_cnt(void)
{
	uint16_t cnt;
	cnt = (TIM_GetCCVal(TIM2_ID,TIM_CHANNEL_0)-0x7fff);
	
	TIM_SetCCVal(TIM2_ID,TIM_CHANNEL_0,0x7fff);	
	
	return cnt;

}