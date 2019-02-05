#include <stdlib.h>
#include <stdio.h>

#include <matrix.h>

/*
 *	Create a new matrix given an array and the size the matrix should be
 *	double* data - The data array that should be used
 *	index_t r - The number of rows in the matrix
 *	index_t c - The number of columns in the matrix
 */
matrix_t m_init(double *data, index_t r, index_t c){
		matrix_t matrix = m_make(r, c);
		for(index_t i = 0; i < r*c; i++)
				matrix.data[i] = data[i];
		return matrix;
}

/*
 *	Create a new matrix give the number of rows and columns. Initialises all entries with 0;
 *	index_t r - The number of rows in the matrix
 *	index_t c - The number of columns in the matrix
 */
matrix_t m_make(index_t r, index_t c){
		double *data = (double*)malloc(MATRIX_SIZE * r * c);
		for(index_t i = 0; i < r*c; i++)
				data[i] = 0.0;
		matrix_t matrix = {data, r, c};
		return matrix;
}

/*
 * Destroys a given matrix, freeing the data used by the matrix
 * matrix_t *matrix - The matrix to be destroyed
 */
int m_destroy(matrix_t matrix){
		free(matrix.data);
		return 0;
}

/*
 * Adds two matrices together, reated like vector addition
 * matrix_t m1 - The first matrix
 * matrix_t m2 - The second matrix
 */
matrix_t m_add(matrix_t m1, matrix_t m2){
		matrix_t m3 = m_make(m1.rows, m1.columns);
		if(m1.rows != m2.rows || m1.columns != m2.columns){
				printf("ERROR: Matrix size mismatch when adding, have a %d,%d and %d,%d\n", m1.rows, m1.columns, m2.rows, m2.columns);
				return m3;
		}
		for(index_t i = 0; i < m1.rows*m1.columns; i++)
				m3.data[i] = m1.data[i] + m2.data[i];

		return m3;
}

/*
 * Subtracts two matrices from each other
 * matrix_t m1 - The first matrix
 * matrix_t m2 - The matrix to subtract from m1
 */
matrix_t m_sub(matrix_t m1, matrix_t m2){
		matrix_t m3 = m_scalar(m2, -1);
		matrix_t m4 = m_add(m1, m3);
		m_destroy(m3);
		return m4;
}

/*
 * Multiplies each element in the matrix by the scalar given.
 * matrix_t m - The matrix to multiply.
 * double s - The scalar to multiply the matrix by
 */
matrix_t m_scalar(matrix_t m, double s){
		matrix_t m2 = m_make(m.rows, m.columns);
		for(index_t i = 0; i < m.rows*m.columns; i++)
				m2.data[i] = m.data[i] * s;
		return m2;
}

/*
 * Applies a function to a matrix. Function must be a mapping from double -> double
 * matrix_t m - The matrix to apply the function to
 * double (*f)(double) - A pointer to a functino that takes a double and returns a double
 */
matrix_t m_func(matrix_t m, double(*f)(double)){
		for(index_t i = 0; i < m.rows*m.columns; i++)
				m.data[i] = (*f)(m.data[i]);
		return m;
}

/*
 * Gets the value in the matrix cell (r, c)
 * matrix_t matrix - The matrix to get the information from
 * index_t r - The row to get the cell from
 * index_t c - The column to get the cell from
 */
double m_get(matrix_t matrix, index_t r, index_t c){
		if(r > matrix.rows || c > matrix.columns){
				printf("Index out of bounds error accessing matrix at (%d,%d). Matrix has size %dx%d\n", r, c, matrix.rows, matrix.columns);
				return 0;
		}
		return matrix.data[ r * matrix.columns + c ];
}
/*
 * Prints a matrix in formatted form
 * matrix_t matrix - The matrix to be printed
 */
void m_printf(matrix_t matrix){
		printf("\n");
		for(index_t r = 0; r < matrix.rows; r++){
				for(index_t c = 0; c < matrix.columns; c++)
						printf("%f\t", m_get(matrix, r, c));
				printf("\n");
		}
		printf("\n");
}



