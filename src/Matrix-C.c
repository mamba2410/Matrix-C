#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <matrix.h>

int main(int argc, char** argv){
	srand(time(NULL));
	
	double m44[] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
	double m442[] = {0, 0, 3, 9, 0, 0, 0, 0, 1, 5, 9, -1, 0, 8, 3, 2};

	matrix_t m4 = m_make(4, 5);
	matrix_t ref;

	m_noise(m4);

	for(index_t i = 0; i < 1e4; i++){
		m_noise(m4);
		ref = m_rref(m4);
		m_printf(ref);
		m_destroy(ref);
	}


	m_destroy(m4);

	return 0;
}

