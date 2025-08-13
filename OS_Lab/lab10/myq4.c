#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

void print_tree(const char *path, const char* prefix){
    DIR *dir = opendir(path);
    if (!dir){
        perror("opendir");
        return ;
    }

    struct dirent **name_list;
    int n = scandir(path,&name_list, NULL, alphasort);
    if (n<0){
        perror("scandir");
        closedir(dir);
        return ;
    }

    for (int i = 0; i < n; i++)
    {
        char *name = name_list[i]->d_name;
        if (strcmp(name,".") == 0 || strcmp(name,"..") == 0){
            free(name_list[i]);
            continue;
        }
        
        printf("%s|- %s\n",prefix,name);

        char childpath[PATH_MAX];
        snprintf(childpath, sizeof(childpath), "%s/%s,",path,name);

        struct stat st;
        if (stat(childpath,&st)==0 && S_ISDIR(st.st_mode)){
            char newprefix[PATH_MAX];
            snprintf(newprefix, sizeof(newprefix), "%s|     ",prefix);
            print_tree(childpath,newprefix);
        }   
        free(name_list[i]);
    }

    free(name_list);
    closedir(dir);
}


int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage : %s <directory>\n",argv[0]);
        return 1;
    }

    const char *start = argv[1];

    struct stat st;

    if (stat(start,&st) < 0){
        perror("stat");
        return 1;
    }

    if (!S_ISDIR(st.st_mode)){
        printf("Error : %s is not a directory.\n",start);
        return 1;
    }

    print_tree(start, "");
    return 0;
}