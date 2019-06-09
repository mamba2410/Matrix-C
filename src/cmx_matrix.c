#include <cmx_matrix.h>

/*
 *	Create a new matrix given an array and the size the matrix should be
 *	double* data - The data array that should be used
 *	size_t r - The number of rows in the matrix
 *	size_t c - The number of columns in the matrix
 */
cmx_matrix_t cmx_init(double *data, size_t r, size_t c){
	cmx_matrix_t matrix = cmx_make(r, c);
	for(size_t i = 0; i < r*c; i++)
		matrix.data[i] = data[i];
	return matrix;
}

/*
 *	Create a new matrix give the number of rows and columns. Initialises all entries with 0;
 *	size_t r - The number of rows in the matrix
 *	size_t c - The number of columns in the matrix
 */
cmx_matrix_t cmx_make(size_t r, size_t c){
	double *data = (double*)malloc(sizeof(double) * r * c);
	for(size_t i = 0; i < r*c; i++)
		data[i] = 0.0;
	cmx_matrix_t matrix = {data, r, c};
	return matrix;
}

/*
 * Destroys a given matrix, freeing the data used by the matrix
 * cmx_matrix_t *matrix - The matrix to be destroyed
 */
int cmx_destroy(cmx_matrix_t matrix){
	free(matrix.data);
	return 0;
}

/*
 * Makes a duplicate of the matrix with separate data pointer, but contaisn the same values as when copied
 * cmx_matrix_t m - The matrix to be duplicated
 */
cmx_matrix_t cmx_copy(cmx_matrix_t m){
	cmx_matrix_t m2 = cmx_make(m.rows, m.columns);
	for(size_t i = 0; i < m.rows*m.columns; i++)
		m2.data[i] = m.data[i];
	return m2;

}

/*
 * Gets the identity matrix of a certain size, n
 * size_t n - The size of the matrix
 */
cmx_matrix_t cmx_getI(size_t n){
	cmx_matrix_t I = cmx_make(n, n);
	for(size_t i = 0; i < n; i++)
		cmx_put(I, 1.0, i, i);

	return I;
}

/*
 * Adds two matrices together, reated like vector addition
 * cmx_matrix_t m1 - The first matrix
 * cmx_matrix_t m2 - The second matrix
 * Replaces data in matrix_1 m1 with the result. Leaves cmx_matrix_t m2 unchanged
 */
cmx_matrix_t cmx_add(cmx_matrix_t m1, cmx_matrix_t m2){
	if(m1.rows != m2.rows || m1.columns != m2.columns){
		printf("ERROR: Matrix size mismatch when adding, have a %d,%d and %d,%d\n", m1.rows, m1.columns, m2.rows, m2.columns);
		return m1;
	}
	for(size_t i = 0; i < m1.rows*m1.columns; i++)
		m1.data[i] += m2.data[i];

	return m1;
}

/*
 * Subtracts two matrices from each other
 * cmx_matrix_t m1 - The first matrix
 * cmx_matrix_t m2 - The matrix to subtract from m1
 * Replaces data in cmx_matrix_t m1 with the result, leaves cmx_matrix_t m2 unchanged
 */
cmx_matrix_t cmx_sub(cmx_matrix_t m1, cmx_matrix_t m2){
	cmx_matrix_t m3 = cmx_copy(m2);
	cmx_add(m1, cmx_scalar(m3, -1));
	cmx_destroy(m3);
	return m1;
}

/*
 * Multiplies each element in the matrix by the scalar given.
 * cmx_matrix_t m - The matrix to multiply.
 * double s - The scalar to multiply the matrix by
 * Overwrites matrix data with results data
 */
cmx_matrix_t cmx_scalar(cmx_matrix_t m, double s){
	for(size_t i = 0; i < m.rows*m.columns; i++)
		m.data[i] *= s;
	return m;
}

/*
 * Applies a function to a matrix. Function must be a mapping from double -> double
 * cmx_matrix_t m - The matrix to apply the function to
 * double (*f)(double) - A pointer to a function that takes a double and returns a double
 * Overwrites matrix data with results data
 */
cmx_matrix_t cmx_func(cmx_matrix_t m, double(*f)(double)){
	for(size_t i = 0; i < m.rows*m.columns; i++)
		m.data[i] = (*f)(m.data[i]);
	return m;
}

/*
 * gets the vector dot product of two column vectors
 * Vectors must be of the form nx1
 * Output equivalent to d = m1 . m2
 * cmx_matrix_t m1 - The first vector to dot
 * cmx_matrix_t m2 - The second vector to dot
 */
