#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char* argv[])
{
    struct timeval start, end;
    float elpased_time;

    int numfiles =  100 * 1024;
    gettimeofday(&start, NULL);
    FILE *fp = NULL;
    for (int i = 0; i < numfiles; i++)
    {
        char filename[20];
        sprintf(filename, "%s/%d", argv[1], i);
        fp = fopen(filename, "w");
        if (fp)
        {
            fprintf(fp, "a");
            fclose(fp);
        }
        fp = NULL;
    }
    sync();
    for (int i = 0; i < numfiles; i += 7)
    {
        char filename[20];
        sprintf(filename, "%s/%d", argv[1], i);
        remove(filename);
    }
    char lasts[20];
    sprintf(lasts, "%s/largefile.txt", argv[1]);

    fp = fopen(lasts, "w");
    while(fprintf(fp, "1010") > 0);

    fclose(fp);
    fp = NULL;
    sync();
    for(int i = 1 ; i < numfiles ; i++)
    {
        if(i % 7 == 0)
            continue;
        char filename[20];
        sprintf(filename, "%s/%d", argv[1], i);
        remove(filename);
    }
    char cmd[100];
    sprintf(cmd, "filefrag -v %s/largefile.txt", argv[1]);
    system(cmd);

    gettimeofday(&end, NULL);
    elpased_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Elapsed time: %.5f sec .\n", elpased_time);
}
