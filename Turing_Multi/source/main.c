// Multi-Scale Turing patterns //

#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <math.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define TWO_PI 6.28318530717958647693f

#define WIDTH 400
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)


float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}


  float sinus[7] = { 0, sinf(TWO_PI/1.0f), sinf(TWO_PI/2.0f),  sinf(TWO_PI/3.0f),  sinf(TWO_PI/4.0f),  sinf(TWO_PI/5.0f),  sinf(TWO_PI/6.0f) };
  float cosinus[7] = { 0, cosf(TWO_PI/1.0f), cosf(TWO_PI/2.0f),  cosf(TWO_PI/3.0f),  cosf(TWO_PI/4.0f),  cosf(TWO_PI/5.0f),  cosf(TWO_PI/6.0f) };

  int level, radius, i, x, y;
  int blurlevels, symmetry;
  bool invert = false;
  float base;
  int levels;
  float stepScale;
  float stepOffset;
  float blurFactor;

  int radii[WIDTH];
  float stepSizes[WIDTH];

  float grid[SCR];
  float blurBuffer[SCR];
  float variation[SCR];
  float bestVariation[SCR];
  int bestLevel[SCR];
  bool direction[SCR];

  float activator[SCR];
  float inhibitor[SCR];
  float swap[SCR];

 int getSymmetry(int i, int w, int h) {
  if(symmetry <= 1) return i;
  if(symmetry == 2) return SCR - 1 - i;
  int x1 = i % w;
  int y1 = i / w;
  float dx = x1 - w/2.0f;
  float dy = y1 - h/2.0f;
  int x2 = w/2 + (int)(dx * cosinus[symmetry] + dy * sinus[symmetry]);
  int y2 = h/2 + (int)(dx * -sinus[symmetry] + dy * cosinus[symmetry]);
  int j = x2 + y2 * w;
  return j<0 ? j+SCR : j>=SCR ? j-SCR : j;
}
    
 void rndrule() {
 
	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	
	memset(fb, 0, SCR * 3);

	symmetry = rand()%7;
	base = randomf(1.5f, 2.4f);
	stepScale = randomf(0.01f, 0.2f);
	stepOffset = randomf(0.01f, 0.4f);
	blurFactor = randomf(0.5f, 1.0f);

	levels = (int) (log(max(WIDTH,HEIGHT)) / logf(base)) - 1.0f;
	blurlevels = (int) max(0, (levels+1) * blurFactor - 0.5f);

	for (i = 0; i < levels; i++) {
		int radius = (int)powf(base, i);
		radii[i] = radius;
		stepSizes[i] = logf(radius) * stepScale + stepOffset;
	}

	for (i = 0; i < SCR; i++) grid[i] = randomf(-1.0f, 1.0f);

} 


int main(int argc, char **argv) {

	gfxInitDefault();

	consoleInit(GFX_BOTTOM, NULL);

	printf("Multi-Scale Turing patterns\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random rule\n");
	printf("Key B = Random fill\n");
	printf("Key Y = Invert\n");
	printf(" \n");
	printf("START = Exit\n");
	
	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, false);
	
	rndrule();



	while (aptMainLoop()) {
	
		u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

		gspWaitForVBlank();

		hidScanInput();

		if (hidKeysDown() & KEY_START) break;
		if (hidKeysDown() & KEY_A) rndrule();
		if (hidKeysDown() & KEY_B) for (i = 0; i < SCR; i++) grid[i] = randomf(-1.0f, 1.0f);
		if (hidKeysDown() & KEY_Y) invert = !invert;
   
		if(symmetry >= 1) for(i = 0; i < SCR; i++) grid[i] = grid[i] * 0.9f + grid[getSymmetry(i, WIDTH, HEIGHT)] * 0.1f;
		
		memcpy(activator, grid, 4*SCR);

		for (level = 0; level < levels - 1; level++) {

			int radius = radii[level];

			if(level <= blurlevels){
        
			for (y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					int t = y * WIDTH + x;
					if (y == 0 && x == 0) blurBuffer[t] = activator[t];
					else if (y == 0) blurBuffer[t] = blurBuffer[t - 1] + activator[t];
					else if (x == 0) blurBuffer[t] = blurBuffer[t - WIDTH] + activator[t];
					else blurBuffer[t] = blurBuffer[t - 1] + blurBuffer[t - WIDTH] - blurBuffer[t - WIDTH - 1] + activator[t];
				}
			}
		}
    
		for (y = 0; y < HEIGHT; y++) {
			for (x = 0; x < WIDTH; x++) {
				int minx = max(0, x - radius);
				int maxx = min(x + radius, WIDTH - 1);
				int miny = max(0, y - radius);
				int maxy = min(y + radius, HEIGHT - 1);
				int area = (maxx - minx) * (maxy - miny);

				int nw = miny * WIDTH + minx;
				int ne = miny * WIDTH + maxx;
				int sw = maxy * WIDTH + minx;
				int se = maxy * WIDTH + maxx;

				int t = y * WIDTH + x;
				inhibitor[t] = (blurBuffer[se] - blurBuffer[sw] - blurBuffer[ne] + blurBuffer[nw]) / area;
			}	
		}
        
		for (i = 0; i < SCR; i++) {
			float variation = fabs(activator[i] - inhibitor[i]);
			if (level == 0 || variation < bestVariation[i]) {
				bestVariation[i] = variation;
				bestLevel[i] = level;
				direction[i] = activator[i] > inhibitor[i];
			}
		}

		if(level==0) {
			memcpy(activator, inhibitor, 4*SCR);
		} else {
			memcpy(swap, activator, 4*SCR);
			memcpy(activator, inhibitor, 4*SCR);
			memcpy(inhibitor, swap, 4*SCR);
		}
            
		}	

		float smallest = MAXFLOAT;
		float largest = -MAXFLOAT;

		for (i = 0; i < SCR; i++) {
			float curStep = stepSizes[bestLevel[i]];
			if (direction[i])grid[i] += curStep;
			else grid[i] -= curStep;
			smallest = min(smallest, grid[i]);
			largest = max(largest, grid[i]);
		}

		float range = (largest - smallest) / 2.0f;
  
		for (i = 0; i < SCR; i++) grid[i] = ((grid[i] - smallest) / range) - 1.0f;
  
		for (y = 0; y < HEIGHT; y++) {
			for (x = 0; x < WIDTH; x++) {
				int t = y * WIDTH + x;
				uint8_t col = invert ? 255-(128 + 128 * grid[t]) : (128 + 128 * grid[t]);			
				fb[3*(y+x*240)]   = col;
				fb[3*(y+x*240)+1] = col;
				fb[3*(y+x*240)+2] = col;
			}		
		}

		gfxFlushBuffers();
		gfxSwapBuffers();

	}
	
	gfxExit();

	return 0;

}