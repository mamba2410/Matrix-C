# Matrix-C
CMX - A matrix library written in C.

This is just for fun and is no way intended to be used for large projects.

Matrix is a C struct with attributes rows, columns, length and data.

rows is the number of rows, and similarly for columns.
length is the size of the data array, and is simply rows\*columns.
data is the pointer to an array of doubles on the heap.

None of these attributes can be modified once initialised, however the contents of the data array can.

