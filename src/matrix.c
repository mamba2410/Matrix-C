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
	double *data = (double*)malloc(sizeof(double) * r * c);
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
 * Makes a duplicate of the matrix with separate data pointer, but contaisn the same values as when copied
 * matrix_t m - The matrix to be duplicated
 */
matrix_t m_copy(matrix_t m){
	matrix_t m2 = m_make(m.rows, m.columns);
	for(index_t i = 0; i < m.rows*m.columns; i++)
		m2.data[i] = m.data[i];
	return m2;

}

/*
 * Gets the identity matrix of a certain size, n
 * index_t n - The size of the matrix
 */
matrix_t m_getI(index_t n){
	matrix_t I = m_make(n, n);
	for(index_t i = 0; i < n; i++)
		m_put(I, 1.0, i, i);

	return I;
}

/*
 * Adds two matrices together, reated like vector addition
 * matrix_t m1 - The first matrix
 * matrix_t m2 - The second matrix
 * Replaces data in matrix_1 m1 with the result. Leaves matrix_t m2 unchanged
 */
matrix_t m_add(matrix_t m1, matrix_t m2){
	if(m1.rows != m2.rows || m1.columns != m2.columns){
		printf("ERROR: Matrix size mismatch when adding, have a %d,%d and %d,%d\n", m1.rows, m1.columns, m2.rows, m2.columns);
		return m1;
	}
	for(index_t i = 0; i < m1.rows*m1.columns; i++)
		m1.data[i] += m2.data[i];

	return m1;
}

/*
 * Subtracts two matrices from each other
 * matrix_t m1 - The first matrix
 * matrix_t m2 - The matrix to subtract from m1
 * Replaces data in matrix_t m1 with the result, leaves matrix_t m2 unchanged
 */
matrix_t m_sub(matrix_t m1, matrix_t m2){
	matrix_t m3 = m_copy(m2);
	m_add(m1, m_scalar(m3, -1));
	m_destroy(m3);
	return m1;
}

/*
 * Multiplies each element in the matrix by the scalar given.
 * matrix_t m - The matrix to multiply.
 * double s - The scalar to multiply the matrix by
 * Overwrites matrix data with results data
 */
matrix_t m_scalar(matrix_t m, double s){
	for(index_t i = 0; i < m.rows*m.columns; i++)
		m.data[i] *= s;
	return m;
}

/*
 * Applies a function to a matrix. Function must be a mapping from double -> double
 * matrix_t m - The matrix to apply the function to
 * double (*f)(double) - A pointer to a function that takes a double and returns a double
 * Overwrites matrix data with results data
 */
matrix_t m_func(matrix_t m, double(*f)(double)){
	for(index_t i = 0; i < m.rows*m.columns; i++)
		m.data[i] = (*f)(m.data[i]);
	return m;
}

/*
 * gets the vector dot product of two column vectors
 * Vectors must be of the form nx1
 * Output equivalent to d = m1 . m2
 * matrix_t m1 - The first vector to dot
 * matrix_t m2 - The second vector to dot
 */
double v_dot(matrix_t m1, matrix_t m2){
	if(m1.rows != m2.rows || m1.columns != m2.columns || m1.columns != 1){
		printf("Error: Size mismatch when vector dot product with %dx%d and %dx%d. Make sure they are column vectors", m1.rows, m1.columns, m2.rows, m2.columns);
		return 0;
	}
	double d = 0;
	for(index_t i = 0; i < m1.rows; i++)
		d += m_get(m1, i, 0) * m_get(m2, i, 0);
	return d;
}

/*
 * Gets the vector cross product of the vectors.
 * Currently only supports 3D vectors.
 * Vectors must be of the form nx1.
 * Output equivalent to m3 = m1 x m2
 * matrix_t m1 - The first vector to cross
 * matrix_t m2 - The second vector to cross
 */
