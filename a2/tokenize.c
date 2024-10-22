/* tokenize
 *
 * use scan_token function to calculate the number of test word syllables
 * and compare with strtok function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool scan_token(const char **p_input, const char *delimiters, char buf[], size_t buflen) {
    if (*p_input == NULL || **p_input == '\0')
        return false;
    
    size_t dellen = strspn(*p_input, delimiters);
    *p_input += dellen;
    size_t toklen = strcspn(*p_input, delimiters);
    
    if (toklen > 0){
        if (toklen >= buflen)
            toklen = buflen-1;
        
        strncpy(buf, *p_input, toklen);
        buf[toklen] = '\0';
        *p_input += toklen;
        
        return true;
    }
    else
        return false;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Missing argument. Please supply delimiters and string to tokenize.\n");
        exit(1);
    }

    const char *delim = argv[1];
    const char *input = argv[2];

    char copy[strlen(input) + 1];
    strcpy(copy, input); // make copy since strtok is going to destroy it

    printf("Using strtok:     { ");
    for (char *token = strtok(copy, delim); token != NULL; token = strtok(NULL, delim))
        printf("\"%s\" ", token);
    printf("}\n");

    char buf[10];
    const char *remaining = input;
    printf("Using scan_token: { ");
    while (scan_token(&remaining, delim, buf, sizeof(buf)))
        printf("\"%s\" ", buf);
    printf("}\n");

    return 0;
}
