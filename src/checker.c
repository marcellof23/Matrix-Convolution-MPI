#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int parallel_min, serial_min, parallel_max, serial_max, parallel_median, serial_median, parallel_average, serial_average;
    FILE *parallel_infile, *serial_infile;

    fprintf(stdout, "\033[1;32m");
    fprintf(stderr, "\033[1;31m");

    /* TEST CASE 1 */

    parallel_infile = fopen("result/K02-01-TC1_parallel.txt", "r");
    serial_infile = fopen("result/K02-01-TC1_serial.txt", "r");

    fscanf(parallel_infile, "%d %d %d %d", &parallel_min, &parallel_max, &parallel_median, &parallel_average);
    fscanf(serial_infile, "%d %d %d %d", &serial_min, &serial_max, &serial_median, &serial_average);

    if(parallel_min != serial_min || parallel_max != serial_max || parallel_median != serial_median || parallel_average != serial_average){
        fprintf(stderr, "TC 1 FAILED\n");
    } else{
        fprintf(stdout, "TC 1 PASSED\n");
    }

    /* TEST CASE 2 */

    parallel_infile = fopen("result/K02-01-TC2_parallel.txt", "r");
    serial_infile = fopen("result/K02-01-TC2_serial.txt", "r");

    fscanf(parallel_infile, "%d %d %d %d", &parallel_min, &parallel_max, &parallel_median, &parallel_average);
    fscanf(serial_infile, "%d %d %d %d", &serial_min, &serial_max, &serial_median, &serial_average);

    if(parallel_min != serial_min || parallel_max != serial_max || parallel_median != serial_median || parallel_average != serial_average){
        fprintf(stderr, "TC 2 FAILED\n");
    } else{
        fprintf(stdout, "TC 2 PASSED\n");
    }

    /* TEST CASE 3 */

    parallel_infile = fopen("result/K02-01-TC3_parallel.txt", "r");
    serial_infile = fopen("result/K02-01-TC3_serial.txt", "r");

    fscanf(parallel_infile, "%d %d %d %d", &parallel_min, &parallel_max, &parallel_median, &parallel_average);
    fscanf(serial_infile, "%d %d %d %d", &serial_min, &serial_max, &serial_median, &serial_average);

    if(parallel_min != serial_min || parallel_max != serial_max || parallel_median != serial_median || parallel_average != serial_average){
        fprintf(stderr, "TC 3 FAILED\n");
    } else{
        fprintf(stdout, "TC 3 PASSED\n");
    }

    /* TEST CASE 4 */

    parallel_infile = fopen("result/K02-01-TC4_parallel.txt", "r");
    serial_infile = fopen("result/K02-01-TC4_serial.txt", "r");

    fscanf(parallel_infile, "%d %d %d %d", &parallel_min, &parallel_max, &parallel_median, &parallel_average);
    fscanf(serial_infile, "%d %d %d %d", &serial_min, &serial_max, &serial_median, &serial_average);

    if(parallel_min != serial_min || parallel_max != serial_max || parallel_median != serial_median || parallel_average != serial_average){
        fprintf(stderr, "TC 4 FAILED\n");
    } else{
        fprintf(stdout, "TC 4 PASSED\n");
    }

    return 0;
}