#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <matrix.h>

int main(int argc, char** argv){
	srand(time(NULL));
	
	matrix_t m4 = m_make(4, 4);
	matrix_t m1 = m_make(3, 2);
	m_noise(m4);
	m_noise(m1);
	
	m_storef(m4, "m4.m", 'w');
	m_storef(m1, "m4.m", 'a');
	
	matrix_t *ms = m_bloadf("m4.m", 2);
	
	m_printf(m4);
	m_printf(ms[0]);

	m_printf(m1);
	m_printf(ms[1]);

	m_destroy(m1);
	m_destroy(ms[1]);
	m_destroy(m4);
	m_destroy(ms[0]);
	return 0;
}

