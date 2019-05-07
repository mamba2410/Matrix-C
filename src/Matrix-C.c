#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <matrix.h>

int main(int argc, char** argv){
	srand(time(NULL));
	
	matrix_t *ms = m_bloadf("m4.m", 2);
	
	m_printf(ms[0]);
	m_printf(ms[1]);

	m_destroy(ms[1]);
	m_destroy(ms[0]);
	free(ms);
	return 0;
}

