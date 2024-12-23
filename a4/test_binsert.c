#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "binsert.h"

int cmp_int(const void *p, const void *q) {
    return  *(const int *) p - *(const int *) q;
}

int main(int argc, char *argv[]) {
    int arr[argc];
    size_t nused = 0;

    for (int i = 1; i < argc; ++i) {
        int val = atoi(argv[i]);        
        int *added = binsert(&val, arr, &nused, sizeof(arr[0]), cmp_int);
        assert(val == *added);
    }

    for (int i = 0; i < nused; ++i)
        printf(" %d", arr[i]);
    printf("\n");    
    
    return 0;
}
