#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"



//PE0 = leftmost, PE4=rightmost
void Port_E_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10;			// 1) activate clock for Port E 
	while((SYSCTL_PRGPIO_R&0x10) == 0){}; //wait
	GPIO_PORTE_DIR_R &= ~0x1F; //inputs on PE 0-3
	GPIO_PORTE_DEN_R |= 0x1F; //enable
	GPIO_PORTE_AFSEL_R = 0x00; // 6) disable alt funct on PE7-0
}
