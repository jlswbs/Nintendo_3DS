// Mandelbrot fractal zoomer //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240


	float x = -0.086f;
	float y = 0.85f;
	float zoom = 1.0f;


	void Mandelbrot(float x1, float y1, float x2, float y2);


int main(int argc, char **argv) {
	
	gfxInitDefault();

	consoleInit(GFX_BOTTOM, NULL);

	printf("Mandelbrot fractal zoomer\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("START = Exit\n");
	
	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, true);



		while (aptMainLoop()) {


			gspWaitForVBlank();
			
			hidScanInput();

			if (hidKeysDown() & KEY_START) break;

			Mandelbrot(-2.0f*zoom+x, -1.5f*(0.8*zoom)+y, 2.0f*zoom+x, 1.5f*(0.8*zoom)+y);

			zoom *= 0.7f;

			if(zoom <= 0.000003f) zoom = 1.0f;

			gfxFlushBuffers();
			gfxSwapBuffers();


		}


	gfxExit();

	return 0;

}



void Mandelbrot(float x1, float y1, float x2, float y2) {

	unsigned int i,j;
	uint16_t iter;

	float sy = y2 - y1;
	float sx = x2 - x1;
	const int MAX = 512;


	for(i = 0; i < WIDTH; i++) {
		for(j = 0; j < HEIGHT; j++) {
			float cy = j * sy / HEIGHT + y1;
			float cx = i * sx / WIDTH + x1;
			float x=0.0f, y=0.0f, xx=0.0f, yy=0.0f;
			for(iter=0; iter <= MAX && xx+yy<4.0f; iter++) {
				xx = x*x;
				yy = y*y;
				y = 2.0f*x*y + cy;
				x = xx - yy + cx;
			}
			
			u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

			fb[3*((j)+(i)*240)]   = iter << 1;
			fb[3*((j)+(i)*240)+1] = iter << 2;
			fb[3*((j)+(i)*240)+2] = iter << 3;

		}
	}
}