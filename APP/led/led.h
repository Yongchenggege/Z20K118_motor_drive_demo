#ifndef __led_H
#define __led_H

#include "Z20K118_conf.h"

typedef enum {
	
	RED = 0x04,
	GREEN = 0x02,
	BLUE = 0x01,
	YELLOW = 0x06,
	PURPEL = 0x05,
	CYAN = 0x03,
	WHITE = 0x07,
	NONE = 0x00
	
}LED_COLOR;

void led_init(void);
void RGB_Set_Color(LED_COLOR color);

#endif
