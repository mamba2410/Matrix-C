#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmx_matrix.h>

// Dev branch stuff
int main(int argc, char** argv){

	srand(time(NULL));

	cmx_matrix_t m1 = cmx_make(4, 3);
	cmx_matrix_t m2 = cmx_make(5, 6);

	cmx_noise(m1);
	cmx_noise(m2);

	cmx_matrix_t *ms = (cmx_matrix_t*)malloc( 2 * CMX_MATRIX_SIZE );
	ms[0] = m1;
	ms[1] = m2;
	cmx_printf(ms[0]);
	cmx_printf(ms[1]);

	cmx_store_file(ms, 2, "m3.m", 'w');
	
	printf("Matrices stored, moving on to loading.\n");

	cmx_matrix_t *ms2 = cmx_load_file("m3.m", 2);
	cmx_printf(ms2[0]);
	cmx_printf(ms2[1]);

	cmx_destroy(m1);
	cmx_destroy(m2);
	free(ms);
	free(ms2);
	return 0;
}

