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

void main() {

    int parent_to_child[5][2], child_to_parent[5][2],pid[5];
    for (int i = 0; i < 5; i++) {
        pipe(parent_to_child[i]);
        pipe(child_to_parent[i]);
    }
    for (int i = 0; i < 5; i++) {
        if (pid[i]=fork()) {
            sleep(1);
            int id;
            char piper[10], pipel[10], cid[5], pipel2[10], piper2[10];
            read(parent_to_child[i][0], &id, 2);
            sprintf(cid, "%d", id);
            sprintf(pipel, "%d", child_to_parent[i][0]);
            sprintf(piper, "%d", child_to_parent[i][1]);
            sprintf(pipel2, "%d", parent_to_child[i][0]);
            sprintf(piper2, "%d", parent_to_child[i][1]);
            execl("./child", "child", cid, pipel, piper, pipel2, piper2, NULL);

        }
    }
    for (int i = 0, j = 1; i < 5; i++) {
        write(parent_to_child[i][1], &j, 2);
        j++;
    }
    sleep(2);
    printf("---Parent READY\n");
    int idc[5],sum;
    srand(time(NULL));
    while (1) {
        idc[5]=0,sum=0;
        int n = rand() % 5;
        int req = REQUEST;
        write(child_to_parent[n][1], &req, 3);
        sleep(1);
        int piv;
        read(parent_to_child[n][0], &piv, 3);
        printf("---Parent broadcasts pivot %d to all Children\n", piv);
        for (int i = 0; i < 5; i++) {
            write(child_to_parent[i][1], &piv, 3);
        }
        sleep(1);
        for (int i = 0; i < 5; i++) {
            read(parent_to_child[i][0], &idc[i], 3);
            sum += idc[i];
        }
        if (sum == 12) {
            printf("---Parent: m=%d+%d+%d+%d+%d=%d. 12 = 25/2. Median found!\n---Parent sends kill signals to all children\n",idc[0],idc[1],idc[2],idc[3],idc[4],sum);
            for(int i=0;i<5;i++){
                kill(pid[i],SIGINT);
            }
        }
        sleep(1);
    }
}

