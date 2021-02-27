#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    clock_t clock1, clock2, clock_delta;
    pid_t ciclo1, ciclo2, ciclo3;
    int i;
    clock1 = clock();
    if ((ciclo1 = fork())== 0){
        //printf("Hijo \n");
    }
    else{
        if ((ciclo2 = fork() ) == 0){
            //printf("Nieto \n");
        }
        else {
            if ((ciclo3 = fork()) == 0){
                //printf("Bisnieto \n");
                for(i = 0; i < 1000000; i++){
                    printf("%d",i);
                }
                wait(NULL);
            }
            else{
                for(i = 0; i < 1000000; i++){
                    printf("%d",i);
                }
                wait(NULL);

            }
            for(i = 0; i < 1000000; i++){
                    printf("%d",i);
                }
                wait(NULL);
        }
        wait(NULL);
        clock2 = clock();
        clock_delta = (double) (clock1 - clock2);
        printf("Clock total: %f \n", clock_delta);
        return 0;
    }
}