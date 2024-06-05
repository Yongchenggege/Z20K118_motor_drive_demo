#include "delay.h"

static uint32_t delay_cnt = 0;

static void STIMTEST_IntCallBack(void);

const STIM_Config_t stimConfig = 
{
    .workMode = STIM_FREE_COUNT,
    .compareValue = 64000,    /*counter clock is 64M, compare value =64000,  period = 1ms*/
    .countResetMode = STIM_INCREASE_FROM_0,
    .clockSource = STIM_FUNCTION_CLOCK,
};

void delay_init(void)
{
	CLK_ModuleSrc(CLK_STIM, CLK_SRC_FIRC64M);
	
	SYSCTRL_EnableModule(SYSCTRL_STIM);
	
	STIM_Init(STIM_0,&stimConfig);
	
	STIM_InstallCallBackFunc(STIM_0,STIM_INT,STIMTEST_IntCallBack);
	
	STIM_IntCmd(STIM_0, ENABLE);
	
	STIM_Enable(STIM_0);
	
	NVIC_EnableIRQ(STIM_IRQn);
}

void delay_ms(uint32_t ms)
{
	uint32_t start = delay_cnt;
	
	while((delay_cnt - start) < ms);
	
}

static void STIMTEST_IntCallBack(void)
{
	delay_cnt++;
}