double v_dot(cmx_matrix_t m1, cmx_matrix_t m2){
	if(m1.rows != m2.rows || m1.columns != m2.columns || m1.columns != 1){
		printf("Error: Size mismatch when vector dot product with %dx%d and %dx%d. Make sure they are column vectors", m1.rows, m1.columns, m2.rows, m2.columns);
		return 0;
	}
	double d = 0;
	for(size_t i = 0; i < m1.rows; i++)
		d += cmx_get(m1, i, 0) * cmx_get(m2, i, 0);
	return d;
}

/*
 * Gets the vector cross product of the vectors.
 * Currently only supports 3D vectors.
 * Vectors must be of the form nx1.
 * Output equivalent to m3 = m1 x m2
 * cmx_matrix_t m1 - The first vector to cross
 * cmx_matrix_t m2 - The second vector to cross
 */
cmx_matrix_t v_cross(cmx_matrix_t m1, cmx_matrix_t m2){
	if(m1.columns != m2.columns || m1.columns != 1 || m1.rows != m2.rows || m1.rows != 3){
		printf("Error: Cannot cross product a %dx%d and %dx%d. Make sure they are 3D column vectors\n", m1.rows, m1.columns, m2.rows, m2.columns);
		return m1;
	}
	cmx_matrix_t m3 = cmx_make(m1.rows, 1), temp;
	cmx_matrix_t intermediate = cmx_make(3, 3);
	cmx_matrix_t zeros = cmx_make(1, 3);

	temp = cmx_transpose(m1);
	cmx_putr(intermediate, temp, 1);
	cmx_destroy(temp);
	temp = cmx_transpose(m2);
	cmx_putr(intermediate, temp, 2);
	cmx_destroy(temp);

	for(size_t i = 0; i < m1.rows; i++){
		cmx_putr(intermediate, zeros, 0);
		cmx_put(intermediate, 1, 0, i);
		cmx_put(m3, cmx_det(intermediate), i, 0);
	}
	cmx_destroy(zeros);
	cmx_destroy(intermediate);
	return m3;
}

/*
 * Finds the magnitude of the vector
 * Vector must be in the form nx1
 * cmx_matrix_t m - The vector to find the magnitude of
 */
double v_mag(cmx_matrix_t m){
	return cmx_rsqsum(m);
}

/*
 * One of the elementary row operations. Takes a row, and multiplies it by a scalar and adds it back in the matrix
 * cmx_matrix_t m - The matrix to operate on
 * size_t r - The row to manipulate
 * double s - The scalar to multiply by
 */
cmx_matrix_t cmx_eros_scalar(cmx_matrix_t m, size_t r, double s){
	cmx_matrix_t row = cmx_getr(m, r);
	cmx_scalar(row, s);
	cmx_putr(m, row, r);
	cmx_destroy(row);
	//printf("\t\tr%d *= %f\n", s);
	return m;
}

/*
 * One of the elementary row operations.
 * Takes a row, and swaps it with another row
 * cmx_matrix_t m - The matrix to operate on
 * size_t i - One row to swap
 * size_t j - The other row to swap
 */
cmx_matrix_t cmx_eros_swap(cmx_matrix_t m, size_t i, size_t j){
	cmx_matrix_t ri = cmx_getr(m, i);
	cmx_matrix_t rj = cmx_getr(m, j);
	cmx_putr(m, ri, j);
	cmx_putr(m, rj, i);
	cmx_destroy(ri);
	cmx_destroy(rj);
	return m;
}

/*
 * One of the elementary row operations.
 * Takes a row, multiplies it by a scalar, and adds it to another row
 * r = r + s*q
 * cmx_matrix_t m - The matrix to operate on
 * size_t r - The destination row
 * double s - The scalar to multiply by
 * size_t q - The source row
 */
cmx_matrix_t cmx_eros_add(cmx_matrix_t m, size_t r, double s, size_t q){
	cmx_matrix_t row = cmx_getr(m, q);
	cmx_scalar(row, s);
	cmx_matrix_t row2 = cmx_getr(m, r);
	row = cmx_add(row, row2);
	cmx_putr(m, row, r);
	cmx_destroy(row);
	cmx_destroy(row2);
	//printf("\t\tr%d += %f r%d\n", r, s, q);
	return m;
}

/*
 * Takes a matrix and produces a copy in reduced echelon form.
 */
