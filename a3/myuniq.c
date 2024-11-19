/* myuniq:
 *
 * a simplified version of Unix uniq command
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "readline.h"

void print_uniq_lines(FILE *fp) {
    char *prev = NULL;
    char *curr = NULL;
    size_t count = 1;
    
    while (true) {
        if (ungetc(getc(fp), fp) == EOF){
            if (prev != NULL){
                printf("%lu %s\n", count, prev);
                free(prev);
            }
            
            return;
        }
        
        curr = read_line(fp);
        if (prev != NULL) {
            if (strcmp(prev, curr)) {
                printf("%lu %s\n", count, prev);
                count = 1;
            }
            else
                ++count;
            
            free(prev);
        }
        
        prev = curr;
    }
}

int main(int argc, char *argv[]) {
    FILE *fp;

    if (argc == 1)
        fp = stdin;
    else {
        fp = fopen(argv[1], "r");
        if (fp == NULL)
            fprintf(stderr, "%s: No such file.", argv[1]);
    }
    
    print_uniq_lines(fp);
    fclose(fp);
    return 0;
}
