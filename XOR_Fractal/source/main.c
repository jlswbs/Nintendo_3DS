// Simple XOR fractal //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240



	int x,y,d;

	char k;



int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Simple XOR fractal\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("START = Exit\n");

	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, false);


	while (aptMainLoop()) {	

		gspWaitForVBlank();
			
		hidScanInput();

		if (hidKeysDown() & KEY_START) break;

		u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);


		for (y=0;y<HEIGHT;y++) {

			for (x=0;x<WIDTH;x++) {

				k = (((x+d)^y) % (x^(y+d))) + d;

				fb[3*(y+x*240)]   = k;
				fb[3*(y+x*240)+1] = k<<1;
				fb[3*(y+x*240)+2] = k<<2;

			}

		}

		d++;

		gfxFlushBuffers();
		gfxSwapBuffers();

		}

	gfxExit();

	return 0;

}