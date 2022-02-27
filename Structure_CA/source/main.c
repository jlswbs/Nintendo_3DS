// Structure 1D cellular automata //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

  
	uint8_t R, G, B;
	uint8_t state[WIDTH];
	uint8_t temp[WIDTH];
	uint8_t k;
		

void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);

	for (int i = 0; i < WIDTH; i++) state[i] = rand()%16;
	for (int i = 0; i < WIDTH; i++) temp[i] = 0;
	k = rand()%16;

}


int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Structure 1D cellular automata\n");
	printf(" \n");
	printf("Created by JLS 2022\n");
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
			
			for (int y = 0; y < HEIGHT; y++) {
 
				for (int x = 0; x < WIDTH; x++) {
    
					k = k ^ temp[x] ^ state[k];
					state[x] = k;
               
					if (state[x] == 0) { R = 0; G = 0; B = 0; }
					if (state[x] == 1) { R = 0; G = 255; B = 0; }
					if (state[x] == 2) { R = 0; G = 0; B = 255; }
					if (state[x] == 3) { R = 255; G = 255; B = 0; }
					if (state[x] == 4) { R = 128; G = 128; B = 255; }
					if (state[x] == 5) { R = 255; G = 0; B = 0; }
					if (state[x] == 6) { R = 0; G = 255; B = 128; }
					if (state[x] == 7) { R = 128; G = 0; B = 255; }
					if (state[x] == 8) { R = 255; G = 255; B = 0; }
					if (state[x] == 9) { R = 255; G = 0; B = 128; }
					if (state[x] == 10) { R = 255; G = 128; B = 0; }
					if (state[x] == 11) { R = 128; G = 255; B = 0; }
					if (state[x] == 12) { R = 0; G = 0; B = 255; }
					if (state[x] == 13) { R = 128; G = 255; B = 0; }
					if (state[x] == 14) { R = 0; G = 128; B = 128; }
					if (state[x] == 15) { R = 255; G = 255; B = 255; }

					u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
				
					fb[3*(-y+x*240)]   = R;
					fb[3*(-y+x*240)+1] = G;
					fb[3*(-y+x*240)+2] = B;
                                                   
				}
                                                 
				memcpy(temp, state, WIDTH);
				
			}

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}