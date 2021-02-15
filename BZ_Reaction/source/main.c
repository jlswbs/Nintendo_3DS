// Belousov-Zhabotinsky Reaction //


#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <math.h>


#define WIDTH  200
#define HEIGHT 120
#define SCR (400 * 240)

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

	float a [WIDTH][HEIGHT][2];
	float b [WIDTH][HEIGHT][2];
	float c [WIDTH][HEIGHT][2];
	
	bool invert = false;
	bool color = false;
	
	float adjust = 1.2f;

	int p = 0, q = 1;
	int x, y, i, j;



void rndrule() {

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	
	memset(fb, 0, SCR * 3);

	adjust = randomf(0.75f, 1.35f);

	for (y = 0; y < HEIGHT; y++) {
	
		for (x = 0; x < WIDTH; x++) {
		
			a[x][y][p] = randomf(0.0f, 1.0f);
			b[x][y][p] = randomf(0.0f, 1.0f);
			c[x][y][p] = randomf(0.0f, 1.0f);
		}
	}
}


int main(int argc, char **argv) {


	gfxInitDefault();

	consoleInit(GFX_BOTTOM, NULL);

	printf("Belousov-Zhabotinsky reaction\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random rule\n");
	printf("Key B = Invert\n");
	printf("Key Y = Color\n");
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
		if (hidKeysDown() & KEY_B) invert = !invert;
		if (hidKeysDown() & KEY_Y) color = !color;


		for (y = 0; y < HEIGHT; y++) {

			for (x = 0; x < WIDTH; x++) {
	
				float c_a = 0.0f;
				float c_b = 0.0f;
				float c_c = 0.0f;
	  
     			for (i = x - 1; i <= x+1; i++) {

        			for (j = y - 1; j <= y+1; j++) {

          				c_a += a[(i+WIDTH)%WIDTH][(j+HEIGHT)%HEIGHT][p];
          				c_b += b[(i+WIDTH)%WIDTH][(j+HEIGHT)%HEIGHT][p];
          				c_c += c[(i+WIDTH)%WIDTH][(j+HEIGHT)%HEIGHT][p];
						
        			}
      			}
      
	  			c_a /= 9.0f;
      			c_b /= 9.0f;
      			c_c /= 9.0f;

      			a[x][y][q] = constrain(c_a + c_a * (adjust * c_b - c_c), 0.0f, 1.0f);
				b[x][y][q] = constrain(c_b + c_b * (c_c - adjust * c_a), 0.0f, 1.0f);
				c[x][y][q] = constrain(c_c + c_c * (c_a - c_b), 0.0f, 1.0f);
	  
	  			uint8_t l = invert ? 255-(255*a[x][y][q]) : (255*a[x][y][q]);
				
				u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

				if (color == 0) {

					fb[3*((2*y)+(2*x)*240)]   = l;
					fb[3*((2*y)+(2*x)*240)+1] = l;
					fb[3*((2*y)+(2*x)*240)+2] = l;
				
				}
				
				else {
				
					fb[3*((2*y)+(2*x)*240)]   = l;
					fb[3*((2*y)+(2*x)*240)+1] = l<<1;
					fb[3*((2*y)+(2*x)*240)+2] = l<<2;
				
				}
	  
			}
		}

	
		if (p == 0) { p = 1; q = 0; } else { p = 0; q = 1; }

		gfxFlushBuffers();
		gfxSwapBuffers();

	}


	gfxExit();

	return 0;

}