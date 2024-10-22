/* mywhich
 *
 * use scan_token and get_env_value functions to implement a simplified version of Unix which command
 * take the names of executables and print out their filesystem locations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <dirent.h>

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

const char *get_env_value(const char *envp[], const char *varname) {
    for (int i = 0; envp[i] != NULL; ++i){
        char *result = strstr(envp[i], varname);
        
        if (result == envp[i]){
            result += strlen(varname);
            if (*result == '=')
                return ++result;
        }
    }
    
    return NULL;
}

int main(int argc, char *argv[], const char *envp[]) {
    const char *searchpaths = get_env_value(envp, "MYPATH");
    
    if (searchpaths == NULL)
        searchpaths = get_env_value(envp, "PATH");

    if (argc == 1) {
        char dir[PATH_MAX+1];
        const char *remaining = searchpaths;

        printf("Directories in search path:\n");
        while (scan_token(&remaining, ":", dir, sizeof(dir)))
            printf("%s\n", dir);
    }
    else {
        argc--;
        argv++;
        char dir[PATH_MAX + 1];
        
        for (int i = 0; i < argc; ++i){
            if (!strlen(argv[i]))
                continue;
            
            unsigned char if_wild = 0;
            if (argv[i][0] == '+') {
                argv[i]++;
                if (!strlen(argv[i]))
                    continue;
                
                if_wild = 1;
            }
            
            const char *remaining = searchpaths;
            
            while (scan_token(&remaining, ":", dir, sizeof(dir))) {
                DIR *dp = opendir(dir);
                if (dp == NULL)
                    continue;
                    
                struct dirent *entry;
                unsigned char if_break = 0;
                
                while ((entry = readdir(dp)) != NULL) {
                    if (!if_wild) {
                        if (!strcmp(argv[i], entry->d_name)) {
                            printf("%s/%s\n", dir, entry->d_name);
                            if_break = 1;
                            break;
                        }
                    }
                    else {                        
                        if (strstr(entry->d_name, argv[i]))
                            printf("%s/%s\n", dir, entry->d_name);
                    }
                }
                
                closedir(dp);
                if (if_break)
                    break;
            }
        }
    }
    
    return 0;
}
