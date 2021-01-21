// StarWars cellular automata //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  200
#define HEIGHT 120
#define SCREEN 400*240

#define min(a,b) ((a)<(b)?(a):(b))

	int current [WIDTH][HEIGHT];
	int next [WIDTH][HEIGHT];
	int alive_counts [WIDTH][HEIGHT];
	int tmp[WIDTH][HEIGHT];
  
	uint8_t R, G, B;

	int ALIVE = 3;
	int DEATH_1 = 2;
	int DEATH_2 = 1;
	int DEAD = 0;


	int x,y,nx,ny;
	


void rndrule() {

	memset(alive_counts, 0, sizeof(alive_counts));

	for (y = 0; y < HEIGHT; y++) { for (x = 0; x < WIDTH; x++) current[x][y] = (rand() % 100) < 20 ? ALIVE : DEAD; }

}



void symrule() {

	uint8_t k = 1 + rand() % (HEIGHT/2);


	memset(alive_counts, 0, sizeof(alive_counts));
	memset(current, 0, sizeof(current));

	for (y = (HEIGHT/2)-k; y < (HEIGHT/2)+k; y+=2) { for (x = (WIDTH/2)-k; x < (WIDTH/2)+k; x+=2) current[x][y] = ALIVE; }

}

  

void step() {
 
    for (x = 0; x < WIDTH; x++) {
	
		for (y = 0; y < HEIGHT; y++) {
		
		int count = 0;
		int next_val;
    
		int mx = WIDTH-1;
		int my = HEIGHT-1;
		
        int self = current[x][y];
    
    for (nx = x-1; nx <= x+1; nx++) {
	
		for (ny = y-1; ny <= y+1; ny++) {
		
			if (nx == x && ny == y) continue;
			
			if (current[wrap(nx, mx)][wrap(ny, my)] == ALIVE) count++;
			
		}   
    }  

    int neighbors = count;
    
    if (self == ALIVE) next_val = ((neighbors == 3) || (neighbors == 4) || (neighbors == 5)) ? ALIVE : DEATH_1;
	
		else if (self == DEAD) next_val = (neighbors == 2) ? ALIVE : DEAD;
	
		else next_val = self-1;
   
		next[x][y] = next_val;
	
        if (next_val == ALIVE) alive_counts[x][y] = min(alive_counts[x][y]+1, 100);
		else if (next_val == DEAD) alive_counts[x][y] = 0;
	  
		}
    }
    
    for (x = 0; x < WIDTH; x++) {
	
		for (y = 0; y < HEIGHT; y++) {
        
			tmp[x][y] = current[x][y];
			current[x][y] = next[x][y];
			next[x][y] = tmp[x][y];
    
		}
    }
	
  }
  

  
int wrap(int v, int m) {

    if (v < 0) return v + m;

		else if (v >= m) return v - m;

		else return v;
  }
  
  
void draw_type(int min_alive, int max_alive, uint8_t r, uint8_t g, uint8_t b){  
    
    for (x = 0; x < WIDTH; x++) {
	
      
		for (y = 0; y < HEIGHT; y++) {
		
			int self = current[x][y];
			if (self == DEAD) continue;
   
			int alive = alive_counts[x][y];
			if (alive < min_alive || alive > max_alive) continue;
        
			if (self == ALIVE) { R = r; G = g; B = b; }
			else if (self == DEATH_1) { R = r>>1; G = g>>1; B = b>>1; }
			else if (self == DEATH_2) { R = r>>2; G = g>>2; B = b>>2; }
			
			u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		
			fb[3*((2*y)+(2*x)*240)]   = R;
			fb[3*((2*y)+(2*x)*240)+1] = G;
			fb[3*((2*y)+(2*x)*240)+2] = B;

        
		}
    }
 
  }



int main(int argc, char **argv) {
	
	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("StarWars cellular automata\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
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

			step();
			
			u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
			
			memset(fb, 0, SCREEN * 3);
			
			draw_type(50, 100, 100, 100, 255);
			draw_type(2, 49, 255, 100, 100);
			draw_type(0, 1, 255, 255, 255);

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}