cmx_matrix_t cmx_ref(cmx_matrix_t mo){
	cmx_matrix_t m = cmx_copy(mo);
	double ril, rji;
	size_t rilp;

	for(size_t i = 0; i < m.rows; i++){
		cmx_order_rows(m);
		// Gets row i leader and the column of that leader
		ril = cmx_get_leader(m, i);
		rilp = cmx_get_leader_col(m, i);
		// Skip if zero row or out of bounds
		if(rilp >= m.columns) continue;
		// Modifies the whole row to make the leader 1
		cmx_eros_scalar(m, i, 1.0/ril);
		// Checks every row underneath the leader
		for(size_t j = i; j < m.rows; j++){
			if(j == i) continue;
			// Gets the cell underneath i leader on the j column
			rji = cmx_get(m, j, rilp);
			// If not zero, make it zero
			cmx_eros_add(m, j, -rji, i);
		}
	}
	cmx_order_rows(m);
	return m;
}

/*
 * Takes a matrix and returns the row reduced echelon form 
 * cmx_matrix_t m - The matrix
 */
cmx_matrix_t cmx_rref(cmx_matrix_t m){
	// Put it in reduce echelon form first. Get half the work done
	m = cmx_ref(m);
	double ril, rji;
	size_t rilp;
	
	// For each row, starting at the bottom
	for(size_t i = m.rows-1; i > 0; i--){
		// Get the leading digit and its position
		ril = cmx_get_leader(m, i);
		rilp = cmx_get_leader_col(m, i);
		// Skip if its a zero row or out of bounds
		if(rilp >= m.columns) continue;

		// For all rows above that row
		for(size_t j = i; j >= 0 && j < m.rows; j--){
			// Skip if it is this row
			if(j == i) continue;
			// Get the cell above this one
			rji = cmx_get(m, j, rilp);
			// Get rid of it
			if(rji != 0) cmx_eros_add(m, j, -rji, i);
		}
	}
	return m;
}

/*
 * Matrix multiplies the matrices together in the order given
 * For example:
 * m3 = m1 m2
 * cmx_matrix_t m1 - The first matrix. Transformation to apply
 * cmx_matrix_t m2 - The second matrix. Matrix to aply transformation to
 */
cmx_matrix_t cmx_product(cmx_matrix_t m1, cmx_matrix_t m2){
	cmx_matrix_t m3 = cmx_make(m1.rows, m2.columns);
	if(m1.columns != m2.rows){
		printf("Size mismatch when multiplying matrices together. Given %dx%d and %dx%d\n", m1.rows, m1.columns, m2.rows, m2.columns);
		return m3;
	}
	for(size_t i = 0; i < m2.columns; i++){
		for(size_t j = 0; j < m1.rows; j++){
			double t = 0;
			for(size_t k = 0; k < m2.rows; k++)
				t += cmx_get(m2, k, i) * cmx_get(m1, j, k);
			cmx_put(m3, t, j, i);
		}
	}
	return m3;
}

/*
 * Creates a new matrix which is the transpose of m
 * cmx_matrix_t m - The matrix to be transposed
 */
cmx_matrix_t cmx_transpose(cmx_matrix_t m){
	cmx_matrix_t m1 = cmx_make(m.columns, m.rows);
	for(size_t i = 0; i < m.rows; i++){
		for(size_t j = 0; j < m.columns; j++)
			cmx_put(m1, cmx_get(m, i, j), j, i);	
	}
	return m1;

}

/*
 * gets the determinant of the matrix. Matrix must be square
 * cmx_matrix_t m - The matrix to find the determinant of
 */
double cmx_det(cmx_matrix_t m){
	if(m.rows != m.columns){
		printf("Error: Can't find the determinant of a non-square %dx%d matrix!", m.rows, m.columns);
		return 0;
	}
	double d = 0;
	if(m.rows == 1) d = cmx_get(m, 0, 0);
	else {
		cmx_matrix_t minor_temp;
		for(size_t i = 0; i < m.columns; i++){
			if(cmx_get(m, 0, i) == 0) continue;
			minor_temp = cmx_minor(m, 0, i);
			d += pow(-1.0, i) * cmx_get(m, 0, i) * cmx_det(minor_temp);
			cmx_destroy(minor_temp);
		}
	}
	return d;
}

/*
 * Produces a copy of the matrix's inverse
 * cmx_matrix_t m - The matrix to get the inverse of
 */
