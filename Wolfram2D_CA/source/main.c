// Wolfram 2D cellular automata //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240
#define RULBIT 10
  
	uint8_t R, G, B;

	bool state[SCREEN];
	bool newst[SCREEN]; 
	bool rules[RULBIT] = {0,0,1,1,1,1,0,0,0,0};
	
	int i;
		

void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);
	
	memset(state, 0, SCREEN);
	memset(newst, 0, SCREEN);
	
	for(i = 0; i < RULBIT; i++) rules[i] = rand()%2;

	state[(WIDTH/2)+(HEIGHT/2)*WIDTH] = 1;

}


uint16_t neighbors(uint16_t x, uint16_t y){
  
  uint16_t result = 0;

  if(y > 0 && state[x+(y-1)*WIDTH] == 1) result = result + 1;
  if(x > 0 && state[(x-1)+y*WIDTH] == 1) result = result + 1;
  if(x < WIDTH-1 && state[(x+1)+y*WIDTH] == 1) result = result + 1;
  if(y < HEIGHT-1 && state[x+(y+1)*WIDTH] == 1) result = result + 1;
  
  return result;
 
}


int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Wolfram 2D cellular automata\n");
	printf(" \n");
	printf("Created by JLS 2022\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random rule\n");
	printf(" \n");
	printf("START = Exit\n");

	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, true);

	rndrule();

	while (aptMainLoop()) {

		gspWaitForVBlank();	
		hidScanInput();

		if (hidKeysDown() & KEY_START) break;
		if (hidKeysDown() & KEY_A) rndrule();
			
		for(int y = 0; y < HEIGHT; y++){
    
			for(int x = 0; x < WIDTH; x++){
           
				uint16_t totalNeighbors = neighbors(x,y);
            
				if(state[x+y*WIDTH] == 0 && totalNeighbors == 0) {newst[x+y*WIDTH] = rules[0]; R = 0; G = 0; B = 0;}
				else if(state[x+y*WIDTH] == 1 && totalNeighbors == 0) {newst[x+y*WIDTH] = rules[1]; R = 0; G = 255; B = 255;}
				else if(state[x+y*WIDTH] == 0 && totalNeighbors == 1) {newst[x+y*WIDTH] = rules[2]; R = 255; G = 0; B = 0;}
				else if(state[x+y*WIDTH] == 1 && totalNeighbors == 1) {newst[x+y*WIDTH] = rules[3]; R = 0; G = 255; B = 0;}
				else if(state[x+y*WIDTH] == 0 && totalNeighbors == 2) {newst[x+y*WIDTH] = rules[4]; R = 128; G = 0; B = 255;}
				else if(state[x+y*WIDTH] == 1 && totalNeighbors == 2) {newst[x+y*WIDTH] = rules[5]; R = 255; G = 0; B = 128;}
				else if(state[x+y*WIDTH] == 0 && totalNeighbors == 3) {newst[x+y*WIDTH] = rules[6]; R = 128; G = 255; B = 0;}
				else if(state[x+y*WIDTH] == 1 && totalNeighbors == 3) {newst[x+y*WIDTH] = rules[7]; R = 0; G = 128; B = 255;}
				else if(state[x+y*WIDTH] == 0 && totalNeighbors == 4) {newst[x+y*WIDTH] = rules[8]; R = 255; G = 255; B = 0;}
				else if(state[x+y*WIDTH] == 1 && totalNeighbors == 4) {newst[x+y*WIDTH] = rules[9]; R = 255; G = 255; B = 255;}
					 	  
				u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
				
				fb[3*(y+x*240)]   = R;
				fb[3*(y+x*240)+1] = G;
				fb[3*(y+x*240)+2] = B;
      
			}
		}
 
		memcpy(state, newst, SCREEN);

		gfxFlushBuffers();
		gfxSwapBuffers();

	}


	gfxExit();

	return 0;

}