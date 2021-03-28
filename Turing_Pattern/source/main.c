// Simple Turing patterns //


#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <math.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define TWO_PI 6.28318530717958647693f

#define WIDTH 200
#define HEIGHT 120
#define SCR (WIDTH * HEIGHT)
#define SCREEN (400 * 240)

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}


	#define SCL 4
	int lim = 128;
	int dirs = 9;
	int patt = 0;
  
	float pat[SCR];
	float pnew[SCR];  
	float pmedian[SCR][SCL];
	float prange[SCR][SCL];
	float pvar[SCR][SCL];
	
	bool invert = false;
	bool color = false;
	
	char col;
	
void rndrule(){
	
	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	
	memset(fb, 0, SCREEN * 3);

	lim = 96 + rand()%96;
	dirs = 6 + rand()%7;
	patt = rand()%3;
	
	for(int j=0; j<SCL; j++){
	
		for(int i=0; i<SCR; i++) {
		
			pmedian[i][j] = 0;
			prange[i][j] = 0;
			pvar[i][j] = 0;
	
		}
	}
  
	for(int i=0; i<SCR; i++) pat[i] = randomf(0, 255.0f);
	
}


int main(int argc, char **argv) {


	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("Simple Turing patterns\n");
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
		
			u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

			gspWaitForVBlank();

			hidScanInput();

			if (hidKeysDown() & KEY_START) break;		
			if (hidKeysDown() & KEY_A) rndrule();	
			if (hidKeysDown() & KEY_B) invert = !invert;	
			if (hidKeysDown() & KEY_Y) color = !color;
			
			float R = randomf(0, TWO_PI);
			memcpy(pnew, pat, 4 * SCR);
	
			for(int j=0; j<SCL; j++){
	
				for(int i=0; i<SCR; i++) {
		
					pmedian[i][j] = 0;
					prange[i][j] = 0;
					pvar[i][j] = 0;
	
				}
			}
			
			for(int i=0; i<SCL; i++) {

				float d = (2<<i);

				for(int j=0; j<dirs; j++) {
					float dir = j * TWO_PI / dirs + R;
					int dx = (d * cos(dir));
					int dy = (d * sin(dir));     
					for(int l=0; l<SCR; l++) {
						int x1 = l%WIDTH + dx, y1 = l/WIDTH + dy;
						if(x1<0) x1 = WIDTH-1-(-x1-1); if(x1>=WIDTH) x1 = x1%WIDTH;
						if(y1<0) y1 = HEIGHT-1-(-y1-1); if(y1>=HEIGHT) y1 = y1%HEIGHT;
						pmedian[l][i] += pat[x1+y1*WIDTH] / dirs;
					}
				}

				for(int j=0; j<dirs; j++) {
					float dir = j * TWO_PI / dirs + R;
					int dx = (d * cos(dir));
					int dy = (d * sin(dir));
    
					for(int l=0; l<SCR; l++) {
						int x1 = l%WIDTH + dx, y1 = l/WIDTH + dy;
						if(x1<0) x1 = WIDTH-1-(-x1-1); if(x1>=WIDTH) x1 = x1%WIDTH;
						if(y1<0) y1 = HEIGHT-1-(-y1-1); if(y1>=HEIGHT) y1 = y1%HEIGHT;
						pvar[l][i] += fabs(pat[x1+y1*WIDTH] - pmedian[l][i]) / dirs;
						prange[l][i] += pat[x1+y1*WIDTH] > (lim + i*10) ? +1.0f : -1.0f;
					}
				}
			}

			for(int l=0; l<SCR; l++) {
				int imin = 0, imax = SCL;
				float vmin = MAXFLOAT;
				float vmax = -MAXFLOAT;

				for(int i=0; i<SCL; i+=1) {
					if(pvar[l][i] <= vmin) { vmin = pvar[l][i]; imin = i; }
					if(pvar[l][i] >= vmax) { vmax = pvar[l][i]; imax = i; }
				}

				switch(patt){
					case 0: for(int i=0; i<=imin; i++)    pnew[l] += prange[l][i]; break;
					case 1: for(int i=imin; i<=imax; i++) pnew[l] += prange[l][i]; break;
					case 2: for(int i=imin; i<=imax; i++) pnew[l] += prange[l][i] + pvar[l][i] / 2.0f; break;
				}
			}

			float vmin = MAXFLOAT;
			float vmax = -MAXFLOAT;

			for(int i=0; i<SCR; i++) {
				vmin = min(vmin, pnew[i]);
				vmax = max(vmax, pnew[i]);     
			}

			float dv = vmax - vmin;
			
			for(int y=0; y<HEIGHT; y++) {

				for(int x=0; x<WIDTH; x++){
		
					pat[x+y*WIDTH] = (pnew[x+y*WIDTH] - vmin) * 255.0f / dv;
					col = invert ? 255-pat[x+y*WIDTH]: pat[x+y*WIDTH];
				
					if (color == 0) {
						fb[3*((2*y)+(2*x)*240)]   = col;
						fb[3*((2*y)+(2*x)*240)+1] = col;
						fb[3*((2*y)+(2*x)*240)+2] = col;		
					} else {		
						fb[3*((2*y)+(2*x)*240)]   = col;
						fb[3*((2*y)+(2*x)*240)+1] = col<<1;
						fb[3*((2*y)+(2*x)*240)+2] = col<<2;	
					}

				}
			
			}

			gfxFlushBuffers();
			gfxSwapBuffers();

		}

	gfxExit();

	return 0;

}