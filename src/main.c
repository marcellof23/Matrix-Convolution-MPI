#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include "matrix.h"
#include "utils.h"
#include "helper.h"

int main(int argc, char *argv[]){
    int kernel_row, kernel_col, target_row, target_col, num_targets;
    int world_size, world_rank;

    Matrix kernel_matrix;
    Matrix *arr_mat;
    int median, floored_mean;
    int *arr_range;
 


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    /* create type for struct matrix */
    const int nitems=3;
    int blocklengths[3] = {NMAX * NMAX, 1 ,1};
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype mpi_matrix;
    MPI_Aint offsets[3];

    offsets[0] = offsetof(Matrix, mat);
    offsets[1] = offsetof(Matrix, row_eff);
    offsets[2] = offsetof(Matrix, col_eff);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_matrix);
    MPI_Type_commit(&mpi_matrix);

    if(world_rank == 0){
        start_timer();
        /* reads kernel's row and column and initalize kernel matrix from input */
        scanf("%d %d", &kernel_row, &kernel_col);
        kernel_matrix = input_matrix(kernel_row, kernel_col);
        
        /* reads number of target matrices and their dimensions.
        initialize array of matrices and array of data ranges (int) */

        scanf("%d %d %d", &num_targets, &target_row, &target_col);
        // arr_mat = (Matrix*)malloc(num_targets * sizeof(Matrix));
        // arr_range = malloc(num_targets * sizeof *arr_range);
        // assert(arr_mat != NULL);
        // assert(arr_range != NULL);
        
        /* read each target matrix, compute their convolution matrices, and compute their data ranges */
        // for (int i = 0; i < num_targets; i++) {
        //   arr_mat[i] = input_matrix(target_row, target_col);
        //   arr_mat[i] = convolution(&kernel_matrix, &arr_mat[i]);
        //   arr_range[i] = get_matrix_datarange(&arr_mat[i]); 
        // }

        /* sort the data range array */
        // merge_sort(arr_range, 0, num_targets - 1);
        
        // median = get_median(arr_range, num_targets);	
        // floored_mean = get_floored_mean(arr_range, num_targets); 
    }
        
    // MPI_BCast(kernel_matrix)

    debug(world_rank, "hello (p = %d)\n", world_size);

    MPI_Bcast(&kernel_matrix, 1, mpi_matrix, BROADCASTER_RANK, MPI_COMM_WORLD);

    if(world_rank != BROADCASTER_RANK) {
        debug(world_rank, "received matrix from %d\n", BROADCASTER_RANK);
        debug(world_rank, "matrix size %d %d\n", kernel_matrix.row_eff, kernel_matrix.col_eff);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    debug(world_rank, "goodbye \n");



    MPI_Finalize();

    if(world_rank == 0){
        get_timer();

        /* stores the output in result/<tc_name>_<serial/parallel>.txt */
        // if(argc > 1){
        //   FILE *outfile;
        //   char filename[80];
        //   sprintf(filename, "result/%s_parallel.txt", argv[1]);
        //   outfile = fopen(filename, "w");
        //   fprintf(outfile,"%d\n%d\n%d\n%d\n", 
        //   arr_range[0], 
        //   arr_range[num_targets - 1], 
        //   median, 
        //   floored_mean);
        // }


        // printf("%d\n%d\n%d\n%d\n", 
        // 		arr_range[0], 
        // 		arr_range[num_targets - 1], 
        // 		median, 
        // 		floored_mean);
        printf("The elapsed time is %f seconds\n", elapsed);
    }

    return 0;
}