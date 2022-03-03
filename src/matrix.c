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
	int max = DATAMIN;
	int min = DATAMAX;
    for (int i = 0; i < m->row_eff; i++) {
      for (int j = 0; j < m->col_eff; j++) {
        int el = m->mat[i][j];
        if (el > max) max = el;
        if (el < min) min = el;
      }
    }

	return max - min;
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

  for (int i = 0; i < out.row_eff; i++) {
    for (int j = 0; j < out.col_eff; j++) {
      out.mat[i][j] = supression_op(kernel, target, i, j);
    }
  }

	return out;
}


void merge_array(int *n, int left, int mid, int right) {
	int n_left = mid - left + 1;
	int n_right = right - mid;
	int iter_left = 0, iter_right = 0, iter_merged = left;
	int arr_left[n_left], arr_right[n_right];

	for (int i = 0; i < n_left; i++) {
		arr_left[i] = n[i + left];
	}

	for (int i = 0; i < n_right; i++) {
		arr_right[i] = n[i + mid + 1];
	}

	while (iter_left < n_left && iter_right < n_right) {
		if (arr_left[iter_left] <= arr_right[iter_right]) {
			n[iter_merged] = arr_left[iter_left++];
		} else {
			n[iter_merged] = arr_right[iter_right++];
		}
		iter_merged++;
	}

	while (iter_left < n_left)  {
		n[iter_merged++] = arr_left[iter_left++];
	}
	while (iter_right < n_right) {
		n[iter_merged++] = arr_right[iter_right++];
	} 
}


void merge_sort(int *n, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		merge_sort(n, left, mid);
		merge_sort(n, mid + 1, right);

		merge_array(n, left, mid, right);
	}	
}
 

void print_array(int *n, int size) {
	for (int i = 0; i < size; i++ ) printf("%d ", n[i]);
	printf("\n");
}


int get_median(int *n, int length) {
	int mid = length / 2;
	if (length & 1) return n[mid];

	return (n[mid - 1] + n[mid]) / 2;
}


long get_floored_mean(int *n, int length) {
	long sum = 0;
	for (int i = 0; i < length; i++) {
		sum += n[i];
	}

	return sum / length;
}