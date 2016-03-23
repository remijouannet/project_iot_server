#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "mem_info.h"

double getMemoryUsage(){
    FILE* file;
    unsigned long long memtotal, memfree, memavailable, buffers, cached;

    file = fopen("/proc/meminfo", "r");
    fscanf(file, "MemTotal: %llu kB MemFree: %llu kB MemAvailable: %llu kB Buffers: %llu kB Cached: %llu kB",
           &memtotal, &memfree, &memavailable, &buffers, &cached);
    fclose(file);

    double result = (memtotal - (memfree + buffers + cached));

    return result;
}

double getMemoryTotal(){
    FILE* file;
    unsigned long long memtotal;
    file = fopen("/proc/meminfo", "r");
    fscanf(file, "MemTotal: %llu kB",&memtotal);
    fclose(file);
    return memtotal;
}