cmx_matrix_t cmx_inverse(cmx_matrix_t m){
	if(m.rows != m.columns){
		printf("ERROR: canot find inverse of %dx%d matrix. Matrix not square.\n", m.rows, m.columns);
		return m;
	}
	double det = cmx_det(m);
	if(det == 0){
		printf("ERROR: Determinant of matrix zero, cannot find inverse.\n");
		return m;
	}
	cmx_matrix_t minor_temp, inverse, C = cmx_copy(m);
	for(size_t i = 0; i < m.rows; i++){
		for(size_t j = 0; j < m.columns; j++){
				minor_temp = cmx_minor(m, i, j);
				cmx_put(C, cmx_det(minor_temp) * ( (i+j)%2? -1: 1), i, j);
				cmx_destroy(minor_temp);
		}
	}
	
	inverse = cmx_transpose(C);
	cmx_destroy(C);
	cmx_scalar(inverse, 1/det);
	return inverse;

}

/*
 * Gets the value in the matrix cell (r, c)
 * cmx_matrix_t matrix - The matrix to get the information from
 * size_t r - The row to get the cell from
 * size_t c - The column to get the cell from
 */
double cmx_get(cmx_matrix_t matrix, size_t r, size_t c){
	if(r >= matrix.rows || c >= matrix.columns){
		printf("Index out of bounds error accessing matrix at (%d,%d). Matrix has size %dx%d\n", r, c, matrix.rows, matrix.columns);
		return 0;
	}
	return matrix.data[ r * matrix.columns + c ];
}

/*
 * Puts the value, double d into the cell specified
 * cmx_matrix_t m - The matrix to put the value in
 * double d - The value to put in the cell
 * size_t r - The index of the row 
 * size_t c - The index of the column
 */
cmx_matrix_t cmx_put(cmx_matrix_t m, double d, size_t r, size_t c){
	if(r >= m.rows || c >= m.columns){
		printf("Index out of bounds error accessing matrix at (%d,%d). Matrix has size %dx%d\n", r, c, m.rows, m.columns);
		return m;
	}
	m.data[r*m.columns + c] = d;
	return m;
}

/*
 * Gets the row specified and returns it as a tuple
 * cmx_matrix_t m - The matrix to retrieve the row from
 * size_t r - The row to collect
 */
cmx_matrix_t cmx_getr(cmx_matrix_t m, size_t r){
	cmx_matrix_t row = cmx_make(1, m.columns);
	if(r >= m.rows){
		printf("Index out of bounds error when retrieving matrix row %d from %dx%d matrix\n", r, m.rows, m.columns);
		return row;
	}
	for(size_t i = 0; i < m.columns; i++)
		cmx_put(row, cmx_get(m, r, i), 0, i);
	return row;
}

/*
 * Puts the specified row in the matrix
 * cmx_matrix_t m - The matrix to put the row in
 * cmx_matrix_t row - The row to insert
 * size_t r - The index of the row to insert
 */
cmx_matrix_t cmx_putr(cmx_matrix_t m, cmx_matrix_t row, size_t r){
	if(r >= m.rows || row.rows != 1 || row.columns != m.columns){
		printf("Error putting row in matrix. m: %dx%d r: %dx%d, at %d", m.rows, m.columns, row.rows, row.columns, r);
		return m;
	}
	for(size_t i = 0; i < m.columns; i++)
		cmx_put(m, cmx_get(row, 0, i), r, i);
	return m;

}

/*
 * Creates a copy of the matrix with the row deleted
 * cmx_matrix_t m - The matrix to delete a row from
 * size_t r - The row to delete
 */
cmx_matrix_t cmx_delr(cmx_matrix_t m, size_t r){
	if(m.rows <= 1){
		printf("Error: Cannot delete a row from a matrix which has only one row!\n");
		return m;
	}
	cmx_matrix_t m1 = cmx_make(m.rows-1, m.columns);
	cmx_matrix_t rowt;
	int f = 0;
	for(size_t i = 0; i < m.rows; i++){
		if(i==r){
			f = 1;
			continue;
		}
		rowt = cmx_getr(m, i);
		cmx_putr(m1, rowt, (f)?(i-1):i);
		cmx_destroy(rowt);
	}
	return m1;

}

/*
 * Creates a copy of the matrix with the column deleted
 * cmx_matrix_t m - The matrix to delete a column from
 * size_t c - The column to delete
 */