matrix_t v_cross(matrix_t m1, matrix_t m2){
	if(m1.columns != m2.columns || m1.columns != 1 || m1.rows != m2.rows || m1.rows != 3){
		printf("Error: Cannot cross product a %dx%d and %dx%d. Make sure they are 3D column vectors\n", m1.rows, m1.columns, m2.rows, m2.columns);
		return m1;
	}
	matrix_t m3 = m_make(m1.rows, 1), temp;
	matrix_t intermediate = m_make(3, 3);
	matrix_t zeros = m_make(1, 3);

	temp = m_transpose(m1);
	m_putr(intermediate, temp, 1);
	m_destroy(temp);
	temp = m_transpose(m2);
	m_putr(intermediate, temp, 2);
	m_destroy(temp);

	for(index_t i = 0; i < m1.rows; i++){
		m_putr(intermediate, zeros, 0);
		m_put(intermediate, 1, 0, i);
		m_put(m3, m_det(intermediate), i, 0);
	}
	m_destroy(zeros);
	m_destroy(intermediate);
	return m3;
}

/*
 * Finds the magnitude of the vector
 * Vector must be in the form nx1
 * matrix_t m - The vector to find the magnitude of
 */
double v_mag(matrix_t m){
	return m_rsqsum(m);
}

/*
 * One of the elementary row operations. Takes a row, and multiplies it by a scalar and adds it back in the matrix
 * matrix_t m - The matrix to operate on
 * index_t r - The row to manipulate
 * double s - The scalar to multiply by
 */
matrix_t eros_scalar(matrix_t m, index_t r, double s){
	matrix_t row = m_getr(m, r);
	m_scalar(row, s);
	m_putr(m, row, r);
	m_destroy(row);
	//printf("\t\tr%d *= %f\n", s);
	return m;
}

/*
 * One of the elementary row operations.
 * Takes a row, and swaps it with another row
 * matrix_t m - The matrix to operate on
 * index_t i - One row to swap
 * index_t j - The other row to swap
 */
matrix_t eros_swap(matrix_t m, index_t i, index_t j){
	matrix_t ri = m_getr(m, i);
	matrix_t rj = m_getr(m, j);
	m_putr(m, ri, j);
	m_putr(m, rj, i);
	m_destroy(ri);
	m_destroy(rj);
	return m;
}

/*
 * One of the elementary row operations.
 * Takes a row, multiplies it by a scalar, and adds it to another row
 * r = r + s*q
 * matrix_t m - The matrix to operate on
 * index_t r - The destination row
 * double s - The scalar to multiply by
 * index_t q - The source row
 */
matrix_t eros_add(matrix_t m, index_t r, double s, index_t q){
	matrix_t row = m_getr(m, q);
	m_scalar(row, s);
	matrix_t row2 = m_getr(m, r);
	row = m_add(row, row2);
	m_putr(m, row, r);
	m_destroy(row);
	m_destroy(row2);
	//printf("\t\tr%d += %f r%d\n", r, s, q);
	return m;
}

/*
 * Takes a matrix and produces a copy in reduced echelon form.
 */
matrix_t m_ref(matrix_t mo){
	matrix_t m = m_copy(mo);
	double ril, rji;
	index_t rilp;

	for(index_t i = 0; i < m.rows; i++){
		m_orderRows(m);
		// Gets row i leader and the column of that leader
		ril = m_getl(m, i);
		rilp = m_getlp(m, i);
		// Skip if zero row or out of bounds
		if(rilp >= m.columns) continue;
		// Modifies the whole row to make the leader 1
		eros_scalar(m, i, 1.0/ril);
		// Checks every row underneath the leader
		for(index_t j = i; j < m.rows; j++){
			if(j == i) continue;
			// Gets the cell underneath i leader on the j column
			rji = m_get(m, j, rilp);
			// If not zero, make it zero
			eros_add(m, j, -rji, i);
		}
	}
	m_orderRows(m);
	return m;
}

/*
 * Takes a matrix and returns the row reduced echelon form 
 * matrix_t m - The matrix
 */
