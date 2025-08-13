#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void print_perm (mode_t m){
    char perm[11] = "----------";
    if (S_ISREG(m))      perm[0] = '-';
    else if (S_ISDIR(m)) perm[0] = 'd';
    else if (S_ISLNK(m)) perm[0] = 'l';
    else                 perm[0] = '?';
    
    perm[1] = (m & S_IRUSR) ? 'r' : '-';
    perm[2] = (m & S_IWUSR) ? 'w' : '-';
    perm[3] = (m & S_IXUSR) ? 'x' : '-';
    
    perm[4] = (m & S_IRGRP) ? 'r' : '-';
    perm[5] = (m & S_IWGRP) ? 'w' : '-';
    perm[6] = (m & S_IXGRP) ? 'x' : '-';
    
    perm[7] = (m & S_IROTH) ? 'r' : '-';
    perm[8] = (m & S_IWOTH) ? 'w' : '-';
    perm[9] = (m & S_IXOTH) ? 'x' : '-';

    printf("%s",perm);
}

int main(int argc, char *argv[]){
    int opt;
    int size = 0, perm = 0;

    while ((opt = getopt(argc,argv, "sp")) != -1)
    {
        switch (opt)
        {
        case 's':
            size = 1;
            break;
        
        case 'p':
            perm = 1;
        default:
            printf("Usage : %s [-s] [-p] [directory]\n",argv[0]);
            return 1;
        }
    }

    const char *dirpath = (optind < argc) ? argv[optind] : ".";
    
    DIR *dir = opendir(dirpath);
    if (!dir){
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL );
    {

        if ((strcmp(entry->d_name,".") == 0) || (strcmp(entry->d_name,".." == 0))){
            continue ;
        }
        
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);

        struct stat st;

        stat(fullpath,&st);

        printf("%s",entry->d_name);

        if (size)
        {
            printf("    %ld",st.st_size);
        }

        if (perm){
            printf("    ");
            print_perm(st.st_mode);
        }
        printf("\n");
    }
    closedir(dir);
    
}