#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 30
#define READ_END 0
#define WRITE_END 1

//Cuiado por el libro " Operating System Concepts, 8th edition".

int main(int argc, char** argv){
    pid_t pid;
    int n, fd, fds[2], i;
    const int size = 2048;
    const char* NAME = "/OS";
    const char* myfifo = "/tmp/myfifo";
    char *x;

    if (argc < 2){
        n = 50;
        x = "a";
    }
    else {
        n = atoi(argv[1]);
        x = argv[2];
    }

    int buffer_size = strlen(x) + 1;

    printf(" Yo soy %s \n", x);

    mkfifo(myfifo, 0666);

    fd = open(myfifo, 0666);

    int shm_fd;
    char *ptr;
    //Memoria Object en esta linea - memoria compartida definido
    shm_fd = shm_open(NAME, O_EXCL | O_CREAT | O_RDWR, 0666);
    /*if (shm_fd == -1){
        fprintf(stderr, "%s: Shared memory not possible to create for memory object: %s. \n", NAME, strerror(errno));
        return EXIT_FAILURE;
    }
    */
    if(shm_fd == -1){
        printf("%c: Share mem obj already created \n", x[0]);
        shm_fd = shm_open(NAME, O_RDWR, 0666);
        int length;
        read(fd, &length, sizeof(length));
        printf("%c: Received shm fd: %d \n", x[0], length);
    }
    else{
        printf("%c: Create new shared memory object %d \n", x[0], shm_fd);
        write(fd, &shm_fd, sizeof(shm_fd));
        printf("%c: Initialized shared mem obj \n", x[0]);
    }
    
    close(fd);

    ftruncate(shm_fd, SIZE);
    ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    printf("%c: ptr created with value %p", x[0], ptr);

    if(pipe(fds) == -1){
        // si falla en crearse un pipe
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid = fork();

    if(pid == 0) {
        char length_message[buffer_size];
        close(fds[WRITE_END]);
        while(read(fds[READ_END], length_message, buffer_size) > 0){
            sprintf(ptr, "%s", (char *) length_message);
            ptr += strlen(length_message);
        }
        close(fds[READ_END]);
    }
    else{
        close(fds[READ_END]);
        for(i=0; i < SIZE; i++){
            if(i%n == 0){
                write(fds[WRITE_END], x, buffer_size);
            }
            close(fds[WRITE_END]);
            wait(NULL);
            printf("%c: Shared memory has %s \n", x[0], (char *) ptr);
        }
        shm_unlink(NAME);
        close(fd);
            munmap(ptr,SIZE);
            shm_unlink(NAME);
        return 0;
    }
}