// Super Langtons Ant cellular automata //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

#define LENGHT 32

  uint16_t state[WIDTH][HEIGHT];
  uint16_t antX = WIDTH/2;
  uint16_t antY = HEIGHT/2;
  uint16_t direction;
  uint16_t stateCount;
  bool type[LENGHT];
  uint8_t stateCols[LENGHT][3];
		

void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);
	
	antX = WIDTH/2;
	antY = HEIGHT/2;

	stateCount = 2 + rand()%(LENGHT-2);
	direction = rand()%4;

	for(int i = 0; i < stateCount; i++) type[i] = rand()%2;
	for(int i = 0; i < stateCount; i++) stateCols[i][0] = rand();
	for(int i = 0; i < stateCount; i++) stateCols[i][1] = rand();
	for(int i = 0; i < stateCount; i++) stateCols[i][2] = rand();

	for (int j=0; j < HEIGHT; j++){ for (int i = 0; i < WIDTH; i++) state[i][j] = 0; }

}


void turn(int angle){
  
  if(angle == 0){
    if(direction == 0){
      direction = 3;
    } else {
      direction--;
    }
  } else {
    if(direction == 3){
      direction = 0;
    } else {
      direction++;
    }
  }
}

void move(){
  
  if(antY == 0 && direction == 0){
    antY = HEIGHT-1;
  } else {
    if(direction == 0 ){
      antY--;
    }
  }
  if(antX == WIDTH-1 && direction == 1){
    antX = 0;
  } else {
    if(direction == 1){
      antX++;
    }
  }
  if(antY == HEIGHT-1 && direction == 2){
   antY = 0; 
  } else {
    if(direction == 2){
      antY++;
    }
  }
  if(antX == 0 && direction == 3){
    antX = WIDTH-1;
  } else {
    if(direction == 3){
      antX--;
    }
  }
}



int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Langtons Ant cellular automata\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random rule\n");
	printf(" \n");
	printf("START = Exit\n");

	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, false);

	rndrule();


		while (aptMainLoop()) {
		
			

			gspWaitForVBlank();
			
			hidScanInput();


			if (hidKeysDown() & KEY_START) break;
			if (hidKeysDown() & KEY_A) rndrule();

			
			for(int i = 0; i < 500; i++) {

				move();
				turn(type[(state[antX][antY]%stateCount)]);
				state[antX][antY]++;
				
				u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
				
				fb[3*(antY+antX*240)]   = stateCols[(state[antX][antY]%stateCount)][0];
				fb[3*(antY+antX*240)+1] = stateCols[(state[antX][antY]%stateCount)][1];
				fb[3*(antY+antX*240)+2] = stateCols[(state[antX][antY]%stateCount)][2];
    
			}

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}