#include "stdio.h"
#include "stdlib.h"
#include "LPC17xx.h"
#include "KBD.h"
#include "GLCD.h"

#define DELAY       20
#define __FI        1   /* Font index 16x24 */

int positionX; 
int positionY; 
int size; 
int direct = 0; 
int directLast = 0; 
int joystickValue = 0; 
int joystickLast = KBD_RIGHT; 
char str[20],str1[20],str2[20],str3[20];
int snake[100][2];
int delx, dely; 
int speed; 
int foodX, foodY;
int collision = 0;
int score = 0;

void food(){
	int i;
	foodX = 1 + rand()%7;
	foodY = 1 + rand()%18;
	for(i=0;i<size;i++){
		if(foodX == snake[i][0])
			if(foodY == snake[i][1])
				food();
	}
	GLCD_DisplayChar(foodX,foodY,__FI,0x81);
}

void delay (int count){
	count <<= DELAY;
	while(count--);
}

void setbody(){
	int i; 
	
	for(i=0;i<size;i++){
		switch(direct){
			case 0:
				snake[i][0] = positionX;
				snake[i][1] = positionY-i;
				break;
			case 1:
				snake[i][0] = positionX;
				snake[i][1] = positionY+i;
				break;
			case 2:
				snake[i][0] = positionX+i;
				snake[i][1] = positionY;
				break;
			case 3:
				snake[i][0] = positionX-1;
				snake[i][1] = positionY;
				break;
		}
	}
}

void addbody(){
	int n=1;
	size++;
	score = score + n;
	if(speed != 0)
		speed--;
	n++;
}

void check(){
	int i;
	
	if(foodX == snake[0][0])
		if(foodY == snake[0][1]){
			addbody();
			food();
		}
		
	for(i=1;i<size;i++){
		if(snake[0][0] == snake[i][0])
			if(snake[0][1] == snake[i][1])
				collision = 1;
	}

		if(snake[0][1] == 19 && snake[1][1] == 18)
			collision = 1;

		if(snake[0][1] == 0 && snake[1][1] == 1)
			collision = 1;

		if(snake[0][0] == 9 && snake[1][0] == 8)
			collision = 1;

		if(snake[0][0] == 0 && snake[1][0] == 1)
			collision = 1;
	
}

void updatebody(){
	int i;
	if(direct == 0){
		for(i=size;i>0;i--){
			if(i -1 == 0){
				snake[0][1] = positionY;
				snake[0][0] = positionX;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],__FI,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}		
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],__FI,0x8B);
			GLCD_DisplayChar(snake[i][0],snake[i][1],__FI,0x82);
		}
		delay(speed);
	}else if(direct == 1){
		for(i=size;i>0;i--){
			if(i -1 == 0){
				snake[0][1] = positionY;
				snake[0][0] = positionX;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],__FI,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],__FI,0x89);
			GLCD_DisplayChar(snake[i][0],snake[i][1],__FI,0x82);
		}
		delay(speed);
	}else if(direct == 2){
		for(i=size;i>0;i--){
			if(i -1 == 0){
				snake[0][1] = positionY;
				snake[0][0] = positionX;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],__FI,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],__FI,0x87);
			GLCD_DisplayChar(snake[i][0],snake[i][1],__FI,0x82);
		}
		delay(speed);
	}else if(direct == 3){
		for(i=size;i>0;i--){
			if(i-1 == 0){
				snake[0][1] = positionY;
				snake[0][0] = positionX;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],__FI,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],__FI,0x85);
			GLCD_DisplayChar(snake[i][0],snake[i][1],__FI,0x82);
		}
		delay(speed);
	}
	check();
}

