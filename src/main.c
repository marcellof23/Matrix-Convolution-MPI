#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

int main(int argc, char *argv[]){
  struct timespec start, finish;
  double elapsed;
 
  clock_gettime(CLOCK_MONOTONIC, &start);


  int kernel_row, kernel_col, target_row, target_col, num_targets;
	
	// reads kernel's row and column and initalize kernel matrix from input
	scanf("%d %d", &kernel_row, &kernel_col);
	Matrix kernel = input_matrix(kernel_row, kernel_col);
	
	// reads number of target matrices and their dimensions.
	// initialize array of matrices and array of data ranges (int)
	scanf("%d %d %d", &num_targets, &target_row, &target_col);
	Matrix* arr_mat = (Matrix*)malloc(num_targets * sizeof(Matrix));
	int arr_range[num_targets];
	
	// read each target matrix, compute their convolution matrices, and compute their data ranges
  for (int i = 0; i < num_targets; i++) {
    arr_mat[i] = input_matrix(target_row, target_col);
    arr_mat[i] = convolution(&kernel, &arr_mat[i]);
    arr_range[i] = get_matrix_datarange(&arr_mat[i]); 
  }


	// sort the data range array
	merge_sort(arr_range, 0, num_targets - 1);
	
	int median = get_median(arr_range, num_targets);	
	int floored_mean = get_floored_mean(arr_range, num_targets); 

  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  // stores the output in result/<tc_name>_<serial/parallel>.txt
  if(argc > 1){
    FILE *outfile;
    char filename[80];
    sprintf(filename, "result/%s_parallel.txt", argv[1]);
    outfile = fopen(filename, "w");
    fprintf(outfile,"%d\n%d\n%d\n%d\n", 
    arr_range[0], 
    arr_range[num_targets - 1], 
    median, 
    floored_mean);
  }


	printf("%d\n%d\n%d\n%d\n", 
			arr_range[0], 
			arr_range[num_targets - 1], 
			median, 
			floored_mean);

  printf("The elapsed time is %f seconds\n", elapsed);

	return 0;
}