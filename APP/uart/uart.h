#ifndef __UART_H
#define __UART_H

#include "Z20K118_conf.h"
#include "stdio.h"

typedef struct usart_pack_{
	uint8_t head;
	uint8_t data[1];
	uint8_t end;
}usart_pack;


#define UART0_EN	0
#ifdef UART0_EN
	#if UART0_EN
		#define UART0_LIBC_PRINT_EN	1
		#define UART0_PRINT_EN			1
	#endif
#endif

#define UART1_EN	1
#ifdef UART1_EN
	#if UART1_EN
		#define UART1_LIBC_PRINT_EN	1
		#define UART1_PRINT_EN			1
		#if UART0_LIBC_PRINT_EN && UART1_LIBC_PRINT_EN
			#error "printf function conflicts"
		#endif
	#endif
#endif

void uart0_init(uint32_t baudRate);
void uart0_callBack(void);
int uart0_printf(const char* format,...);
void uart1_init(uint32_t baudRate);
void uart1_callBack(void);
int uart1_printf(const char* format,...);

#endif