void direction(int joyval){

	switch(joyval){
		case KBD_UP:
			if (joystickLast == KBD_LEFT || joystickLast == KBD_RIGHT){
				positionX--;
				if (positionX < 0){
					positionX = 9;
				}
				direct = 3;
				directLast = direct;
				joystickLast = joystickValue;
				updatebody();
			}
			break;
		case KBD_DOWN:
			if (joystickLast == KBD_LEFT || joystickLast == KBD_RIGHT){
				positionX++;
				if (positionX > 9){
					positionX = 0;
				}
				direct = 2;
				directLast = direct;
				joystickLast = joystickValue;
				updatebody();
			}
			break;
		case KBD_LEFT:
			if (joystickLast == KBD_UP || joystickLast == KBD_DOWN){
				positionY--;
				if (positionY < 0){
					positionY = 20;
				}
				direct = 1;
				directLast = direct;
				joystickLast = joystickValue;
				updatebody();
			}
			break;
		case KBD_RIGHT:
			if (joystickLast == KBD_UP || joystickLast == KBD_DOWN){
				positionY++;
				if (positionY > 20 ){
					positionY = 0;
				}
				direct = 0;
				directLast = direct;
				joystickLast = joystickValue;
				updatebody();
			}
			break;
		default:
			switch(direct){
				case 0:
					positionY++;
					if (positionY > 20){
						positionY = 0;
					}
					updatebody();
					check();
					break;
				case 1:
					positionY--;
					if (positionY < 0){
						positionY = 20;
					}
					updatebody();
					check();
					break;
				case 2:
					positionX++;
					if (positionX > 9){
						positionX = 0;
					}
					updatebody();
					check();
					break;
				case 3:
					positionX--;
					if (positionX < 0){
						positionX = 9;
					}
					updatebody();
					check();
					break;
			}
			break;
	}	
}

void clearsnake(){
	int i;
	for(i=0;i<size;i++){
		snake[i][0]=1;
		snake[i][1]=1;
	}
}

int snakegame(){
	int joy_difficulty, joy_try;
	int mode, modesel;
	int highscore=0;
	int gameover, tryagain;
	char scores[54];
	char highscores[54];
	int done=0;
	
	GLCD_Init();
	KBD_Init();


	while(!done){
		modesel = 1;
		mode = 1;
		tryagain = 1;
		direct = 0;
		directLast = 0;
		joystickValue = 0;
		joystickLast = KBD_RIGHT;
		speed = 10;
		size = 2;
		positionX = 5;
		positionY = 10;
		
		GLCD_Clear(Blue);
		GLCD_SetBackColor(Blue);
		GLCD_SetTextColor(Green);
		setbody();
		food();	
		while(collision == 0){
			joystickValue = get_button();
			direction(joystickValue);
			sprintf(str,"     score:[%d]",score);
			GLCD_DisplayString(0,0,__FI,(unsigned char *)str);	
		}
		if(collision == 1){
			GLCD_Clear(Black);
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Red);
			if(score>=highscore)
					highscore = score;
			
			GLCD_DisplayString(2,0,__FI,"     GAME OVER     ");
			
			sprintf(scores,"SCORE: %d",score);
			GLCD_DisplayString(4,0,__FI,(unsigned char *)scores);
			sprintf(highscores,"HIGH SCORE: %d ",highscore);
			GLCD_DisplayString(5,0,__FI,(unsigned char *)highscores);
			
			GLCD_DisplayString(7,0,__FI,"     try again?    ");
			GLCD_DisplayString(8,0,__FI,"        YES      ");
			score = 0;
			gameover = 0;
			while(gameover == 0){
				joy_try = get_button();
				switch(joy_try){
					case KBD_DOWN:
						GLCD_DisplayString(8,0,__FI,"        NO       ");

						tryagain = 0;
						break;
					case KBD_UP:
						GLCD_DisplayString(8,0,__FI,"        YES      ");

						tryagain = 1;
						break;
					case KBD_SELECT:
						if(tryagain == 0){
							GLCD_Clear(White);
							done =1;
							return 0;
						}
						if(tryagain == 1){
							gameover = 1;
							collision = 0;
							clearsnake();
						}
						break;
				}
			}
		}
		delay(5);
	}
	return 0;
}