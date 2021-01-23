// Turmites cellular automata //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240
  
	uint8_t R, G, B;

	int posx,posy;
	int oldposx, oldposy;
	int state;
	int dir;
	int world[WIDTH][HEIGHT];
	int last_filled;
	int current_col;
	int next_col[4][4];
	int next_state[4][4];
	int directions[4][4];
		

void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);

	state = rand()%4;
	dir = 0;
	posx = WIDTH/2;
	posy = HEIGHT/2;
  
	for(int j=0; j<4; j++){   
		for(int i=0; i<4; i++){         
		next_col[i][j] = rand()%4;
		next_state[i][j] = rand()%4;
		directions[i][j] = rand()%8;
		}   
	}

	for (int j=0; j < HEIGHT; j++){ for (int i = 0; i < WIDTH; i++) world[i][j] = 0; }

	world[posx][posy] = rand()%4;

}

void move_turmite(){
  
  int cols = world[posx][posy];
  
  oldposx = posx;
  oldposy = posy;
  current_col = next_col[cols][state];
  world[posx][posy] = next_col[cols][state];
  state = next_state[cols][state];    

  dir = (dir + directions[cols][state]) % 8;

  switch(dir){
    case 0: posy--; break;
    case 1: posy--; posx++; break;
    case 2: posx++; break;
    case 3: posx++; posy++; break;
    case 4: posy++; break;
    case 5: posy++; posx--; break;
    case 6: posx--; break;
    case 7: posx--; posy--; break;
  }

  if(posy < 0) posy = HEIGHT-1;
  if(posy >= HEIGHT) posy = 0;
  if(posx < 0) posx = WIDTH-1;
  if(posx >= WIDTH) posx=0;
  
}


int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Turmites cellular automata\n");
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

			
			for(int i=0; i<100; i++){
    
				move_turmite();
    
				switch(current_col){
					case 0: R = 255; G = 0; B = 0; break;
					case 1: R = 0; G = 255; B = 0; break;
					case 2: R = 0; G = 0; B = 255; break;
					case 3: R = 255; G = 255; B = 255; break;
				}
				
				u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
				
				fb[3*(oldposy+oldposx*240)]   = R;
				fb[3*(oldposy+oldposx*240)+1] = G;
				fb[3*(oldposy+oldposx*240)+2] = B;
    
			}

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}