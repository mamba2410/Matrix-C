#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmx_matrix.h>

// Dev branch stuff
int main(int argc, char** argv){

	srand(time(NULL));

	cmx_matrix_t m2 = cmx_load_matrix("m4.m");

	cmx_printf(m2);

	cmx_destroy(m2);
	return 0;
}

