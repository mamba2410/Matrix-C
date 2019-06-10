#ifndef CMX_MATRIX_H
#define CMX_MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>


// Define max data bytes as 8GiB
// This is 2^33 bits
#define CMX_MAX_BYTES 0x200000000

// size_t is 8 bytes on 64-bit, so max data length is 2^(8*8) = 2^64
// However, max bytes is 8GiB so only 32-bit will fit
#define CMX_MAX_DATA_LENGTH 0x100000000

// Max side length of matrix will be square root of max data length
// This is 16 bits, or 2 bytes
#define CMX_MAX_SIDE_LENGTH 0x10000

// Set max array length to max bytes / the size of a matrix
#define CMX_MAX_ARRAY_LENGTH CMX_MAX_BYTES/CMX_MATRIX_SIZE

// Define a macro to initialise a matrix array from the heap
#define CMX_ARRAY_ASSIGN(array_position, m) \
	memcpy((void*)(&array_position), (void*)(&m), CMX_MATRIX_SIZE);



/*
 *	The actual Matrix structure.
 *	double *data - the data array that is used to store the values.
 *		data is stored as consecutive rows, so row 0, is directly to the left of row 1
 *	int rows - The number of rows in the Matrix
 *	int columns - the number of columns in the Matrix
 *	
 *	Note: The size of the data array must equal r*c
 */
typedef struct cmx_matrix {
	double * const data;
	const size_t rows, columns, length;
} cmx_matrix_t;


// Gets the size of the Matrix object
// sizeof operator is apparently evaluated at compile time
#define CMX_MATRIX_SIZE sizeof(cmx_matrix_t)

// Initialisation of matrices
cmx_matrix_t	cmx_init(double*, size_t, size_t);
cmx_matrix_t	cmx_init_copy(double*, size_t, size_t);
cmx_matrix_t	cmx_make(size_t, size_t);
void			cmx_destroy(cmx_matrix_t);
void			cmx_destroy_contents(cmx_matrix_t*, size_t);
cmx_matrix_t*	cmx_destroy_all(cmx_matrix_t*, size_t);
cmx_matrix_t	cmx_copy(cmx_matrix_t);
cmx_matrix_t	cmx_identity(size_t);

// Vector space Matrix functions
cmx_matrix_t	cmx_add(cmx_matrix_t, cmx_matrix_t);
cmx_matrix_t	cmx_sub(cmx_matrix_t, cmx_matrix_t);
cmx_matrix_t	cmx_scalar(cmx_matrix_t, double);
cmx_matrix_t	cmx_func(cmx_matrix_t, double (*f)(double));

// Vector operations
double			cmx_v_dot(cmx_matrix_t, cmx_matrix_t);
cmx_matrix_t	cmx_v_cross(cmx_matrix_t, cmx_matrix_t);
double			cmx_v_mag(cmx_matrix_t);

// Elementary row operations
cmx_matrix_t	cmx_eros_scalar(cmx_matrix_t, size_t, double);
cmx_matrix_t	cmx_eros_swap(cmx_matrix_t, size_t, size_t);
cmx_matrix_t	cmx_eros_add(cmx_matrix_t, size_t, double, size_t);

// Row-Echelon and Row-Reduced Echelon form
cmx_matrix_t	cmx_ref(cmx_matrix_t);
cmx_matrix_t	cmx_rref(cmx_matrix_t);

// Matrix operations
cmx_matrix_t	cmx_product(cmx_matrix_t, cmx_matrix_t);
cmx_matrix_t	cmx_transpose(cmx_matrix_t);
double			cmx_det(cmx_matrix_t);
cmx_matrix_t	cmx_inverse(cmx_matrix_t);

// Matrix data manipulation
double			cmx_get(cmx_matrix_t, size_t r, size_t c);
cmx_matrix_t	cmx_put(cmx_matrix_t, double, size_t r, size_t c);
cmx_matrix_t	cmx_getr(cmx_matrix_t, size_t);
cmx_matrix_t	cmx_putr(cmx_matrix_t, cmx_matrix_t, size_t);
cmx_matrix_t	cmx_delr(cmx_matrix_t, size_t);
cmx_matrix_t	cmx_delc(cmx_matrix_t, size_t);
cmx_matrix_t	cmx_minor(cmx_matrix_t, size_t, size_t);
double			cmx_get_leader(cmx_matrix_t, size_t);
size_t			cmx_get_leader_col(cmx_matrix_t, size_t);
cmx_matrix_t	cmx_order_rows(cmx_matrix_t);
cmx_matrix_t	cmx_shift_zeros(cmx_matrix_t);

// Misc functions
cmx_matrix_t	cmx_noise(cmx_matrix_t);
double			cmx_sum(cmx_matrix_t);
double			cmx_mean(cmx_matrix_t);
double			cmx_sqsum(cmx_matrix_t);
double			cmx_rsqsum(cmx_matrix_t);
double			cmx_rms(cmx_matrix_t);

// Printing, storing and formatting
void			cmx_print(cmx_matrix_t);
void			cmx_printf(cmx_matrix_t);
void			cmx_store_matrix(cmx_matrix_t, char*, char);
void			cmx_store_file(cmx_matrix_t*, size_t, char*, char);
cmx_matrix_t	cmx_load_matrix(char*);
cmx_matrix_t*	cmx_load_file(char*, size_t);

#endif
