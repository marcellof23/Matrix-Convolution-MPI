#include "matrix.h"

void init_matrix(Matrix *m, int nrow, int ncol) {
	m->row_eff = nrow;
	m->col_eff = ncol;

  for (int i = 0; i < m->row_eff; i++) {
    for (int j = 0; j < m->col_eff; j++) {
      m->mat[i][j] = 0;
    }
  }

}


Matrix input_matrix(int nrow, int ncol) {
	Matrix input;
	init_matrix(&input, nrow, ncol);

    int i, j;

	for (i = 0; i < nrow; i++) {
    for (j = 0; j < ncol; j++) {
      scanf("%d", &input.mat[i][j]);
    }
	}

	return input;
}


void print_matrix(Matrix *m) {
	for (int i = 0; i < m->row_eff; i++) {
		for (int j = 0; j < m->col_eff; j++) {
			printf("%d ", m->mat[i][j]);
		}
		printf("\n");
	}
}


int get_matrix_datarange(Matrix *m) {
	int smax = DATAMIN;
	int smin = DATAMAX;

  #pragma omp parallel for reduction(max:smax) reduction(min:smin) num_threads(THREADS_COUNT) 
  for (int i = 0; i < m->row_eff; i++) {
    for (int j = 0; j < m->col_eff; j++) {
      int el = m->mat[i][j];
      smax = el > smax ? el : smax;
      smin = el < smin ? el : smin;
    }
  }

	return smax - smin;
}


int supression_op(Matrix *kernel, Matrix *target, int row, int col) {
	int intermediate_sum = 0;

  for (int i = 0; i < kernel->row_eff; i++) {
    for (int j = 0; j < kernel->col_eff; j++) {
      intermediate_sum += kernel->mat[i][j] * target->mat[row + i][col + j];
    }
  }

	return intermediate_sum;
}


Matrix convolution(Matrix *kernel, Matrix *target) {
	Matrix out;
	int out_row_eff = target->row_eff - kernel->row_eff + 1;
	int out_col_eff = target->col_eff - kernel->col_eff + 1;
	
	init_matrix(&out, out_row_eff, out_col_eff);

  #pragma omp parallel for num_threads(THREADS_COUNT)
  for (int i = 0; i < out.row_eff; i++) {
    for (int j = 0; j < out.col_eff; j++) {
      out.mat[i][j] = supression_op(kernel, target, i, j);
    }
  }

	return out;
}