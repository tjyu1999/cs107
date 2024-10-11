/* Sierpinski triangle:
 *
 * outputs an ascii representation of the Sierpinski triangle
 * the works due to the relationship between the fractal and Pascal's triangle
 */

#include <stdio.h>
#include <stdlib.h>

void print_triangle(int n) {
    int size = 1 << n;
    
    for (int row = size - 1; row >= 0; --row) {
        for (int col = 0; col < row; ++col)
            printf(" ");
        for (int col = 0; col + row < size; ++col)
            printf("%c ", (col & row) ? ' ' : '*');  // generating patterns by using a bitwise AND
            
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int n = 3;
    
    if (argc > 1)
        n = atoi(argv[1]);
    if (n > 8 || n < 0) {
        fprintf(stderr, "Illegal value %d. Level must be in range[0, 8].\n", n);
        exit(1);
    }
    
    print_triangle(n);
    
    return 0;
}