matrix_t m_rref(matrix_t m){
	// Put it in reduce echelon form first. Get half the work done
	m = m_ref(m);
	double ril, rji;
	index_t rilp;
	
	// For each row, starting at the bottom
	for(index_t i = m.rows-1; i > 0; i--){
		// Get the leading digit and its position
		ril = m_getl(m, i);
		rilp = m_getlp(m, i);
		// Skip if its a zero row or out of bounds
		if(rilp >= m.columns) continue;

		// For all rows above that row
		for(index_t j = i; j >= 0 && j < m.rows; j--){
			// Skip if it is this row
			if(j == i) continue;
			// Get the cell above this one
			rji = m_get(m, j, rilp);
			// Get rid of it
			if(rji != 0) eros_add(m, j, -rji, i);
		}
	}
	return m;
}

/*
 * Matrix multiplies the matrices together in the order given
 * For example:
 * m3 = m1 m2
 * matrix_t m1 - The first matrix. Transformation to apply
 * matrix_t m2 - The second matrix. Matrix to aply transformation to
 */
matrix_t m_product(matrix_t m1, matrix_t m2){
	matrix_t m3 = m_make(m1.rows, m2.columns);
	if(m1.columns != m2.rows){
		printf("Size mismatch when multiplying matrices together. Given %dx%d and %dx%d\n", m1.rows, m1.columns, m2.rows, m2.columns);
		return m3;
	}
	for(index_t i = 0; i < m2.columns; i++){
		for(index_t j = 0; j < m1.rows; j++){
			double t = 0;
			for(index_t k = 0; k < m2.rows; k++)
				t += m_get(m2, k, i) * m_get(m1, j, k);
			m_put(m3, t, j, i);
		}
	}
	return m3;
}

/*
 * Creates a new matrix which is the transpose of m
 * matrix_t m - The matrix to be transposed
 */
matrix_t m_transpose(matrix_t m){
	matrix_t m1 = m_make(m.columns, m.rows);
	for(index_t i = 0; i < m.rows; i++){
		for(index_t j = 0; j < m.columns; j++)
			m_put(m1, m_get(m, i, j), j, i);	
	}
	return m1;

}

/*
 * gets the determinant of the matrix. Matrix must be square
 * matrix_t m - The matrix to find the determinant of
 */
double m_det(matrix_t m){
	if(m.rows != m.columns){
		printf("Error: Can't find the determinant of a non-square %dx%d matrix!", m.rows, m.columns);
		return 0;
	}
	double d = 0;
	if(m.rows == 1) d = m_get(m, 0, 0);
	else {
		matrix_t minor_temp;
		for(index_t i = 0; i < m.columns; i++){
			if(m_get(m, 0, i) == 0) continue;
			minor_temp = m_minor(m, 0, i);
			d += pow(-1.0, i) * m_get(m, 0, i) * m_det(minor_temp);
			m_destroy(minor_temp);
		}
	}
	return d;
}

/*
 * Produces a copy of the matrix's inverse
 * matrix_t m - The matrix to get the inverse of
 */
matrix_t m_inverse(matrix_t m){
	if(m.rows != m.columns){
		printf("ERROR: canot find inverse of %dx%d matrix. Matrix not square.\n", m.rows, m.columns);
		return m;
	}
	double det = m_det(m);
	if(det == 0){
		printf("ERROR: Determinant of matrix zero, cannot find inverse.\n");
		return m;
	}
	matrix_t minor_temp, inverse, C = m_copy(m);
	for(index_t i = 0; i < m.rows; i++){
		for(index_t j = 0; j < m.columns; j++){
				minor_temp = m_minor(m, i, j);
				m_put(C, m_det(minor_temp) * ( (i+j)%2? -1: 1), i, j);
				m_destroy(minor_temp);
		}
	}
	
	inverse = m_transpose(C);
	m_destroy(C);
	m_scalar(inverse, 1/det);
	return inverse;

}

/*
 * Gets the value in the matrix cell (r, c)
 * matrix_t matrix - The matrix to get the information from
 * index_t r - The row to get the cell from
 * index_t c - The column to get the cell from
 */
double m_get(matrix_t matrix, index_t r, index_t c){
	if(r >= matrix.rows || c >= matrix.columns){
		printf("Index out of bounds error accessing matrix at (%d,%d). Matrix has size %dx%d\n", r, c, matrix.rows, matrix.columns);
		return 0;
	}
	return matrix.data[ r * matrix.columns + c ];
}

