#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NMAX 100
#define DATAMAX 1000
#define DATAMIN -1000
#define THREADS_COUNT 8

/* 
 * Struct Matrix
 *
 * Matrix representation consists of matrix data 
 * and effective dimensions 
 * */
typedef struct Matrix {
	int mat[NMAX][NMAX];	// Matrix cells
	int row_eff;			// Matrix effective row
	int col_eff;			// Matrix effective column
} Matrix;


/* 
 * Procedure init_matrix
 * 
 * Initializing newly allocated matrix
 * Setting all data to 0 and effective dimensions according
 * to nrow and ncol 
 * */
void init_matrix(Matrix *m, int nrow, int ncol);


/* 
 * Function input_matrix
 *
 * Returns a matrix with values from stdin input
 * */
Matrix input_matrix(int nrow, int ncol);


/* 
 * Procedure print_matrix
 * 
 * Print matrix data
 * */
void print_matrix(Matrix *m);


/* 
 * Function get_matrix_datarange
 *
 * Returns the range between maximum and minimum
 * element of a matrix
 * */
int get_matrix_datarange(Matrix *m);


/*
 * Function supression_op
 *
 * Returns the sum of intermediate value of special multiplication
 * operation where kernel[0][0] corresponds to target[row][col]
 * */
int supression_op(Matrix *kernel, Matrix *target, int row, int col);


/* 
 * Function convolution
 *
 * Return the output matrix of convolution operation
 * between kernel and target
 * */
Matrix convolution(Matrix *kernel, Matrix *target);

#endif