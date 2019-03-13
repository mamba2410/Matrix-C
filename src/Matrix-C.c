#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <matrix.h>

int main(int argc, char** argv){
	srand(time(NULL));
	
	matrix_t m4 = m_make(4, 4);
	matrix_t inv, prod, I;

	m_noise(m4);

	inv = m_inverse(m4);
	m_printf(inv);

	prod = m_product(inv, m4);
	m_printf(prod);

	I = m_getI(3);
	m_printf(I);

	m_destroy(m4);
	m_destroy(inv);
	m_destroy(prod);
	m_destroy(I);

	return 0;
}

