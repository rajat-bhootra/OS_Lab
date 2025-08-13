#include <stdio.h>
#include <sys/stat.h>

int main(int argc , char *argv[]){

    if (argc != 2){
        printf("Usage : %s <file path> \n",argv[0]);
        return 1;
    }

    const char *path = argv[1];
    struct stat st;

    if (stat(path,&st) < 0){
        perror("state");
        return 1;
    }

    printf("File size : %ld bytes\n",st.st_size);
    printf("Number of blocks allocated : %ld\n",st.st_blocks);
    printf("Reference count : %ld \n",st.st_nlink);

}


























