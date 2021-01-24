// Fizzy cellular automata //


#include <string.h>
#include <stdio.h>
#include <3ds.h>
#include <math.h>

#define WIDTH  200
#define HEIGHT 120
#define SCREEN 400*240

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}


	char calm = 233;
	int CellIndex = 0;
	float a = 4.7f;
	float b = 3.9f;
	float c = 5.5f;

	int i,j,k;

	float CellVal[WIDTH*HEIGHT];
	
	bool grayscale = false;
	
	
void rndrule(){

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	
	memset(fb, 0, SCREEN * 3);
	
	calm = 36 + rand()%220;
	
	a = randomf(1.0f, 5.5f);
	b = randomf(1.0f, 5.5f);
	c = randomf(1.0f, 5.5f);
	
	for (k = 0; k < (WIDTH*HEIGHT); k++) CellVal[k] =  rand()%256;

}


int main(int argc, char **argv) {


	gfxInitDefault();

	consoleInit(GFX_BOTTOM, NULL);
	
	printf("Fizzy cellular automata\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("Key A = Random rule\n");
	printf("Key B = Grayscale\n");
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
		if (hidKeysDown() & KEY_B) grayscale = !grayscale;

			
		for (i = 0; i < WIDTH; i++) {
      
			for (j = 0; j < HEIGHT; j++) {
      
				CellIndex = (CellIndex+1)%(WIDTH*HEIGHT);
      
				char klimp  = (char)(CellVal[CellIndex]*a);       
				char nifna  = (char)(CellVal[CellIndex]*b);
				char blugg  = (char)(CellVal[CellIndex]*c);

				u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
				
				if (grayscale == true) {
				
					fb[3*((2*j)+(2*i)*240)]   = klimp;
					fb[3*((2*j)+(2*i)*240)+1] = klimp;
					fb[3*((2*j)+(2*i)*240)+2] = klimp;
				
				} else {
				
					fb[3*((2*j)+(2*i)*240)]   = klimp;
					fb[3*((2*j)+(2*i)*240)+1] = nifna;
					fb[3*((2*j)+(2*i)*240)+2] = blugg;
				
				}
   
				int below      = (CellIndex+1)%(WIDTH*HEIGHT);
				int above      = (CellIndex+(WIDTH*HEIGHT)-1)%(WIDTH*HEIGHT);
				int left       = (CellIndex+(WIDTH*HEIGHT)-HEIGHT)%(WIDTH*HEIGHT);
				int right      = (CellIndex+HEIGHT)%(WIDTH*HEIGHT);
				int aboveright = ((CellIndex-1) + HEIGHT +(WIDTH*HEIGHT))%(WIDTH*HEIGHT);
				int aboveleft  = ((CellIndex-1) - HEIGHT +(WIDTH*HEIGHT))%(WIDTH*HEIGHT);
				int belowright = ((CellIndex+1) + HEIGHT +(WIDTH*HEIGHT))%(WIDTH*HEIGHT);
				int belowleft  = ((CellIndex+1) - HEIGHT +(WIDTH*HEIGHT))%(WIDTH*HEIGHT);

				float NeighbourMix = powf((CellVal[left]*CellVal[right]*CellVal[above]*CellVal[below]*CellVal[belowleft]*CellVal[belowright]*CellVal[aboveleft]*CellVal[aboveright]), 0.125f);
				
				CellVal[CellIndex] = fmod((((sqrtf(CellVal[CellIndex]))*(sqrtf(NeighbourMix)))+0.5f), calm); 
   
			}
	
		}
				
		gfxFlushBuffers();
		gfxSwapBuffers();

	}


	gfxExit();

	return 0;

}