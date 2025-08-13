#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>     // opendir, readdir, closedir
#include <sys/stat.h>   // stat, S_ISDIR
#include <limits.h>     // PATH_MAX

void print_tree(const char *path, const char *prefix) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, alphasort);
    if (n < 0) {
        perror("scandir");
        closedir(dir);
        return;
    }

    for (int i = 0; i < n; i++) {
        char *name = namelist[i]->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            free(namelist[i]);
            continue;
        }

        printf("%s|- %s\n",prefix ,name);

        // Build new path
        char childpath[PATH_MAX];
        snprintf(childpath, sizeof(childpath), "%s/%s", path, name);

        // Recurse if directory
        struct stat st;
        if (stat(childpath, &st) == 0 && S_ISDIR(st.st_mode)) {
            char new_prefix[PATH_MAX];
            snprintf(new_prefix, sizeof(new_prefix), "%s|   ",prefix);
            print_tree(childpath, new_prefix);
        }

        free(namelist[i]);
    }
    free(namelist);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *start = argv[1];

    struct stat st;
    if (stat(start, &st) < 0) {
        perror("stat");
        return EXIT_FAILURE;
    }
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: %s is not a directory\n", start);
        return EXIT_FAILURE;
    }

    // printf("%s\n", start);
    print_tree(start, "");
    return EXIT_SUCCESS;
}
