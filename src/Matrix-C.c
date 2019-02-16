#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <matrix.h>

int main(int argc, char** argv){
	srand(time(NULL));
	
	double m44[] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
	double dv1[] = {1, 0, 0};
	double dv2[] = {0, 1, 0};

	matrix_t v1 = m_init(dv1, 3, 1);
	matrix_t v2 = m_init(dv2, 3, 1);

	m_printf(v1);
	m_printf(v2);

	matrix_t v3 = v_cross(v1, v2);
	m_printf(v3);
	printf("Dot product: %f\n", v_dot(v1, v1));

	m_destroy(v1);
	m_destroy(v2);
	m_destroy(v3);

	return 0;
}

