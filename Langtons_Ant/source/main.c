// // Langtons Ant cellular automata //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

#define NUMANTS 6
#define ITER 64
  
  uint8_t coll[NUMANTS];
  
  int x[NUMANTS];
  int y[NUMANTS];
  int antsdir[NUMANTS];
		

void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);
	
	for(int i = 0; i < NUMANTS; i++){
  
    x[i] = rand()%WIDTH;
    y[i] = rand()%HEIGHT;
    antsdir[i] = rand()%4;
    coll[i] = 1 + rand();
    
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
			
			u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

			
			for(int k = 0; k < ITER; k++){
  
				for(int i = 0; i < NUMANTS; i++){
				
					if (fb[3*(y[i]+x[i]*240)] && fb[3*(y[i]+x[i]*240)+1] && fb[3*(y[i]+x[i]*240)+2] > 0) {
					
						antsdir[i] = antsdir[i] - 1;
						fb[3*(y[i]+x[i]*240)]   = 0;
						fb[3*(y[i]+x[i]*240)+1] = 0;
						fb[3*(y[i]+x[i]*240)+2] = 0;
						
					} else {
					
						antsdir[i] = antsdir[i] + 1;
						fb[3*(y[i]+x[i]*240)]   = coll[i];
						fb[3*(y[i]+x[i]*240)+1] = coll[i]<<1;
						fb[3*(y[i]+x[i]*240)+2] = coll[i]<<2;
							
					}
					
					if (antsdir[i] > 3) antsdir[i] = 0;   
					if (antsdir[i] < 0) antsdir[i] = 3;   
    
					if (antsdir[i] == 0) x[i] = x[i] - 1;
					if (antsdir[i] == 1) y[i] = y[i] + 1;
					if (antsdir[i] == 2) x[i] = x[i] + 1;
					if (antsdir[i] == 3) y[i] = y[i] - 1;
    
					if (x[i] > WIDTH-1) x[i] = 0;
					if (x[i] < 0) x[i] = WIDTH-1;
					if (y[i] > HEIGHT-1) y[i] = 0;
					if (y[i] < 0) y[i] = HEIGHT-1;
    
				}
			}

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}