/* automata:
 *
 * start with an initial pattern
 * simulate the birth and death of future generations of cells by using a set of simple rules
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#define LIVE  "+"
#define EMPTY " "
#define MSB sizeof(long) * CHAR_BIT - 1

const int pow2[3] = {1, 2, 4};

void convert_ruleset(unsigned char ruleset, int arr[]) { // translate the bit pattern of ruleset to a bool array
    for (int i = 0; i < CHAR_BIT; ++i)
        arr[i] = (('\1' << i) & ruleset) > '\0';
}

unsigned long acyclic_shift(unsigned long num, int shift) { // return num << shift without the cyclic behavior
    if ((shift <= MSB) && shift >= 0)
        return num << shift;
    else
        return 0UL;
}

int find_pattern(int i, unsigned long gen) {
    int out = 0;
    
    for (int j = 1; j >= -1; --j)
        out += ((acyclic_shift(1L, i + j) & gen) > 0) * pow2[j + 1];
    
    return out;
}

void draw_generation(unsigned long gen) {
    const char *font[] = {LIVE, EMPTY};
    
    for (int i = MSB; i >= 0; --i) {
        long bit = 1L << i;
        printf("%s", font[(gen & bit) > 0]);
    }
    printf("\n");
}

unsigned long advance(unsigned long gen, unsigned char ruleset) {
    unsigned long res = 0;
    int arr[CHAR_BIT];
    convert_ruleset(ruleset, arr);
    
    for (int i = MSB; i >= 0; --i) {
        int pattern = find_pattern(i, gen);
        res += (long)arr[pattern] << i;
    }
    
    return res;
}

unsigned long convert_arg(const char *str, unsigned long low, unsigned long high, const char *argname) {
    char *end;
    unsigned long n = strtoul(str, &end, 0);
    
    if (*end != '\0') {
        fprintf(stderr, "Invalid number '%s' for %s.\n", str, argname);
        exit(1);
    }
    if (n < low || n > high || errno == ERANGE) { 
        fprintf(stderr, "%s %s is not in range [%#lx, %#lx].\n", argname, str, low, high);
        exit(1);
    }
    
    return n;
}

int main(int argc, char *argv[]) {
    unsigned long gen_0 = 1L << 32;
    unsigned char ruleset = 30;

    if (argc < 2) {
        fprintf(stderr, "Missing argument. Please supply ruleset and optional initial generation.\n");
        exit(1);
    }
    
    ruleset = convert_arg(argv[1], 0, UCHAR_MAX, "ruleset");
    if (argc > 2)
        gen_0 = convert_arg(argv[2], 0, ULONG_MAX, "initial generation");

    if (ruleset == 0)
        draw_generation(gen_0);
    else {
        unsigned long gen = gen_0;
        
        for (int i = 0; i < 32; ++i) {
            draw_generation(gen);
            gen = advance(gen, ruleset);
        }
    }
    
    return 0;
}
