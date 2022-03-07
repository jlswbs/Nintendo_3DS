// Conway's Game of Life //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  200
#define HEIGHT 120
#define SCREEN 400*240

  
	uint8_t R, G, B;
	bool grid[2][WIDTH][HEIGHT];
	int current = 0;


void rndrule() {

	current = 0;
  
	for (int y = 0; y < HEIGHT; y++) {
	
		for (int x = 0; x < WIDTH; x++) {
		
			grid[0][x][y] = rand()%2;
			grid[1][x][y] = 0;
			
		}

	}

}



void symrule() {

	current = 0;

	uint8_t k = 1 + rand() % (HEIGHT/2);
	
	for (int y = 0; y < HEIGHT; y++) {
		
		for (int x = 0; x < WIDTH; x++) {
		
			grid[0][x][y] = 0;
			grid[1][x][y] = 0;
			
		}

	}

	for (int y = (HEIGHT/2)-k; y < (HEIGHT/2)+k; y++) {
	
		for (int x = (WIDTH/2)-k; x < (WIDTH/2)+k; x++) grid[0][x][y] = true;

	}

}


int neighbours(int x, int y){

	int result = 0;

	x--;
	y--;
  
	for (int i = 0; i < 3; i++) {
  
		if (y < 0 || y > (HEIGHT - 1)) continue;

		for (int j = 0; j < 3; j++) {
			if (x < 0 || x > (WIDTH - 1)) continue;
			if (i==1 && j == 1) { x++; continue; }
			if (grid[current][x][y]) result++;
			x++;
		}
		y++;
		x -= 3;
	}
  
	return result;
	
}


void rungrid(){
  
	int count;
	int value = 0;
	int new_grid;

	new_grid = 1 - current;
  
	for (int y = 0; y < HEIGHT; y++) {
  
		for (int x = 0; x < WIDTH; x++) {
      
			count = neighbours(x, y);
      
			if (count < 2 || count > 3) { value = 0; }
			else if (count == 3) { value = 3; }
			else { value = grid[current][x][y]; }
    
			grid[new_grid][x][y] = value;
    
		}
	}
  
	current = new_grid;
	
}


int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Conway's Game of Life\n");
	printf(" \n");
	printf("Created by JLS 2022\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random rule\n");
	printf("Key B = Symmetry rule\n");
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
			if (hidKeysDown() & KEY_B) symrule();
			
			rungrid();
			
			for (int y = 0; y < HEIGHT; y++) {
    
				for (int x = 0; x < WIDTH; x++) {
				
					if(grid[current][x][y]) {R = 255; G = 255; B = 255;}
					else {R = 0; G = 0; B = 0;}
					
					u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
					
					fb[3*((2*y)+(2*x)*240)]   = R;
					fb[3*((2*y)+(2*x)*240)+1] = G;
					fb[3*((2*y)+(2*x)*240)+2] = B;
		
				}
			}

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}