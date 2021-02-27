#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

int main(){
    pid_t pid1;
    int i;
    pid1 = fork();

    if (pid1 == 0){

        //printf("Hijo \n");
        for(i=0; i<4000000; i){
            printf("%d",i);
        }
    } else {
        while(true){

        }
    }
    return 0;
}