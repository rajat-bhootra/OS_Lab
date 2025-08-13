#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
 
void print_tree (char *path, int lvl){
    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0){
            continue;
        }
        
        for (int i = 0; i < lvl; i++)
        {
            printf("|   ");
        }
        printf("|-  %s\n",entry->d_name);
    
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path,entry->d_name);
    
        struct stat st;
        stat(fullpath,&st);
        if(S_ISDIR(st.st_mode)){
            print_tree(fullpath,lvl+1);
        }
    }
    
}

int main(int argc, char * argv[]){
    if (argc != 2){
        printf("Usage : %s <directory>\n",argv[0]);
        return 1;
    }

    print_tree(argv[1],0);
}