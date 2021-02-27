#include <stdio.h>
#include <unistd.h>
//libreria de usleep
#include <sys/wait.h>

int main (){
    pid_t child = fork();

    if ( child = 0) {
        execl("ipc", "ipc", "10", "b", (char *) NULL);
    }
    else {
        //usleep(1000);
        execl("ipc", "ipc", "10", "a", (char *) NULL);

    }
}