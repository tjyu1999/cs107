/* myls:
 *
 * a simplified version of the Unix ls command
 * it takes in zero or more arguments, which are paths to directories,
 * and lists the directory entries from each path, one per line
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <getopt.h>

bool is_dir(int filetype) {
    return filetype == DT_DIR;
}

int is_fold(const struct dirent *entry) {
    return is_dir(entry->d_type);
}

int is_file(const struct dirent *entry) {
    return !is_fold(entry);
}

int wi_dot(const struct dirent *entry) {
    return (entry->d_name)[0] == '.';
}

int wo_dot(const struct dirent *entry) {
    return !wi_dot(entry);
}

int fold_wo_dot(const struct dirent *entry) {
    return wo_dot(entry) && is_fold(entry);
}

int file_wo_dot(const struct dirent *entry) {
    return wo_dot(entry) && is_file(entry);
}

int dir_comp(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}

void ls(const char *dir, bool show_all, bool sort_by_type) {
    struct dirent **lists[2] = {NULL, NULL};
    int nums[2] = {-2, -2};

    if (show_all && sort_by_type) {
        nums[0] = scandir(dir, &lists[0], is_fold, dir_comp);
        nums[1] = scandir(dir, &lists[1], is_file, dir_comp);
    }
    else if (show_all && !sort_by_type)
        nums[0] = scandir(dir, &lists[0], NULL, dir_comp);
    else if (show_all == false && sort_by_type == false)
        nums[0] = scandir(dir, &lists[0], wo_dot, dir_comp);
    else {
        nums[0] = scandir(dir, &lists[0], fold_wo_dot, dir_comp);
        nums[1] = scandir(dir, &lists[1], file_wo_dot, dir_comp);
    }

    for (int i = 0; i < 2; ++i) {
        if (nums[i] == -1) {
            printf("./myls: cannot access %s: No such directory\n", dir);
            continue;
        }
        
        if (lists[i] == NULL || nums[i] == -2)
            continue;
        
        for (int j = 0; j < nums[i]; ++j) {
            struct dirent *entry = lists[i][j];
        
	    if (is_fold(entry))
                printf("%s  ", entry->d_name);
            else
                printf("%s  ", entry->d_name);
        
            free(entry);
        }
        
        printf("\n");
        free(lists[i]);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    bool show_all = false;
    bool dirs_first = false;
    
    while ((opt = getopt(argc, argv, "az")) != -1) {
        switch (opt) {
            case 'a':
                show_all = true;
                break;
            
            case 'z':
                dirs_first = true;
                break;
            
            default:
                exit(1);
        }
    }
    
    if (optind < argc -1) {
        for (int i = optind; i < argc; ++i) {
            printf("%s:\n", argv[i]);
            ls(argv[i], show_all, dirs_first);
            printf("\n");
        }
    }
    else
        ls(optind == argc -1 ? argv[optind] : ".", show_all, dirs_first);
    
    return 0;
}
