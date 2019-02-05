#ifndef MATRIX_H
#define MATRIX_H

#include <matrix_utils.h>

// Gets the size of the matrix object
#define MATRIX_SIZE sizeof(struct matrix)


/*
 *	The actual matrix structure.
 *	double *data - the data array that is used to store the values.
 *		data is stored as consecutive rows, so row 0, is directly to the left of row 1
 *	int rows - The number of rows in the matrix
 *	int columns - the number of columns in the matrix
 *	
 *	Note: The size of the data array must equal r*c
 */
typedef struct matrix {
		double *data;
		index_t rows, columns;
} matrix_t;

// Initialisation of matrices
matrix_t	m_init(double *data, index_t r, index_t c);
matrix_t	m_make(index_t r, index_t c);
int			m_destroy(matrix_t matrix);

/*
 *
 *	Make a mc_add function that copies matrices and returns a brand new matrix with new data pointer
 *	keep m_add as overwriting the original m.data
 *	Apply the same convention to all matrix operations
 *
 */

// Vector space matrix functions
matrix_t	m_add(matrix_t, matrix_t);
matrix_t	m_sub(matrix_t, matrix_t);
matrix_t	m_scalar(matrix_t, double);
matrix_t	m_func(matrix_t, double (*f)(double));

// Matrix operations
matrix_t	m_dot(matrix_t, matrix_t);
matrix_t	m_transpose(matrix_t);

// Matrix data manipulation
double		m_get(matrix_t, index_t r, index_t c);
matrix_t	m_put(matrix_t, double);

// Misc functions
matrix_t*	m_noise(matrix_t*);
double		m_mean(matrix_t);
double		m_py_sum(matrix_t);
double		m_sum(matrix_t);

// Printing and formatting
void		m_print(matrix_t);
void		m_printf(matrix_t);

#endif
