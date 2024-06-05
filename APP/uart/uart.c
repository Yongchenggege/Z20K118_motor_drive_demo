#include "uart.h"
#include "stdarg.h"
#include "math.h"
#include "string.h"

static int uart_printf(UART_ID_t uartId,const char* format,va_list pArgs);
usart_pack pack;
uint8_t p_flag;

#ifdef UART0_EN
#if UART0_EN

void uart0_init(uint32_t baudRate)
{
	CLK_ModuleSrc(CLK_UART0, CLK_SRC_OSC40M); // 配置串口0时钟
	CLK_SetClkDivider(CLK_UART0, CLK_DIV_1);  // 串口0时钟不分频
	SYSCTRL_ResetModule(SYSCTRL_UART0); // 复位串口0
	SYSCTRL_EnableModule(SYSCTRL_UART0); // 时钟串口0外设
	
	CLK_ModuleSrc(CLK_PORTC, CLK_SRC_OSC40M); // 配置串口0的GPIO时钟
	SYSCTRL_EnableModule(SYSCTRL_PORTC); // 时钟串口0的GPIO
	CLK_ModuleSrc(CLK_PORTA, CLK_SRC_OSC40M); // 配置串口0的GPIO时钟
	SYSCTRL_EnableModule(SYSCTRL_PORTA); // 时钟串口0的GPIO
	
	
	PORT_PinmuxConfig(PORT_A, GPIO_3, PTA3_UART0_TX); // 把PTC3 复用为uart0_tx
	
	PORT_PinmuxConfig(PORT_A, GPIO_2, PTA2_UART0_RX); // 把PTC2 复用为uart0_rx
	
	UART_Config_t tUartCfgStruct;
	
	tUartCfgStruct.baudRate = baudRate;
	tUartCfgStruct.dataBits = UART_DATABITS_8;
	tUartCfgStruct.parity = UART_PARITY_NONE;
	tUartCfgStruct.stopBits = UART_STOPBITS_1;
	tUartCfgStruct.autoFlowControl = DISABLE;
	tUartCfgStruct.oscFreq = 40000000;
	
	while(ERR == UART_WaitBusyClear(UART0_ID, 7000))
	{
			(void)UART_ReceiveByte(UART0_ID);
	}
	
	UART_InstallCallBackFunc(UART0_ID,UART_INT_RBFI,uart0_callBack); // 开始接收中断 设置中断回调函数
	/* enable received data available interrupt  */
    UART_IntMask(UART0_ID, UART_INT_RBFI, UNMASK);

    /* enable IRQ */
    NVIC_EnableIRQ(UART0_IRQn);
	
	UART_Init(UART0_ID, &tUartCfgStruct);
}

__weak void uart0_callBack(void)
{
	uint8_t ch;
	
	if(SET == UART_GetLineStatus(UART0_ID,UART_LINESTA_DR))
	{
		ch = UART_ReceiveByte(UART0_ID);
		
		UART_SendByte(UART0_ID, ch);
	}
}

#if UART0_PRINT_EN
__weak int uart0_printf(const char* format,...)
{
	unsigned int ret = 0;
	
	va_list pArgs; // 定义va_list类型指针，用于存储参数的地址
	
	va_start(pArgs,format); // 初始化pArgs
	
	ret = uart_printf(UART0_ID,format,pArgs);
	
	va_end(pArgs);// 结束取参数
	
	return ret;
}
#endif

#if UART0_LIBC_PRINT_EN
int fputc(int ch,FILE *p)
{
	UART_SendByte(UART0_ID, ch);
	while(UART_GetLineStatus(UART0_ID,UART_LINESTA_TEMT) == RESET);
	return ch;
}
#endif

#endif
#endif

#ifdef UART1_EN
#if UART1_EN

void uart1_init(uint32_t baudRate)
{
	CLK_ModuleSrc(CLK_UART1, CLK_SRC_OSC40M); // 配置串口1时钟
	CLK_SetClkDivider(CLK_UART1, CLK_DIV_1);  // 串口1时钟不分频
	SYSCTRL_EnableModule(SYSCTRL_UART1); // 时钟串口1外设
	
	CLK_ModuleSrc(CLK_PORTC, CLK_SRC_OSC40M); // 配置串口1的GPIO时钟
	SYSCTRL_EnableModule(SYSCTRL_PORTC); // 时钟串口1的GPIO
	
	PORT_PinmuxConfig(PORT_C, GPIO_7, PTC7_UART1_TX); // 把PTC3 复用为uart1_tx
	
	PORT_PinmuxConfig(PORT_C, GPIO_6, PTC6_UART1_RX); // 把PTC2 复用为uart1_rx
	
	UART_Config_t tUartCfgStruct;
	
	tUartCfgStruct.baudRate = baudRate;
	tUartCfgStruct.dataBits = UART_DATABITS_8;
	tUartCfgStruct.parity = UART_PARITY_NONE;
	tUartCfgStruct.stopBits = UART_STOPBITS_1;
	tUartCfgStruct.autoFlowControl = DISABLE;
	tUartCfgStruct.oscFreq = 40000000;
	
	while(ERR == UART_WaitBusyClear(UART1_ID, 7000))
	{
			(void)UART_ReceiveByte(UART1_ID);
	}
	
	UART_InstallCallBackFunc(UART1_ID,UART_INT_RBFI,uart1_callBack); // 开始接收中断 设置中断回调函数
	/* enable received data available interrupt  */
    UART_IntMask(UART1_ID, UART_INT_RBFI, UNMASK);

    /* enable IRQ */
    NVIC_EnableIRQ(UART1_IRQn);
	
	UART_Init(UART1_ID, &tUartCfgStruct);
}

