#ifndef MATRIX_H
#define MATRIX_H

#include <matrix_utils.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
matrix_t	m_copy(matrix_t);

// Vector space matrix functions
matrix_t	m_add(matrix_t, matrix_t);
matrix_t	m_sub(matrix_t, matrix_t);
matrix_t	m_scalar(matrix_t, double);
matrix_t	m_func(matrix_t, double (*f)(double));

// Vector operations
double		v_dot(matrix_t, matrix_t);
matrix_t	v_cross(matrix_t, matrix_t);
double		v_mag(matrix_t);

// Elementary row operations
matrix_t	eros_scalar(matrix_t, index_t, double);
matrix_t	eros_swap(matrix_t, index_t, index_t);
matrix_t	eros_add(matrix_t, index_t, double, index_t);

// Row-Echelon and Row-Reduced Echelon form
matrix_t	m_ref(matrix_t);
matrix_t	m_rref(matrix_t);

// Matrix operations
matrix_t	m_product(matrix_t, matrix_t);
matrix_t	m_transpose(matrix_t);
double		m_det(matrix_t);
matrix_t	m_inverse(matrix_t);

// Matrix data manipulation
double		m_get(matrix_t, index_t r, index_t c);
matrix_t	m_put(matrix_t, double, index_t r, index_t c);
matrix_t	m_getr(matrix_t, index_t);
matrix_t	m_putr(matrix_t, matrix_t, index_t);
matrix_t	m_delr(matrix_t, index_t);
matrix_t	m_delc(matrix_t, index_t);
matrix_t	m_minor(matrix_t, index_t, index_t);

// Misc functions
matrix_t	m_noise(matrix_t);
double		m_sum(matrix_t);
double		m_mean(matrix_t);
double		m_sqsum(matrix_t);
double		m_rsqsum(matrix_t);
double		m_rms(matrix_t);

// Printing and formatting
void		m_print(matrix_t);
void		m_printf(matrix_t);

#endif
