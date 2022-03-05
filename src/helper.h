#ifndef HELPER_H
#define HELPER_H

struct timespec start, current;
double elapsed;

void start_timer();

void get_timer();

void debug(int rank, char *format, ...);

#endif