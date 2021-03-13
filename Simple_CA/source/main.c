// Simple 1D cellular automata //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

#define ITER 16

	int x,y,d,l;

	bool k;

	bool state[WIDTH];


int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Simple 1D cellular automata\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("START = Exit\n");

	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, false);
	
	state[WIDTH/2] = 1;


	while (aptMainLoop()) {	
	
		l = ITER;
			
		while(l--) gspWaitForVBlank();
			
		hidScanInput();

		if (hidKeysDown() & KEY_START) break;

		u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

		for (y=0;y<HEIGHT;y++) {

			for (x=0;x<WIDTH;x++) {

				k = k^state[x];
				state[x] = k;

				if (k == true)  d = 255;
				else d = 0;

				fb[3*(y+x*240)]   = d;
				fb[3*(y+x*240)+1] = d;
				fb[3*(y+x*240)+2] = d;

			}
			
		}

		gfxFlushBuffers();
		gfxSwapBuffers();

	}

	gfxExit();

	return 0;

}