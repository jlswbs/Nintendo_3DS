// FitzHugh-Nagumo Reaction-Diffusion //


#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <math.h>


#define WIDTH  200
#define HEIGHT 120
#define SCREEN 400*240


float randomf(float minf, float maxf) {return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);}


	int i,j;

	float gridU[WIDTH][HEIGHT];
	float gridV[WIDTH][HEIGHT];
	float gridNext[WIDTH][HEIGHT];

	float reactionRate = 0.2f;

	float diffusionRateV = 0.01f;
	float diffusionRateU = 0.04f;
	float diffusionRateUX = 0.04f;
	float diffusionRateUY = 0.03f;


	float diffRateUYarr[WIDTH][HEIGHT];
	float diffRateUXarr[WIDTH][HEIGHT];

	float deltaT = 4.0f;

	float Farr[WIDTH][HEIGHT];
	float karr[WIDTH][HEIGHT];
	float temp[WIDTH][HEIGHT];  

	bool invert = false;
	bool color = false;



void rndrule() {

	i = 0;
	j = 0;

	u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

	memset(fb, 0, SCREEN * 3);
	
	diffusionRateV = randomf(0.01f, 0.05f);
	diffusionRateU = randomf(0.01f, 0.05f);
	diffusionRateUX = randomf(0.01f, 0.09f);
	diffusionRateUY = randomf(0.01f, 0.09f);

	for(i=0;i<WIDTH;++i) {
		
		for(j=0;j<HEIGHT;++j) {
			
			gridU[i][j] = 0.5f + randomf(-0.01f, 0.01f);
			gridV[i][j] = 0.25f + randomf(-0.01f, 0.01f);
				
		}
	}

	setupF();
	setupK();
	setupDiffRates();

}


void diffusionU() {
  for(i=0;i<WIDTH;++i) {
    for(j=0;j<HEIGHT;++j) {

      gridNext[i][j] = gridU[i][j]+deltaT*
        (diffRateUXarr[i][j]*(gridU[(i-1+WIDTH)%WIDTH][j]
        +gridU[(i+1)%WIDTH][j]-2*gridU[i][j])+
        diffRateUYarr[i][j]*(gridU[i][(j-1+HEIGHT)%HEIGHT]
        +gridU[i][(j+1)%HEIGHT]
        -2*gridU[i][j]));
    }
  }
  
  for(i=0;i<WIDTH;++i) {
    for(j=0;j<HEIGHT;++j) {
  temp[i][j] = gridU[i][j];
  gridU[i][j] = gridNext[i][j];
  gridNext[i][j] = temp[i][j];
    }
  }
  
}


void diffusionV() {
  for(i=0;i<WIDTH;++i) {
    for(j=0;j<HEIGHT;++j) {

      gridNext[i][j] = gridV[i][j]+diffusionRateV*deltaT*
        (gridV[(i-1+WIDTH)%WIDTH][j]
        +gridV[(i+1)%WIDTH][j]
        +gridV[i][(j-1+HEIGHT)%HEIGHT]
        +gridV[i][(j+1)%HEIGHT]
        -4*gridV[i][j]
        );
    }
  }
  
  for(i=0;i<WIDTH;++i) {
    for(j=0;j<HEIGHT;++j) {
  temp[i][j] = gridV[i][j];
  gridV[i][j] = gridNext[i][j];
  gridNext[i][j] = temp[i][j];
  
    }
  }
}


void reaction() {

  for(i=0;i<WIDTH;++i) {
  
    for(j=0;j<HEIGHT;++j) {

      gridU[i][j] = gridU[i][j] + deltaT*(reactionRate*(gridU[i][j]-gridU[i][j]*gridU[i][j]*gridU[i][j]-gridV[i][j]+karr[i][j]));
      gridV[i][j] = gridV[i][j] + deltaT*(reactionRate*Farr[i][j]*(gridU[i][j]-gridV[i][j]));
    
		}
	}
}


void setupF() {
  
  for(i=0;i<WIDTH;++i) {
  
    for(j=0;j<HEIGHT;++j) Farr[i][j] = 0.01f + i * 0.09f / WIDTH;
	
	}
}


void setupK() {

  for(i=0;i<WIDTH;++i) {
  
    for(j=0;j<HEIGHT;++j) karr[i][j] = 0.06f + j * 0.4f / HEIGHT;
	  
	} 
}


void setupDiffRates() {
  
  for(i=0;i<WIDTH;++i) {
  
    for(j=0;j<HEIGHT;++j) {
	
      diffRateUYarr[i][j] = 0.03f + j * 0.04f / HEIGHT;
      diffRateUXarr[i][j] = 0.03f + j * 0.02f / HEIGHT;
	  
		}
	}
}


int main(int argc, char **argv) {


	gfxInitDefault();
	
	consoleInit(GFX_BOTTOM, NULL);

	printf("FitzHugh-Nagumo reaction diffusion\n");
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

			diffusionU();
			diffusionV();
			reaction();
	
			for(i=0;i<WIDTH;++i) {
		
				for(j=0;j<HEIGHT;++j) {
		
					uint8_t col = invert ? 255-(255*gridU[i][j]) : (255*gridU[i][j]);

					if (color == 0) {

						fb[3*((2*j)+(2*i)*240)]   = col;
						fb[3*((2*j)+(2*i)*240)+1] = col;
						fb[3*((2*j)+(2*i)*240)+2] = col;
				
					}
				
					else {
				
						fb[3*((2*j)+(2*i)*240)]   = col;
						fb[3*((2*j)+(2*i)*240)+1] = col<<1;
						fb[3*((2*j)+(2*i)*240)+2] = col<<2;
				
					}

				}
			}

			gfxFlushBuffers();
			gfxSwapBuffers();


		}


	gfxExit();

	return 0;

}