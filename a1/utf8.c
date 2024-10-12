/* UTF-8:
 *
 * accept code point as command line argument
 * print the UTF-8 encoding and the actual character
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int to_utf8(unsigned short cp, unsigned char seq[]) {
    unsigned short bound[4] = {(unsigned short)0x0000,
                               (unsigned short)0x0080,
                               (unsigned short)0x0800};
    
    if (bound[0] <= cp && cp < bound[1]) {
        seq[0] = cp;
        return 1;
    }
    if (bound[1] <= cp && cp < bound[2]) {
        seq[0] = '\xC0' + (char)((cp & (unsigned short)0x07C0) >> 6);
        seq[1] = '\x80' + (char)(cp & (unsigned short)0x003F);
        return 2;
    }
    else if (bound[2] <= cp) {
        seq[0] = '\xE0' + (char)((cp & (unsigned short)0xF000) >> 12);
        seq[1] = '\x80' + (char)((cp & (unsigned short)0x0FC0) >> 6);
        seq[2] = '\x80' + (char)(cp & (unsigned short)0x003F);
        return 3;
    }
    else
        exit(1);
}

void print_utf8(unsigned short cp, unsigned char seq[], int len) {
    int i;
    
    printf("U+%04X   Hex:", cp);
    for (i = 0; i < len; i++)
        printf(" %02x", seq[i]);
    for ( ; i < 4; i++) 
        printf("   ");
        
    seq[len] = '\0';
    printf("Character: %s  \n", seq);  
}

unsigned short convert_arg(const char *str, unsigned short low, unsigned short high) {
    char *end;
    long n = strtol(str, &end, 0);
    
    if (*end != '\0')
        fprintf(stderr, "Invalid code point '%s'.\n", str);
    if (n < low || n > high) 
        fprintf(stderr, "Code point %s not in range [%u, %u].\n", str, low, high);
        
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) 
        fprintf(stderr, "Missing argument. Please supply one or more unicode code points in decimal or hex.\n");
    
    for (int i = 1; i < argc; i++) {
        unsigned short cp = convert_arg(argv[i], 0, USHRT_MAX);
        unsigned char seq[4];
        int len = to_utf8(cp, seq);
        print_utf8(cp, seq, len);  
    }
    
    return 0;
}
