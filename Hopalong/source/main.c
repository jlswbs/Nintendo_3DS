// Hopalong orbit fractal //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <math.h>


#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}


	float a,b,c,x,y,t;

	char col;

	int xx,yy,i,r;


void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memset(fb, 0, 3*SCREEN);

	x = 0.1f;
	y = 0.1f;
  
	float logpmax = logf(WIDTH/4);
	  
	a = expf(randomf(0.0f, 1.0f)*logpmax);
	b = expf(randomf(0.0f, 1.0f)*logpmax);
	c = expf(randomf(0.0f, 1.0f)*logpmax);

}


int main(int argc, char **argv) {


	gfxInitDefault();

	consoleInit(GFX_BOTTOM, NULL);

	printf("Hopalong orbit fractal\n");
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
	
		col = rand();
	
		gspWaitForVBlank();
		
		hidScanInput();
		
		if (hidKeysDown() & KEY_START) break;
		if (hidKeysDown() & KEY_A) rndrule();
		
		for (int i=0;i<10000;i++) {

			float nx = x;
			float ny = y;

			t = sqrtf(fabs(b*nx-c));
			x = ny - ((nx<0) ? t : -t);
			y = a - nx;

			int ax = (WIDTH/2) + x/2;
			int ay = (HEIGHT/2) + y/2;
      
			if (ax>0 && ax<WIDTH && ay>0 && ay <HEIGHT) {
			
			u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

			
				fb[3*((ay)+(ax)*240)]   = col;
				fb[3*((ay)+(ax)*240)+1] = col<<1;
				fb[3*((ay)+(ax)*240)+2] = col<<2;
				
			}

		}

		gfxFlushBuffers();
		gfxSwapBuffers();	
		
	}

	gfxExit();

	return 0;

}