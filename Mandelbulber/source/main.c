// Mandelbulber fractal //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <Math.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}


  int maxIterations = 2;
  float power = 5.0f;
  float xmin = -1.5f;
  float ymin = -1.5f;
  float zmin = -1.5f;
  float wh = 3;
  float toAdd = 0;
  float x, y;
  float R = 1.0f;
  float G = 1.0f;
  float B = 1.0f;
  int i = 0;
  
  
void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);
	
	i = 0;
	
	toAdd = wh/WIDTH;

	x = xmin;
	y = ymin;
  
    power = randomf(1.1f, 8.0f);
	R = randomf(0.1f, 1.0f);
	G = randomf(0.1f, 1.0f);
	B = randomf(0.1f, 1.0f);
	
} 


int main(int argc, char **argv) {

	gfxInitDefault();

	consoleInit(GFX_BOTTOM, NULL);

	printf("Mandelbulber fractal\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random power\n");
	printf(" \n");
	printf("START = Exit\n");
	
	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, false);
	
	toAdd = wh/WIDTH;

	x = xmin;


	while (aptMainLoop()) {

		gspWaitForVBlank();

		hidScanInput();

		if (hidKeysDown() & KEY_START) break;
		if (hidKeysDown() & KEY_A) rndrule();
		
		u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

		y = ymin;

		for (int j = 0; j < WIDTH; j++) {
		
			float z = zmin;
			for (int k = 255; k > 0 && z < -1*zmin; k--) {
			float dr = 1;
			float nx = x;
			float ny = y;
			float nz = z;
			float rad = sqrtf(x*x+y*y+z*z);
			int n = 0;
			
			while (n < maxIterations) {
				float powRad = powf(rad, power);
				float theta = atan2f(sqrtf(nx*nx+ny*ny), nz)*power;
				float phi = atan2f(ny, nx)*power;
				nx = sinf(theta)*cosf(phi)*powRad+x;
				ny = sinf(theta)*sinf(phi)*powRad+y;
				nz = cosf(theta)*powRad+z;
				dr = dr*power*powf(rad, power-1)+1;
				rad = sqrtf(nx*nx+ny*ny+nz*nz);
				if (rad > 4) break;
				n++;    
			}
			
			if (0.5f*logf(rad)*rad/dr < 0.00001f) {
  
				if (j > 0 && i < WIDTH && j > 0 && j < WIDTH){
				
					fb[3*((-80+j)+(i)*240)]   = k/R;
					fb[3*((-80+j)+(i)*240)+1] = k/G;
					fb[3*((-80+j)+(i)*240)+2] = k/B;

				} 
      
				break;
    
			} else {
          
				k--;
				z += 0.5f*logf(rad)*rad/dr;  
			}
		}
      
		y += toAdd;
		
	}	

	x += toAdd;
	
	if (i == WIDTH) i = 0; 
	i++;
	
	gfxFlushBuffers();
	gfxSwapBuffers();

	}

	gfxExit();

	return 0;

}