#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // getopt
#include <dirent.h>     // opendir, readdir, closedir
#include <sys/stat.h>   // stat, struct stat
#include <string.h>

void print_perms(mode_t m) {
    char perms[11] = "----------";
    // File type
    if      (S_ISREG(m))  perms[0] = '-';
    else if (S_ISDIR(m))  perms[0] = 'd';
    else if (S_ISLNK(m))  perms[0] = 'l';
    else                  perms[0] = '?';
    // Owner
    perms[1] = (m & S_IRUSR) ? 'r' : '-';
    perms[2] = (m & S_IWUSR) ? 'w' : '-';
    perms[3] = (m & S_IXUSR) ? 'x' : '-';
    // Group
    perms[4] = (m & S_IRGRP) ? 'r' : '-';
    perms[5] = (m & S_IWGRP) ? 'w' : '-';
    perms[6] = (m & S_IXGRP) ? 'x' : '-';
    // Others
    perms[7] = (m & S_IROTH) ? 'r' : '-';
    perms[8] = (m & S_IWOTH) ? 'w' : '-';
    perms[9] = (m & S_IXOTH) ? 'x' : '-';
    printf("%s", perms);
}

int main(int argc, char *argv[]) {
    int opt;
    int show_size = 0, show_perm = 0;
    while ((opt = getopt(argc, argv, "sp")) != -1) {
        switch (opt) {
            case 's': show_size = 1; break;
            case 'p': show_perm = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-s] [-p] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    const char *dirpath = (optind < argc) ? argv[optind] : ".";
    
    DIR *dir = opendir(dirpath);
    if (!dir) { 
        perror("opendir");
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Build full path
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) < 0) {
            fprintf(stderr, "stat(%s)", fullpath);
            continue;
        }

        // Print name
        printf("%s", entry->d_name);

        // Print size if requested
        if (show_size) {
            printf("  %lld", (long long)st.st_size);
        }
        // Print permissions if requested
        if (show_perm) {
            printf("  ");
            print_perms(st.st_mode);
        }
        printf("\n");
    }

    closedir(dir);
}
