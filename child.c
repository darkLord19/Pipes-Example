#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>

#define REQUEST 100 
#define PIVOT 200
#define LARGE 300 
#define SMALL 400  
#define READY 500

char child_id[5];

void handler(int sig) {
    printf("---Child %s terminates\n", child_id);
    exit(0);
}

void main(int argc, char* argv[]) {
    signal(SIGINT, handler);
    strcpy(child_id, argv[1]);
    int fd = atoi(argv[2]);
    int fdl = atoi(argv[3]);
    int fd2 = atoi(argv[4]);
    int fdl2 = atoi(argv[5]);
    fflush(stdout);
    char a[] = "dat_";
    char b[] = ".txt";
    strcat(a, argv[1]);
    strcat(a, b);
    FILE *f;
    f = fopen(a, "r");
    int num[5];
    for (int i = 0; i < 5; i++) {
        fscanf(f, "%2d", &num[i]);
    }
    int xx = READY;
    srand(time(NULL));
    printf("---Child %s sends READY\n", argv[1]);
    char aa[10];
    int j = 0;
    while (1) {
        int aaa, xxx = 0;
        sleep(1);
        read(fd, &aaa, 3);
        if (aaa == 100) {
            int n = rand() % 5;
            write(fdl2, &num[n], 3);
            printf("---Child %s sends %d to Parent\n", argv[1], num[n]);
        } else {
            sleep(1);
            for (int i = 0; i < 5; i++) {
                if (num[i] > aaa) {
                    xxx++;
                }
            }
            printf("---Child %s receives pivot and replies %d\n", argv[1], xxx);
            fflush(stdout);
            write(fdl2, &xxx, 3);
        }
    }
    printf("End\n");
}