__weak void uart1_callBack(void)
{
	static uint8_t data[3];
	static uint16_t i=0;
	if(SET == UART_GetLineStatus(UART1_ID,UART_LINESTA_DR))
	{
		data[i++] = UART_ReceiveByte(UART1_ID);
		UART_SendByte(UART1_ID, data[i-1]);
		if(i==3) {
			i=0;
			if(data[0] == 0xaa && data[2] == 0xbb) {
				pack.head = data[0];
				pack.end = data[2];
				pack.data[0] = data[1];
				switch(pack.data[0]) {
					case 0x00:
						p_flag = 1;
						break;
					case 0x11:
						p_flag = 2;
						break;
					case 0x12:
						p_flag = 3;
						break;
					case 0x13:
						p_flag = 4;
						break;
					case 0x14:
						p_flag = 5;
						break;
					case 0x15:
						p_flag = 6;
						break;
					case 0x16:
						p_flag = 7;
						break;	

					case 0x01:
						p_flag = 8;
						break;
					case 0x02:
						p_flag = 9;
						break;
					case 0x03:
						p_flag = 10;
						break;
					case 0x04:
						p_flag = 11;
						break;
					case 0x05:
						p_flag = 12;
						break;
					case 0x06:
						p_flag = 13;
						break;				
					default:
						break;
				}
			}
		}
	}
}

#if UART1_PRINT_EN
__weak int uart1_printf(const char* format,...)
{
	unsigned int ret = 0;
	
	va_list pArgs; // 定义va_list类型指针，用于存储参数的地址
	
	va_start(pArgs,format); // 初始化pArgs
	
	ret = uart_printf(UART1_ID,format,pArgs);
	
	va_end(pArgs);// 结束取参数
	
	return ret;
}
#endif

#if UART1_LIBC_PRINT_EN
int fputc(int ch,FILE *p)
{
	UART_SendByte(UART1_ID, ch);
	while(UART_GetLineStatus(UART1_ID,UART_LINESTA_TEMT) == RESET);
	return ch;
}
#endif

#endif
#endif

static void uart_send_char(UART_ID_t uartId,char ch)
{
	UART_SendByte(uartId, ch);
	while(UART_GetLineStatus(uartId,UART_LINESTA_TEMT) == RESET);
}

