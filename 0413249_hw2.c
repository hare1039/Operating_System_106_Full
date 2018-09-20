#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>	



int main(){
	
	while(1)
	{
	    int mtr_dim, num_process;
	    pid_t pid[16];
	    int shmidA;
	    int shmidB;
	    int shmidC;
	    unsigned int *A, *B, *C;
	    struct timeval start, end;
	    unsigned int checksum;
	    float elpased_time;
	    
	    
		printf("Input the matrix dimension:");
		scanf("%d", &mtr_dim);
		for(num_process = 1; num_process <= 16; num_process++)
		{
			printf("Multiplying matrices using %d process\n", num_process);
			
			////////////////////////////////////////////////////////
			
			shmidA = shmget(IPC_PRIVATE, mtr_dim * mtr_dim * sizeof(unsigned int), IPC_CREAT | 0666);
			shmidB = shmget(IPC_PRIVATE, mtr_dim * mtr_dim * sizeof(unsigned int), IPC_CREAT | 0666);
			shmidC = shmget(IPC_PRIVATE, mtr_dim * mtr_dim * sizeof(unsigned int), IPC_CREAT | 0666);

			if( shmidA < 0 || shmidB < 0 || shmidC < 0)
			{ 
			    printf("shmget error\n"); 
			    return -1; 
			}

			A = shmat(shmidA, NULL, 0);
			B = shmat(shmidB, NULL, 0);
			C = shmat(shmidC, NULL, 0);


			for(int i = 0; i < mtr_dim*mtr_dim; i++){
				A[i] = i;
				B[i] = i;
				C[i] = 0;
			}
			
			
            ////////////////////////////////////////////////
			gettimeofday(&start, NULL);
			
			int i, row, col, n, k;

			for(i=0; i < num_process; i++){
			    pid[i] = fork();
			    if(pid[i] < 0)
			    {
					perror("Fork Failed.\n");
					exit(-1);
				}
			    else if(pid[i] == 0)
			    {
					int begin = i * (mtr_dim / num_process);
					int end = begin + (mtr_dim / num_process);
				    if(i == num_process-1)
				    {
				        end = mtr_dim; 
				    }
			        for(row = begin; row < end; row++)
			        {
						for(col = 0; col < mtr_dim; col++)
						{
							C[row*mtr_dim + col] = 0;	
							for(n = 0; n < mtr_dim; n++)
							{
								C[row * mtr_dim + col] += A[row * mtr_dim + n] * B[n * mtr_dim + col];
							}
						}
					}		
					exit(0);
				}
				
			}

			for(int i = 0; i < num_process; i++)
				waitpid(pid[i], NULL, 0);
				
		    gettimeofday(&end, NULL);

			//////////////////////////////////////////////////////
			checksum = 0;
			
			unsigned int *shm_addr;
			
			shm_addr =( unsigned int *) shmat(shmidC , NULL, 0);
			if(shm_addr < 0)
			{
			    printf("shmat error\n"); 
			    return -1; 
			}
			for(int i = 0; i < mtr_dim * mtr_dim ; i ++)
			{
                checksum += shm_addr[i];
            }
			
			//////////////////////////////////////////////////////
			
			elpased_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

			printf("Elapsed time: %.5f sec, Checksum: %u\n", elpased_time, checksum);

			shmdt(A);
			shmdt(B);
			shmdt(C);

			shmctl(shmidA, IPC_RMID, NULL);
			shmctl(shmidB, IPC_RMID, NULL);
			shmctl(shmidC, IPC_RMID, NULL);
		}
	}

	return 0;
}
