#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <matrix.h>

int main(int argc, char** argv){
	srand(time(NULL));
	
	double m44[] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
	matrix_t I4 = m_init(m44, 4, 4);

	m_printf(I4);
	
	matrix_t m3;
	m3 = m_minor(I4, 0, 3);
	m_printf(m3);

	printf("m_det: %f\n", m_det(I4));
	printf("m_det: %f\n", m_det(m3));
	
	m_destroy(m3);
	m_destroy(I4);
	return 0;
}

