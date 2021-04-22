#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//84x48
#define TEST 0 //1 - disables losing in the game 

int food = 250;
int fall_speed; //affected by settings bellow; 1 was default
int hunger; //afected by settings bellow; 1 was default

int basket_X = 38;
int basket_W = 8; //width
const int basket_Y = LCDHEIGHT - 3;
const int basket_H = 3;

int eggs_number; //affected by settings bellow; 8 was default
int *eggs_X;
int *eggs_Y;
const int EGG_W = 2;
const int EGG_H = 3;

bool first = true;
bool again = false;

void setup(){
	gb.begin();
	gb.titleScreen(F("Eggs"));
}

void loop(){
	while(gb.update()){ //returns true every 50ms. 20fps
	
		//SETTINGS
		if(first){
			if (again){
				gb.display.println("GAME OVER");
				gb.display.println("Do you want to play\nagain?\n");
			}
			gb.display.println(F("Select level:"));
			gb.display.println(F("  LEFT - easy"));
			gb.display.println(F("  UP - normal"));
			gb.display.print(F("  RIGHT - hard"));

			if(gb.buttons.repeat(BTN_LEFT, 0)){
				fall_speed = 1;
				hunger = 1;
				eggs_number = 9;
				first = false;
			}else if(gb.buttons.repeat(BTN_UP, 0)){
				fall_speed = 1;
				hunger = 2;
				eggs_number = 8;
				first = false;
			}else if(gb.buttons.repeat(BTN_RIGHT, 0)){
				fall_speed = 2;
				hunger = 3;
				eggs_number = 6;
				first = false;
			}

			if (!first){ //create table with eggs
				eggs_X = new int[eggs_number];
				eggs_Y = new int[eggs_number];

				for (int i = 0; i < eggs_number; i++){
					eggs_X[i] = random(1, LCDWIDTH);
					eggs_Y[i] = random(-40, -EGG_H);
				}
			}
		}	
		
		if(!first){
			//INPUT
			if (gb.buttons.repeat(BTN_LEFT, 0) && basket_X > 0){
				basket_X -= 2;
			} else if (gb.buttons.repeat(BTN_RIGHT, 0) && basket_X < LCDWIDTH - basket_W){
				basket_X += 2;
			}
			
			//LOGIC
			food -= hunger;
			food < 40 ? basket_W = 2 : basket_W = food / 20;
			
			for (int i = 0; i < eggs_number; i++){
				eggs_Y[i] += fall_speed; //move eggs down

				if (eggs_X[i] >= basket_X - 1 && eggs_X[i] <= basket_X + basket_W + 1 && eggs_Y[i] >= LCDHEIGHT - basket_H){ //collision with basket
					food += 30;
					eggs_X[i] = random(1, LCDWIDTH);
					eggs_Y[i] = random(-40, -EGG_H);
				}else if (eggs_X[i] < basket_X - 1 && eggs_X[i] > basket_X + basket_W + 1 && eggs_Y[i] >= LCDHEIGHT){ //egg fell
					eggs_X[i] = random(1, LCDWIDTH);
					eggs_Y[i] = random(-40, -EGG_H);
				}else if (eggs_Y[i] >= LCDHEIGHT+2){
					eggs_X[i] = random(1, LCDWIDTH);
					eggs_Y[i] = random(-40, -EGG_H);
				}
			}

			if(gb.buttons.repeat(BTN_C, 0)) gb.titleScreen(F("Eggs"));

			//DRAW
			gb.display.print("Food:");
			gb.display.println(food);
			#if (TEST)
				gb.display.print(basket_W);
			#endif
			gb.display.fillRect(basket_X, basket_Y, basket_W, basket_H);

			for (int i = 0; i < eggs_number; i++){
				gb.display.fillRect(eggs_X[i], eggs_Y[i], EGG_W, EGG_H);
			}

			#if (!TEST)	//GAME OVER
				if (food <= 0){ //if game ends
					food = 250;
					first = true;
					again = true;

					delete eggs_X;
					eggs_X = NULL;
					delete eggs_Y;
					eggs_Y = NULL;

					delay(3000);
				}
			#endif
			
			//gb.display.clear();
		}
	}
}