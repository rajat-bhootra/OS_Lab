#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc , char *argv[]){
    int n = atoi(argv[1]);
    int fd = open(argv[2], O_RDONLY);
    int newlinesread=0;
    int totalread=0;
    char * totalcontent =malloc(1);
    int pos = lseek(fd, 0, SEEK_END);
    while (pos>0 && newlinesread<=n){
        int toread = 100;
        char temp[100];
        pos-=toread;
        lseek(fd, pos, SEEK_SET);
        int r = read(fd, temp, toread);
        for (int i = toread-1; i >=0 ; i--){
            if (temp[i]=='\n'){
                newlinesread++;
            }
        }
        char *buffer =malloc(totalread + toread +1);
        memcpy(buffer, temp, toread);
        buffer[totalread +toread]=0;
        memcpy(buffer + toread, totalcontent, totalread);
        free(totalcontent);
        totalcontent = buffer;
        totalread+=toread;
    }
    int skiplines = newlinesread-n;
    int i=0;
    while(skiplines>0){
        if (totalcontent[i]=='\n'){
            skiplines--;
        }
        i++;
    }
    
    printf("%s", totalcontent+i);
    return 0;
}