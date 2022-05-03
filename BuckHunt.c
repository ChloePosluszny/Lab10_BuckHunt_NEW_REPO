// BuckHunt.c
//Erik and Chloe
// Runs on TM4C123

// Last Modified: 4/2/2022

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer1.h"
#include "Timer2.h"
#include "Timer0A.h"
#include "Buttons.h"
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
   // execute user task
}



struct Deer{
    uint8_t x; 		//x position
		uint8_t y; 		//y position
		int8_t alive;	//life status (alive or dead) //0 dead 1 alive
		int8_t xold; 	//old x poision for use in redraw
		int8_t yold; 	//old y poision for use in redraw
		int8_t vx; 		// x velocity
		int8_t vy; 		// y velocity
		int8_t w; 		//width(always 32)
		int8_t h; 		//height(always 32)
		uint8_t frame; //0-2
		char doeOrBuck; //0 = doe 1 = buck
		const unsigned short *image;
};
typedef struct Deer DeerStruct;

/*
struct Doe{
    uint8_t x;
		uint8_t y;
		int8_t alive;
		int8_t xold;
		int8_t yold;
		int8_t vx;
		int8_t vy;
		int8_t w;
		int8_t h;
		uint8_t frame; 
		const unsigned short *image;
};
typedef struct Doe Doe_type;
*/

struct target{
    uint8_t x;
		uint8_t y;
		int8_t xold;
		int8_t yold;
		int8_t w;
		int8_t h;
		const unsigned short *image;
	};
typedef struct target target_type;
target_type reticle = {54,80,54,80,16,16,scope};

//Move function for reticle
void move(){
	//update previous values
	reticle.xold =reticle.x;
	reticle.yold =reticle.y;
	//LCD is UPSIDE DOWN!!!
	//move right
	if((GPIO_PORTE_DATA_R & 0x01) == 0x1 &&(reticle.x > 6))
	reticle.x =reticle.x-8;
	//move left
	if((GPIO_PORTE_DATA_R & 0x08) == 0x8 &&(reticle.x <110))
	reticle.x =reticle.x+8;
	
	//move up
	if((GPIO_PORTE_DATA_R & 0x02) == 0x2 &&(reticle.y > 16))
	reticle.y =reticle.y-8;
	
	//move down
	if((GPIO_PORTE_DATA_R & 0x04) == 0x4 &&(reticle.y <150))
	reticle.y =reticle.y+8;
}

void moveDeer(DeerStruct deer){
	deer.xold = deer.x; //set old x and y for drawing over old sprite
	deer.yold = deer.y; 
	
	while(deer.x > 0){
		deer.vx = -1; // change to be a random value from 0 to 4
		deer.x += deer.vx; //change position based on velocity
		deer.vy = 2; //change to be random value between -4 and 4
		deer.y += deer.vy;
	}
	
	ST7735_FillRect(deer.xold,deer.yold-(deer.w-1) /*y-height-1*/,deer.w /*width*/,deer.h /*height*/,0xC9DF); //replace old sprite
	ST7735_DrawBitmap(deer.x,deer.y,reticle.image,deer.h,deer.w); //draw new sprite
}

//Reload
//Check old ADC value, and check new value.
//if the old was low and new is high, reload.

//gametime
uint32_t gametime = 45; //how long the game goes on
void countsec(){ //function to decrease the game time every second.
	gametime--;
};

int main(void){
 DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
	ADC_Init();
	Random_Init(1);
	Output_Init();
	Port_E_Init();
	ST7735_SetRotation(2);
	ST7735_FillScreen(0x0000);   // set screen to black
	
	//language select:
	//remember port F buttons are negative logic
	int language_flag = 0;
	ST7735_SetCursor(1, 1);
	ST7735_OutString("English(left)");
	ST7735_SetCursor(1, 4);
	ST7735_OutString("Espanol(right)");


	//Language Select: Wait for player to make a choice.
	while(language_flag == 0x0){
		
		if((GPIO_PORTE_DATA_R & 0x01) == 0x1) { 
		language_flag =1;
		}
	
		if((GPIO_PORTE_DATA_R & 0x8) == 0x8) {
		language_flag =2;
		
	}
}
	if (language_flag ==1){
	ST7735_FillScreen(0x0000);
  ST7735_SetCursor(4, 1);
	ST7735_OutString("Buck Hunt!");
  ST7735_SetCursor(4, 2);
  ST7735_OutString("Shoot bucks,");
	ST7735_SetCursor(4, 3);
  ST7735_OutString("avoid doe.");
  ST7735_SetCursor(4, 4);
  ST7735_OutString("Good luck!");
	
	Delay100ms(50);
	} 
	if (language_flag ==2){
	ST7735_FillScreen(0x0000);
	ST7735_SetCursor(0, 1);
  ST7735_OutString("Caza de ciervos!");
  ST7735_SetCursor(0, 2);
  ST7735_OutString("Dispara a los-");
	ST7735_SetCursor(0, 3);
	ST7735_OutString("ciervos machos,");
	ST7735_SetCursor(0, 4);
  ST7735_OutString("evitar la cierva.");
  ST7735_SetCursor(0, 5);
  ST7735_OutString("Buena suerte!");
	
	Delay100ms(50);
	}
	
	ST7735_FillScreen(0xC9DF);            // set background to Green.
	ST7735_DrawBitmap (reticle.x,reticle.y,reticle.image,reticle.h,reticle.w); //start by drawing the game reticle.
	
	
	
	//Game Engine Here
	EnableInterrupts(); //enable the interrupts here
	Timer2_Init(&move,80000000/25); //timer to check the reticle's position
	Timer0A_Init(&countsec,80000000); //clock timer for the game
	
	while(1){
		
			if (GPIO_PORTE_DATA_R !=0){ //check for movement
			ST7735_FillRect(reticle.xold,reticle.yold-15,16,16,0xC9DF);
			ST7735_DrawBitmap(reticle.x,reticle.y,reticle.image,reticle.h,reticle.w);
		}
			
		//Move buck and doe
		

		//Display the game time to the player in the top left corner
			if (gametime == 0){break;} //if time is up break out
			
			if(gametime >=10){ //check for position of number to not mess up the count if less than 10
				ST7735_SetCursor(0,0);
				ST7735_OutUDec(gametime);
			}
		
			else {
				ST7735_SetCursor(0,0); //correct for less than 10
				ST7735_OutUDec(0);
				ST7735_SetCursor(1,0); //correct for less than 10
				ST7735_OutUDec(gametime);
			}
			
			
			
}
	//Finished with game, display score and the 
	DisableInterrupts(); //turn off interupts
	ST7735_FillScreen(0x0000);
	ST7735_SetCursor(4,3);
	ST7735_OutString("Done");
} //end of main


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}

