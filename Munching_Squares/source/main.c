// Simple Munching Squares //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

	int x,y,t;
	char k;


int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Simple Munching Squares\n");
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

				k = ((x+t)^(y+t))+t;

				fb[3*(y+x*240)]   = k;
				fb[3*(y+x*240)+1] = k;
				fb[3*(y+x*240)+2] = k;

			}

		}

		t++;

		gfxFlushBuffers();
		gfxSwapBuffers();

	}

	gfxExit();

	return 0;

}