/*
 * Puts the value, double d into the cell specified
 * matrix_t m - The matrix to put the value in
 * double d - The value to put in the cell
 * index_t r - The index of the row 
 * index_t c - The index of the column
 */
matrix_t m_put(matrix_t m, double d, index_t r, index_t c){
	if(r >= m.rows || c >= m.columns){
		printf("Index out of bounds error accessing matrix at (%d,%d). Matrix has size %dx%d\n", r, c, m.rows, m.columns);
		return m;
	}
	m.data[r*m.columns + c] = d;
	return m;
}

/*
 * Gets the row specified and returns it as a tuple
 * matrix_t m - The matrix to retrieve the row from
 * index_t r - The row to collect
 */
matrix_t m_getr(matrix_t m, index_t r){
	matrix_t row = m_make(1, m.columns);
	if(r >= m.rows){
		printf("Index out of bounds error when retrieving matrix row %d from %dx%d matrix\n", r, m.rows, m.columns);
		return row;
	}
	for(index_t i = 0; i < m.columns; i++)
		m_put(row, m_get(m, r, i), 0, i);
	return row;
}

/*
 * Puts the specified row in the matrix
 * matrix_t m - The matrix to put the row in
 * matrix_t row - The row to insert
 * index_t r - The index of the row to insert
 */
matrix_t m_putr(matrix_t m, matrix_t row, index_t r){
	if(r >= m.rows || row.rows != 1 || row.columns != m.columns){
		printf("Error putting row in matrix. m: %dx%d r: %dx%d, at %d", m.rows, m.columns, row.rows, row.columns, r);
		return m;
	}
	for(index_t i = 0; i < m.columns; i++)
		m_put(m, m_get(row, 0, i), r, i);
	return m;

}

/*
 * Creates a copy of the matrix with the row deleted
 * matrix_t m - The matrix to delete a row from
 * index_t r - The row to delete
 */
matrix_t m_delr(matrix_t m, index_t r){
	if(m.rows <= 1){
		printf("Error: Cannot delete a row from a matrix which has only one row!\n");
		return m;
	}
	matrix_t m1 = m_make(m.rows-1, m.columns);
	matrix_t rowt;
	int f = 0;
	for(index_t i = 0; i < m.rows; i++){
		if(i==r){
			f = 1;
			continue;
		}
		rowt = m_getr(m, i);
		m_putr(m1, rowt, (f)?(i-1):i);
		m_destroy(rowt);
	}
	return m1;

}

/*
 * Creates a copy of the matrix with the column deleted
 * matrix_t m - The matrix to delete a column from
 * index_t c - The column to delete
 */
matrix_t m_delc(matrix_t m, index_t c){
	if(m.columns <= 1){
		printf("Error: Cannot delete a column from a matrix which has only one column!\n");
		return m;
	}
	matrix_t m2 = m_transpose(m);
	matrix_t m1 = m_delr(m2, c);
	matrix_t m3 = m_transpose(m1);
	m_destroy(m2);
	m_destroy(m1);
	return m3;

}

/*
 * Creates a matrix of minors by deleting a single row and column
 * matrix_t m - The matrix to make a minor out of
 * index_t r - The row to delete
 * index_t c - The column to delete
 */
matrix_t m_minor(matrix_t m, index_t r, index_t c){
	if(r >= m.rows || c >= m.columns){
		printf("Error: Index out of bounds, cannot make a minor of matrix %dx%d by deleting %d,%d\n", m.rows, m.columns, r, c);
		return m;
	}
	matrix_t m1 = m_delr(m, r);
	matrix_t m2 = m_delc(m1, c);
	m_destroy(m1);
	return m2;
}

/*
 * Gets the leading entry of a row of a matrix from a given row
 * matrix_t m - The matrix
 * index_t r - The row to find the leading entry of
 */
double m_getl(matrix_t m, index_t r){
	matrix_t row = m_getr(m, r);
	double l = 0;
	for(index_t i = 0; i < m.columns; i++){
		l = m_get(row, 0, i);
		if(l != 0) break;
	}
	m_destroy(row);
	return l;

}

