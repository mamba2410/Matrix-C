#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <matrix.h>

double fff(double a){
		return 3*a;
}

int main(int argc, char** argv){
		double data[] = {0.5, 1.5, 2.5, 3.5};
		double data2[] = {0.25, 1.25, 2.25, 3.25};
		matrix_t m1 = m_init(data, 1, 4);
		matrix_t m2 = m_init(data2, 1, 4);
		m_printf(m1);

		matrix_t m3 = m_add(m1, m2);
		m_printf(m3);

		matrix_t m4 = m_sub(m1, m2);
		m_printf(m4);
		
		//m3 = m_func(m1, fff);
		//m_printf(m3);

		m_destroy(m1);
		m_destroy(m2);
		m_destroy(m3);
		m_destroy(m4);
		return 0;
}

