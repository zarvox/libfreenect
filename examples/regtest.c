#include "libfreenect.h"
#include <stdio.h>

#define DEPTH_X_RES 640
#define DEPTH_Y_RES 480

uint8_t   input[640*480];
uint16_t output[640*480];
	
freenect_registration registration;

// main routine

int main() {

	freenect_init_registration(0,&registration);

#if 0
	FILE* df = fopen("depth.raw","r");
	if (!df) { printf("could not open depth.raw (expecting packed 640x480 11-bit depth data)\n"); return 1; }
	fread(input,640*480*11/8,1,df);
	fclose(df);

	freenect_apply_registration( &registration, input, output );
	
	FILE* cf = fopen("calib.pgm","w+");
	fprintf(cf,"P5 640 480 65535 ");
	fwrite(output,640*480*2,1,cf);
	fclose(cf);
#endif

	int i, n = DEPTH_X_RES * DEPTH_Y_RES;

	printf("\n");

	printf("Depth-to-mm table:\nraw_value mm_value\n");
	for (i = 0; i < 2048; i++) {
		printf("%d %d\n",i,registration.raw_to_mm_shift[i]);
	}
	printf("\n");

	printf("Depth-to-RGB table:\nmm_value shift*16\n");
	for (i = 0; i < 10000; i++) {
		printf("%d %d\n",i,registration.depth_to_rgb_shift[i]/16);
	}
	printf("\n");

	printf("Distortion table:\noffset xshift*16 yshift\n");
	for (i = 0; i < n; i++) {
		printf("%d %d %d\n",i,registration.registration_table[i][0]/16,registration.registration_table[i][1]);
	}
	printf("\n");

	return 0;
}

