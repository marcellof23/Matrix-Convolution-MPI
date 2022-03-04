#ifndef UTILS_H
#define UTILS_H

/*
 * Procedure merge_array
 *
 * Merges two subarrays of n with n[left..mid] and n[mid+1..right]
 * to n itself, with n now ordered ascendingly
 * */
void merge_array(int *n, int left, int mid, int right);


/* 
 * Procedure merge_sort
 *
 * Sorts array n with merge sort algorithm
 * */
void merge_sort(int *n, int left, int right);


/* 
 * Procedure print_array
 *
 * Prints all elements of array n of size to stdout
 * */
void print_array(int *n, int size);


/* 
 * Function get_median
 *
 * Returns median of array n of length
 * */
int get_median(int *n, int length);


/* 
 * Function get_floored_mean
 *
 * Returns floored mean from an array of integers
 * */
long get_floored_mean(int *n, int length);

#endif