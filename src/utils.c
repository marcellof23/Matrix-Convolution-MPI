#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"

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

	return length > 0 ? sum / length : 0;
}

/*
    I.S. unsorted_arr and sorted_arr memory is allocated with the same size
*/
void merge_k_sorted_arrays(int *send_counts, int k, int *unsorted_arr, int *sorted_arr, int num_targets){
    int *offsets;
    int *offsets_limit;
    offsets = (int*)malloc(k * sizeof(int));
    offsets_limit = (int*)malloc(k * sizeof(int));
    assert(offsets != NULL);
    assert(offsets_limit != NULL);

    int sum_offset = 0;
    for(int i = 0; i < k; i++){
        offsets[i] = sum_offset;
        offsets_limit[i] = sum_offset + send_counts[i];
        sum_offset += send_counts[i];
    }

    // for(int i = 0; i < k ; i++){
    //     printf("offset[%d]: %d, offset_limit[%d]: %d\n", i, offsets[i], i, offsets_limit[i]);
    // }

    int partition_index = 0;

    for(int i = 0; i < num_targets; i++){
        for(int j = 0 ; j < k; j++){
            if(offsets[j] < offsets_limit[j] && unsorted_arr[offsets[j]] < unsorted_arr[offsets[partition_index]]){
                partition_index = j;
            }
        }
        // printf("sorted_arr[%d] = %d with real index of %d\n", i, unsorted_arr[offsets[partition_index]], offsets[partition_index]);
        sorted_arr[i] = unsorted_arr[offsets[partition_index]];
        offsets[partition_index]++;
        while(i != num_targets-1 && offsets[partition_index] >= offsets_limit[partition_index]){
            partition_index = (partition_index + 1) % k;
        }
    }
}

// [0,4,8,12] -> 0 ~ 15 ada 16 elements
// Misal minimum milik partisi ke 2
// offset menjadi [0,5,8,12]
// misal kasus partisi ke 2 memang kecil semua menjadi
// [0,8,8,12] -> 8 harus kita ignore