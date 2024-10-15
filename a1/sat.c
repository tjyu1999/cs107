/* saturating addition:
 *
 * return the type's maximum value while positive overflow and minimum while negative overflow
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

long signed_min(int bitwidth) {
    return ~0L << (bitwidth - 1); // return min of 2's complement integer of bitwidth
}

long signed_max(int bitwidth) {
    return (1L << (bitwidth - 1)) - 1; // return max of 2's complement integer of bitwidth
}

long sat_add(long a, long b, int bitwidth) {
    long c = a + b;
    long msb = 1L << (bitwidth - 1);
    
    if (!(a & msb) && !(b & msb) && (c & msb)) // overflow
        c = signed_max(bitwidth);
    else if ((a & msb) && (b & msb) && !(c & msb)) // underflow
        c = signed_min(bitwidth);
    
    return c;
}

long convert_arg(const char *str, long low, long high, const char *argname) {
    char *end;
    long n = strtol(str, &end, 0);
    
    if (*end != '\0') {
        fprintf(stderr, "Invalid number '%s'", str);
        exit(1);
    }
    if (n < low || n > high || errno == ERANGE) {
        fprintf(stderr, "Illegal value %s. %s must be in range [%ld, %ld].\n", str, argname, low, high);
        exit(1);
    }
    
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Missing argument. Please specify the bitwidth.\n");
        exit(1);
    }
    
    int bitwidth = convert_arg(argv[1], 4, sizeof(long) * 8, "Bitwidth");
    long min = signed_min(bitwidth);
    long max = signed_max(bitwidth);

    if (argc < 4) {
        printf("%d-bit signed integer range\n", bitwidth);
        printf("min: %ld   %#018lx\n", min, min);
        printf("max:  %ld   %#018lx\n", max, max);
    }
    else {
        long a = convert_arg(argv[2], min, max, "Operand");
        long b = convert_arg(argv[3], min, max, "Operand");
        printf("%ld + %ld = %ld\n", a, b, sat_add(a, b, bitwidth));   
    }
    
    return 0;
}