cmx_matrix_t cmx_delc(cmx_matrix_t m, size_t c){
	if(m.columns <= 1){
		printf("Error: Cannot delete a column from a matrix which has only one column!\n");
		return m;
	}
	cmx_matrix_t m2 = cmx_transpose(m);
	cmx_matrix_t m1 = cmx_delr(m2, c);
	cmx_matrix_t m3 = cmx_transpose(m1);
	cmx_destroy(m2);
	cmx_destroy(m1);
	return m3;

}

/*
 * Creates a matrix of minors by deleting a single row and column
 * cmx_matrix_t m - The matrix to make a minor out of
 * size_t r - The row to delete
 * size_t c - The column to delete
 */
cmx_matrix_t cmx_minor(cmx_matrix_t m, size_t r, size_t c){
	if(r >= m.rows || c >= m.columns){
		printf("Error: Index out of bounds, cannot make a minor of matrix %dx%d by deleting %d,%d\n", m.rows, m.columns, r, c);
		return m;
	}
	cmx_matrix_t m1 = cmx_delr(m, r);
	cmx_matrix_t m2 = cmx_delc(m1, c);
	cmx_destroy(m1);
	return m2;
}

/*
 * Gets the leading entry of a row of a matrix from a given row
 * cmx_matrix_t m - The matrix
 * size_t r - The row to find the leading entry of
 */
double cmx_get_leader(cmx_matrix_t m, size_t r){
	cmx_matrix_t row = cmx_getr(m, r);
	double l = 0;
	for(size_t i = 0; i < m.columns; i++){
		l = cmx_get(row, 0, i);
		if(l != 0) break;
	}
	cmx_destroy(row);
	return l;

}

/*
 * gets the column of the leading entry in a matrix, on a given row
 * cmx_matrix_t m - The matrix to use
 * size_t r - The row of the leading entry
 */
size_t cmx_get_leader_col(cmx_matrix_t m, size_t r){
	size_t c = 0;
	cmx_matrix_t row = cmx_getr(m, r);
	double l = 0;
	for(size_t i = 0; i < m.columns; i++){
		l = cmx_get(row, 0, i);
		if(l != 0) break;
		c++;
	}
	cmx_destroy(row);
	return c;
}

/*
 * Puts rows in staircase pattern. Zero rows at bottom and 
 * cmx_matrix_t m - The matrix to order
 */
cmx_matrix_t cmx_order_rows(cmx_matrix_t m){
	size_t hlp=0, hlr, bottom=m.rows-1, lp;

	// For each row
	for(size_t j = 0; j < m.rows; j++){
		hlp = 0;

		// For each row that isn't in place
		// Fill from the bottom
		for(size_t i = bottom; i >= 0 && i < m.rows; i--){
			// Where is the leading digit
			lp = cmx_get_leader_col(m, i);
			// If its further right than the highest
			if(lp >= hlp){
				// Set that as current highest and save which row
				hlp = lp;
				hlr = i;
			}
		}
		// Once checked each row above, put the highest row on the 'stack'
		cmx_eros_swap(m, hlr, bottom);
		// Move the 'stack pointer' up
		bottom--;
	}

	return m;
}

/*
 * Puts every zero row on the bottom of the matrix
 * cmx_matrix_t m - The matrix to manipulate
 */
cmx_matrix_t cmx_shift_zeros(cmx_matrix_t m){
	size_t last = m.rows-1;
	size_t lp;
	for(size_t i = 0; i < last; i++){
		lp = cmx_get_leader_col(m, i);
		if(lp >= m.columns){
			cmx_eros_swap(m, i, last);
			last--;
		}
	}
	return m;
}

/*
 * Takes a matrix and replaces its contents with 'noise', random double between 0 and 1
 * MAKE SURE TO CALL srand(time(NULL)) BEFORE USING cmx_matrix_t m - The matrix to 'noise'
 */
cmx_matrix_t cmx_noise(cmx_matrix_t m){
	for(size_t i = 0; i < m.rows*m.columns; i++){
		m.data[i] = (double)rand()/RAND_MAX;
	}
	return m;
}

/*
 * Gets the algebraic sum of the contents
 * cmx_matrix_t m - The matrix to find the sum of
 */
double cmx_sum(cmx_matrix_t m){
	double s = 0;
	for(size_t i = 0; i < m.rows*m.columns; i++){
		s += m.data[i];
	}
	return s;
}

/*
 * Takes a matrix and returns the algebraic mean of the contents
 * cmx_matrix_t m - The matrix to find the mean of
 */
double cmx_mean(cmx_matrix_t m){
	return cmx_sum(m)/(m.rows*m.columns);
}

