/* mycat:
 *
 * it is similar to the built-in cat Unix command
 * the purpose is to test read line function
 */

#include <stdio.h>
#include <stdlib.h>
#include "readline.h"

int main(int argc, char *argv[]) {
    FILE *fp;

    if (argc == 1)
        fp = stdin;
    else {
        fp = fopen(argv[1], "r");
        if (fp == NULL)
             fprintf(stderr, "%s: No such file,\n", argv[1]);
    }

    char *line;
    for (int i = 1; (line = read_line(fp)) != NULL; ++i) {
        printf("%6d  %s\n", i, line);
        free(line);
    }
    
    fclose(fp);
    return 0;
}