/*
 * gets the column of the leading entry in a matrix, on a given row
 * matrix_t m - The matrix to use
 * index_t r - The row of the leading entry
 */
index_t m_getlp(matrix_t m, index_t r){
	index_t c = 0;
	matrix_t row = m_getr(m, r);
	double l = 0;
	for(index_t i = 0; i < m.columns; i++){
		l = m_get(row, 0, i);
		if(l != 0) break;
		c++;
	}
	m_destroy(row);
	return c;
}

/*
 * Puts rows in staircase pattern. Zero rows at bottom and 
 * matrix_t m - The matrix to order
 */
matrix_t m_orderRows(matrix_t m){
	index_t hlp=0, hlr, bottom=m.rows-1, lp;

	// For each row
	for(index_t j = 0; j < m.rows; j++){
		hlp = 0;

		// For each row that isn't in place
		// Fill from the bottom
		for(index_t i = bottom; i >= 0 && i < m.rows; i--){
			// Where is the leading digit
			lp = m_getlp(m, i);
			// If its further right than the highest
			if(lp >= hlp){
				// Set that as current highest and save which row
				hlp = lp;
				hlr = i;
			}
		}
		// Once checked each row above, put the highest row on the 'stack'
		eros_swap(m, hlr, bottom);
		// Move the 'stack pointer' up
		bottom--;
	}

	return m;
}

/*
 * Puts every zero row on the bottom of the matrix
 * matrix_t m - The matrix to manipulate
 */
matrix_t m_shiftZeros(matrix_t m){
	index_t last = m.rows-1;
	index_t lp;
	for(index_t i = 0; i < last; i++){
		lp = m_getlp(m, i);
		if(lp >= m.columns){
			eros_swap(m, i, last);
			last--;
		}
	}
	return m;
}

/*
 * Takes a matrix and replaces its contents with 'noise', random double between 0 and 1
 * MAKE SURE TO CALL srand(time(NULL)) BEFORE USING matrix_t m - The matrix to 'noise'
 */
matrix_t m_noise(matrix_t m){
	for(index_t i = 0; i < m.rows*m.columns; i++){
		m.data[i] = (double)rand()/RAND_MAX;
	}
	return m;
}

/*
 * Gets the algebraic sum of the contents
 * matrix_t m - The matrix to find the sum of
 */
double m_sum(matrix_t m){
	double s = 0;
	for(index_t i = 0; i < m.rows*m.columns; i++){
		s += m.data[i];
	}
	return s;
}

/*
 * Takes a matrix and returns the algebraic mean of the contents
 * matrix_t m - The matrix to find the mean of
 */
double m_mean(matrix_t m){
	return m_sum(m)/(m.rows*m.columns);
}

/*
 * Takes a matris and returns the sum of the squares of the cell
 * matrix_t m - The matrix to find the square sum of
 */
double m_sqsum(matrix_t m){
	double s = 0;
	for(index_t i = 0; i < m.rows*m.columns; i++)
		s += pow(m.data[i], 2);
	return s;
}

/*
 * Takes the root of the smu of squared values in the matrix
 * sqrt(m11^2 + m21^2 + ... + mpq^2)
 * matrix_t m - The matrix to operate on
 */
double m_rsqsum(matrix_t m){
	return pow(m_sqsum(m), 0.5);
}


/*
 * Takes the root mean square of the matrix
 * matrix_t m - The matrix to take the rms of
 */
double m_rms(matrix_t m){
	return pow(m_sqsum(m)/(m.rows*m.columns), 0.5);
}

/*
 * Prints a matrix in formatted form
 * matrix_t matrix - The matrix to be printed
 */
void m_printf(matrix_t matrix){
	printf("\n");
	for(index_t r = 0; r < matrix.rows; r++){
		for(index_t c = 0; c < matrix.columns; c++){
			if(m_get(matrix, r, c) >= 0)
				printf(" ");
			printf("%f\t", m_get(matrix, r, c));
		}
		printf("\n");
	}
	printf("\n");
}