static int uart_printf(UART_ID_t uartId,const char* format,va_list pArgs)
{
	if (format == NULL) return -1;
	
	unsigned int ret_num = 0;// 返回打印字符的个数
	char* pStr = (char*)format;// 指向str
	int ArgIntVal = 0;  // 接收整型
	unsigned long ArgHexVal = 0;// 接十六进制
	char* ArgStrVal = NULL;  // 接收字符型
	double ArgFloVal = 0.0; // 接受浮点型
	unsigned long val_seg = 0;   // 数据切分
	unsigned long val_temp = 0;  // 临时保存数据
	int cnt = 0;       // 数据长度计数
	int i = 0;
	
	while (*pStr != '\0')
	{
		switch (*pStr)
		{
			case ' ':
				uart_send_char(uartId,*pStr); ret_num++; break;
			case '\t':
				uart_send_char(uartId,*pStr); ret_num += 4; break;
			case '\r':
				uart_send_char(uartId,*pStr); ret_num++; break;
			case '\n':
				uart_send_char(uartId,*pStr); ret_num++; break;
			case '%':
				pStr++;
				// % 格式解析
				switch (*pStr)
				{
					case '%':
						uart_send_char(uartId,'%');// %%，输出%
						ret_num++;
						pStr++;
						continue;
					case 'c':
						ArgIntVal = va_arg(pArgs, int);// %c，输出char
						uart_send_char(uartId,(char)ArgIntVal);
						ret_num++;
						pStr++;
						continue;
					case 'd':
						// 接收整型
						ArgIntVal = va_arg(pArgs, int);
						if (ArgIntVal < 0)// 如果为负数打印，负号
						{
							ArgIntVal = -ArgIntVal;// 取相反数

							uart_send_char(uartId,'-');
							ret_num++;
						}
						val_seg = ArgIntVal;// 赋值给 val_seg处理数据
						// 计算ArgIntVal长度
						if (ArgIntVal)
						{
							while (val_seg) {
								cnt++;
								val_seg /= 10;
							}
						}
						else cnt = 1;// 数字0的长度为1

						ret_num += cnt;// 字符个数加上整数的长度

						// 将整数转为单个字符打印
						while (cnt)
						{
							val_seg = ArgIntVal / pow(10, cnt - 1);
							ArgIntVal %= (int)pow(10, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						pStr++;
						continue;
					case 'o':
						// 接收整型
						ArgIntVal = va_arg(pArgs, int);
						if (ArgIntVal < 0)// 如果为负数打印，负号
						{
							ArgIntVal = -ArgIntVal;// 取相反数

							uart_send_char(uartId,'-');
							ret_num++;
						}
						val_seg = ArgIntVal;// 赋值给 val_seg处理数据
						// 计算ArgIntVal长度
						if (ArgIntVal)
						{
							while (val_seg) {
								cnt++;
								val_seg /= 8;
							}
						}
						else cnt = 1;// 数字0的长度为1

						ret_num += cnt;// 字符个数加上整数的长度

						// 将整数转为单个字符打印
						while (cnt)
						{
							val_seg = ArgIntVal / pow(8, cnt - 1);
							ArgIntVal %= (int)pow(8, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						pStr++;
						continue;
					case 'x':
						// 接收16进制
						ArgHexVal = va_arg(pArgs, unsigned long);
						val_seg = ArgHexVal;
						// 计算ArgIntVal长度
						if (ArgHexVal)
						{
							while (val_seg) {
								cnt++;
								val_seg /= 16;
							}
						}
						else cnt = 1;// 数字0的长度为1

						ret_num += cnt;// 字符个数加上整数的长度
						// 将整数转为单个字符打印
						while (cnt)
						{
							val_seg = ArgHexVal / pow(16, cnt - 1);
							ArgHexVal %= (int)pow(16, cnt - 1);
							if (val_seg <= 9)
								uart_send_char(uartId,(char)val_seg + '0');
							else
							{
								//uart_send_char(uartId,(char)val_seg - 10 + 'a'); //小写字母
								uart_send_char(uartId,(char)val_seg - 10 + 'A');
							}
							cnt--;
						}
						pStr++;
						continue;
					case 'b':
						// 接收整型
						ArgIntVal = va_arg(pArgs, int);
						val_seg = ArgIntVal;
						// 计算ArgIntVal长度
						if (ArgIntVal)
						{
								while (val_seg) {
										cnt++;
										val_seg /= 2;
								}
						}
						else cnt = 1;// 数字0的长度为1

						ret_num += cnt;// 字符个数加上整数的长度
						// 将整数转为单个字符打印
						while (cnt)
						{
								val_seg = ArgIntVal / pow(2, cnt - 1);
								ArgIntVal %= (int)pow(2, cnt - 1);
								uart_send_char(uartId,(char)val_seg + '0');
								cnt--;
						}
						pStr++;
						continue;
					case 's':
						// 接收字符
						ArgStrVal = va_arg(pArgs, char*);
						ret_num += (unsigned int)strlen(ArgStrVal);
						while (*ArgStrVal)
						{
							uart_send_char(uartId,*ArgStrVal);
							ArgStrVal++;
						}
						pStr++;
						continue;
					case 'f':
						// 接收浮点型 保留6为小数，不采取四舍五入
						ArgFloVal = va_arg(pArgs, double);
						val_seg = (unsigned long)ArgFloVal;// 取整数部分
						val_temp = val_seg;      // 临时保存整数部分数据
						ArgFloVal = ArgFloVal - val_seg;// 得出余下的小数部分
						// 计算整数部分长度
						if (val_seg)
						{
							while (val_seg) {
									cnt++;
									val_seg /= 10;
							}
						}
						else cnt = 1;// 数字0的长度为1
						ret_num += cnt;// 字符个数加上整数的长度
						// 将整数转为单个字符打印
						while (cnt)
						{
							val_seg = val_temp / pow(10, cnt - 1);
							val_temp %= (int)pow(10, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						// 打印小数点
						uart_send_char(uartId,'.');
						ret_num++;
						// 开始输出小数部分
						ArgFloVal *= 1000000;
						// printf("\r\n %f\r\n", ArgFloVal);
						cnt = 6;
						val_temp = (int)ArgFloVal;// 取整数部分
						while (cnt)
						{
							val_seg = val_temp / pow(10, cnt - 1);
							val_temp %= (int)pow(10, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						ret_num += 6;
						pStr++;
						continue;
					default:// % 匹配错误，暂输出空格
						uart_send_char(uartId,' ');
						ret_num++;
						continue;
					}
			default:
					uart_send_char(uartId,*pStr); ret_num++;
					break;
		}
		pStr++;
	}

	return ret_num;
}