/*
 * Takes a matris and returns the sum of the squares of the cell
 * cmx_matrix_t m - The matrix to find the square sum of
 */
double cmx_sqsum(cmx_matrix_t m){
	double s = 0;
	for(size_t i = 0; i < m.rows*m.columns; i++)
		s += pow(m.data[i], 2);
	return s;
}

/*
 * Takes the root of the smu of squared values in the matrix
 * sqrt(m11^2 + m21^2 + ... + mpq^2)
 * cmx_matrix_t m - The matrix to operate on
 */
double cmx_rsqsum(cmx_matrix_t m){
	return pow(cmx_sqsum(m), 0.5);
}


/*
 * Takes the root mean square of the matrix
 * cmx_matrix_t m - The matrix to take the rms of
 */
double cmx_rms(cmx_matrix_t m){
	return pow(cmx_sqsum(m)/(m.rows*m.columns), 0.5);
}

/*
 * Prints a matrix in formatted form
 * cmx_matrix_t matrix - The matrix to be printed
 */
void cmx_printf(cmx_matrix_t matrix){
	printf("\n");
	for(size_t r = 0; r < matrix.rows; r++){
		for(size_t c = 0; c < matrix.columns; c++){
			if(cmx_get(matrix, r, c) >= 0)
				printf(" ");
			printf("%f\t", cmx_get(matrix, r, c));
		}
		printf("\n");
	}
	printf("\n");
}

/*
 * Stores an array of matrices in a given file. Can write or append.
 * cmx_matrix *m - The pointer of the array to store
 * size_t length - The length of the array to store
 * char* fname - The file name to store in. Matrix is stored in binary.
 * char mode - 'w'rite or 'a'ppend to the file
 */
void cmx_store_file(cmx_matrix_t *m, size_t length, char* fname, char mode){
	FILE *f = NULL;

	if(mode == 'w')
		f = fopen(fname, "wb");
	else if(mode == 'a')
		f = fopen(fname, "ab");
	else
		printf("Error opening \'%s\'. Write mode \'%c\'not recognised", fname, mode);

	if(f == NULL){
		printf("Error opening \'%s\' to write to. Aborting.\n", fname);
		fclose(f);
		return;
	}

	for(size_t i = 0; i < length; i++){
	
		fwrite( &(m[i].rows), sizeof(size_t), 1, f );
		fwrite( &(m[i].columns), sizeof(size_t), 1, f );

		for(size_t j = 0; j < m[i].rows*m[i].columns; j++){
			fwrite( (m[i]).data + j, sizeof(double), 1, f );
		}
	}

	fclose(f);
}


/*
 * Stores a singular matrix in the file specified.
 * Can either write over with 'w' or append with 'a'
 * cmx_matrix_m - The matrix to be stored
 * char* fname - The file to be stored in
 * char mode - 'w'rite or 'a'ppend to the file
 */
void cmx_store_matrix(cmx_matrix_t m, char* fname, char mode){
	cmx_store_file(&m, 1, fname, mode);
}

/*
 * Loads matrices in bulk from a file and returns them in an array of length l
 * char* fname - The file name to read from
 * size_t l - The number of matrices to read from t he file
 */
cmx_matrix_t* cmx_load_file(char* fname, size_t l){
	FILE *f = NULL;
	f = fopen(fname, "rb");
	if(f == NULL){
		printf("Error opening file to read from. Aborting.\n");
		fclose(f);
		return NULL;
	}
	printf("");
	size_t rows, columns;
	cmx_matrix_t *ms = (cmx_matrix_t*)malloc(sizeof(cmx_matrix_t)*l);
	//cmx_matrix_t ms[l];
	for(size_t j = 0; j < l; j++){
		fread( &rows, sizeof(size_t), 1, f );
		fread( &columns, sizeof(size_t), 1, f );
	
		ms[j] = cmx_make(rows, columns);
		for(size_t i = 0; i < ms[j].rows*ms[j].columns; i++){
			fread( (ms[j].data+i), sizeof(double), 1, f );
		}
	}
	fclose(f);
	return ms;
}

/*
 * Reads a matrix from a file. Reads only the first one if multiple are stored in there
 * char* fname - The file name to read from
 */
cmx_matrix_t cmx_load_matrix(char* fname){
	cmx_matrix_t *ms = cmx_load_file(fname, 1);
	cmx_matrix_t m = ms[0];
	free(ms);
	return m;
}


