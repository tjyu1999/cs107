/* read line:
 * 
 * take one parameter, which is a file to read from
 * return the next line as a heap-allocated C string or NULL if there's nothing more to read
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>

// initial allocation will be for min size
#define MINIMUM_SIZE 32

char *read_line(FILE *fp) {
    size_t current_size = MINIMUM_SIZE;
    char *line = NULL;
    
    while (true) {
        line = realloc(line, current_size);
        assert(line != NULL);
    
        if (ungetc(getc(fp), fp) == EOF) {
            free(line);
            return NULL;
        }
    
        fgets(line, current_size, fp);
        char *point = strstr(line, "\n");
    
        if (point) {
            *point = '\0';
            return line;
        }
        else {
            if (ungetc(getc(fp), fp) == EOF)
                return line;
            else {
                assert(fseek(fp, 1 - current_size, SEEK_CUR) == 0);
                current_size *= 2;
            }
        }
    }
    
    return NULL;
}
