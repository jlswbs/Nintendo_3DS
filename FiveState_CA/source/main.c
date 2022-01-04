// Five state 1D cellular automata //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

#define RND   11
#define VAL   15
#define COL   5
  
	uint8_t R, G, B;

	uint16_t coll;
	uint8_t parent[WIDTH]; 
	uint8_t child[WIDTH];
	int count;
	int a[VAL];
		

void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);

	for(int i = 0; i < VAL; i++) a[i] = rand()%RND;

	for(int i = 0; i < WIDTH; i++){
  
		parent[i] = rand()%COL;
		child[i] = 0;

	}

}

void rndseed(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);

	for(int i = 0; i < WIDTH; i++){
  
		parent[i] = rand()%COL;
		child[i] = 0;

	}

}


int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Five state 1D cellular automata\n");
	printf(" \n");
	printf("Created by JLS 2022\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random rule\n");
	printf(" \n");
	printf("Key B = Random seed\n");
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
			if (hidKeysDown() & KEY_B) rndseed();
			
			for (int y = 0; y < HEIGHT; y++) {
 
				for (int x = 0; x < WIDTH; x++) {
    
					if (x == 0) count = parent[WIDTH-1] + parent[0] + parent[1];
					else if (x == WIDTH-1) count = parent[WIDTH-2] + parent[WIDTH-1] + parent[0];
					else count = parent[x-1] + parent[x] + parent[x+1];        
            
					if(count == a[0] || count == a[1] || count == a[2]) child[x] = 0;
					if(count == a[3] || count == a[4] || count == a[5]) child[x] = 1;
					if(count == a[6] || count == a[7] || count == a[8]) child[x] = 2;
					if(count == a[9] || count == a[10] || count == a[11]) child[x] = 3;
					if(count == a[12] || count == a[13] || count == a[14]) child[x] = 4;
               
					if (child[x] == 0) { R = 255; G = 0; B = 0; }
					if (child[x] == 1) { R = 0; G = 255; B = 0; }
					if (child[x] == 2) { R = 0; G = 0; B = 255; }
					if (child[x] == 3) { R = 255; G = 255; B = 0; }
					if (child[x] == 4) { R = 255; G = 255; B = 255; }

					u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
				
					fb[3*(-y+x*240)]   = R;
					fb[3*(-y+x*240)+1] = G;
					fb[3*(-y+x*240)+2] = B;
                                                   
				}
                                                 
				memcpy(parent, child, WIDTH);
			}

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}