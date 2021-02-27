#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(){
    clock_t clock1, clock2, clock_delta
    int i;
    clock1 = clock();
    for(i = 0; i < 1000000; i++){
        printf("%d",i);
    }
    for(i = 0; i < 1000000; i++){
        printf("%d",i);
    }
    for(i = 0; i < 1000000; i++){
        printf("%d",i);
    }
    clock2 = clock();
    clock_delta = (double)(clock1 - clock2);
    printf("Clock total: %f \n", clock_delta);
    return 0;
}