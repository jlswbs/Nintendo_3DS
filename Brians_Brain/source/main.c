// BriansBrain cellular automata //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  200
#define HEIGHT 120
#define SCREEN 400*240

#define DENSITY     7
#define READY       0
#define REFRACTORY  1
#define FIRING      2
  
	uint8_t R, G, B;
	uint8_t world[WIDTH][HEIGHT];
	uint8_t temp[WIDTH][HEIGHT];

	
uint8_t weighted_randint(int true_weight){
  
    int choice = rand() % 10;
    
    if (choice > true_weight) return 1;
    else return 0;
	
}

void rndrule() {
  
	int r;
  
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++){
			r = weighted_randint(DENSITY);
			if (r == 1) world[x][y] = FIRING;
			else world[x][y] = READY;
		}
	}

}



void symrule() {

	uint8_t k = 1 + rand() % (HEIGHT/2);
	
	memset(world, 0, sizeof(world));

	for (int y = (HEIGHT/2)-k; y < (HEIGHT/2)+k; y+=2) {
	
		for (int x = (WIDTH/2)-k; x < (WIDTH/2)+k; x+=2) world[x][y] = FIRING;

	}

}




uint8_t count_neighbours(uint8_t world[WIDTH][HEIGHT], int x_pos, int y_pos){
  
    int cx, cy, cell;
    int count = 0;

    for (int y = -1; y < 2; y++) {
        for (int x = -1; x < 2; x++) {
            cx = x_pos + x;
            cy = y_pos + y;
            if ( (0 <= cx && cx < WIDTH) && (0 <= cy && cy < HEIGHT)) {
                cell = world[x_pos + x][y_pos + y];
                if (cell == FIRING) count ++;
            }
        }
    }
	
	return count;
  
}


void apply_rules(uint8_t world[WIDTH][HEIGHT]){
  
	int cell, neighbours;

	memcpy(temp, world, sizeof(temp));

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++){
			cell = temp[x][y];          
			if (cell == READY) {
				neighbours = count_neighbours(temp, x, y);
				if (neighbours == 2) world[x][y] = FIRING; }
			else if (cell == FIRING) world[x][y] = REFRACTORY;
			else world[x][y] = READY;
		}
	}
	
}



void draw_world(uint8_t world[WIDTH][HEIGHT]){
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++){
            if (world[x][y] == FIRING) {R = 255; G = 0; B =0;}  
            else if (world[x][y] == REFRACTORY) {R = 255; G = 255; B = 255;}
            else {R = 0; G = 0; B = 0;}
			
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

	printf("BriansBrain cellular automata\n");
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
			
			apply_rules(world);
			draw_world(world);

			gfxFlushBuffers();
			gfxSwapBuffers();

		}


	gfxExit();

	return 0;

}