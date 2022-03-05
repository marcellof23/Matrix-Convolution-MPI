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

    /* Master MPI Variables */
    Matrix kernel_matrix;
    Matrix *arr_mat;
    int *arr_range;
    int median, floored_mean;

    int *send_counts; /* The number of chunk every process receives */
    int *displacements; /* The displacement offset where each chunk segment begins */
    int remainder; /* Remainder of num_targets/world_size */


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    /* create type for struct matrix */
    const int nitems=3;
    int blocklengths[3] = {NMAX * NMAX, 1 ,1};
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype MPI_MATRIX;
    MPI_Aint offsets[3];

    offsets[0] = offsetof(Matrix, mat);
    offsets[1] = offsetof(Matrix, row_eff);
    offsets[2] = offsetof(Matrix, col_eff);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &MPI_MATRIX);
    MPI_Type_commit(&MPI_MATRIX);

    send_counts = (int*)malloc(world_size * sizeof(int));
    displacements = (int*)malloc(world_size * sizeof(int));

    if(world_rank == BROADCASTER_RANK){
        start_timer();
        /* reads kernel's row and column and initalize kernel matrix from input */
        scanf("%d %d", &kernel_row, &kernel_col);
        kernel_matrix = input_matrix(kernel_row, kernel_col);
        
        /* reads number of target matrices and their dimensions.
        initialize array of matrices and array of data ranges (int) */

        scanf("%d %d %d", &num_targets, &target_row, &target_col);
        arr_mat = (Matrix*)malloc(num_targets * sizeof(Matrix));
        arr_range = (int*)malloc(num_targets * sizeof(int));
        assert(arr_mat != NULL);
        assert(arr_range != NULL);

        /* read each target matrix */
        for(int i = 0; i < num_targets; i++){
            arr_mat[i] = input_matrix(target_row, target_col);
        }

        /* calculate send_counts and displacements */
        int base_target_count_per_process = num_targets / world_size;
        int offset = 0;
        remainder = num_targets % world_size;
        for(int i = 0; i < world_size; i++){
            send_counts[i] = base_target_count_per_process;
            if(remainder > 0){
                send_counts[i]++;
                remainder--;
            }
            displacements[i] = offset;
            offset+=send_counts[i];
        }
        
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

    debug(world_rank, "hello (p = %d)\n", world_size);

    MPI_Bcast(&kernel_matrix, 1, MPI_MATRIX, BROADCASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(send_counts, world_size, MPI_INT, BROADCASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(displacements, world_size, MPI_INT, BROADCASTER_RANK, MPI_COMM_WORLD);

    if(world_rank != BROADCASTER_RANK) {
        debug(world_rank, "received kernel matrix from %d\n", BROADCASTER_RANK);
        debug(world_rank, "kernel matrix size %d %d\n", kernel_matrix.row_eff, kernel_matrix.col_eff);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    debug(world_rank, "num chunks %d\n", send_counts[world_rank]);

    if(send_counts[world_rank] == 0){
        return 0;
    }

    Matrix *sub_arr_mat = (Matrix *)malloc(send_counts[world_rank] * sizeof(Matrix));
    int *sub_arr_range = malloc(send_counts[world_rank] * sizeof(int));
    int sub_median, sub_floored_mean;

    MPI_Scatterv(arr_mat, send_counts, displacements, MPI_MATRIX, sub_arr_mat, send_counts[world_rank], MPI_MATRIX, BROADCASTER_RANK, MPI_COMM_WORLD);

    debug(world_rank, "input matrix size [%d][%d]\n", sub_arr_mat[0].col_eff, sub_arr_mat[0].row_eff);

    /*  compute their convolution matrices, and compute their data ranges */
    for(int i = 0; i < send_counts[world_rank]; i++){
        sub_arr_mat[i] = convolution(&kernel_matrix, &sub_arr_mat[i]);
        sub_arr_range[i] = get_matrix_datarange(&sub_arr_mat[i]); 
    }

    sub_median = get_median(sub_arr_range, send_counts[world_rank]);
    sub_floored_mean = get_floored_mean(sub_arr_range, send_counts[world_rank]); 

    debug(world_rank, "Median %d Floored Mean %d\n",sub_median, sub_floored_mean);

    debug(world_rank, "goodbye \n");

    int *sub_avg;


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

        free(arr_mat);
        free(arr_range);
    }

    return 0;
}