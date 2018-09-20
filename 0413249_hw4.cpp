#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <functional>
#include <iostream>
#include <iomanip>

using namespace std;


struct task_t;

class threadpool{
public:
	pthread_t *threads;
	int threadcount;
	int pool_end;
};

class task_t{
public:
    int argu[3];                     // taskID, left, right
    std::function<void(void *)> task;
    task_t *nexttask;
};


int data[1000000] , temp_[1000000];
task_t *tasklist, *tasktail, *readylist;
int taskcount, readycnt;

sem_t S1;	            // semaphore S1 indicates a new task available
sem_t S2;             // semaphore S2 locks the task
sem_t S3;             // semaphore S3 indicates ready list is available
sem_t Close;
sem_t done[8];

task_t tasks[16];

void taskfunction(void *);
void* threading(void *);
void tasklist_init(int);
void* main_threading(void *);
int partition(int , int , int );
void bubble(int , int );
void swap(int , int );
///////////////////////////////////////////
/////////////////////////////////////////
////////////////////////////////////////
int main(void)
{
	int  n;
	FILE *fptr;

	pthread_t masterthread;
	threadpool pool;

	struct timeval start_t, end_t;

	char file[100];
	char filename[100];

	int d;
	int size;

	////////////////////////////////////////////////////////////////////////
	//read file
	cout << "Please Enter the input file name:   (ex:: input.txt)" << endl ;
	scanf("%s" , file) ;
	FILE *fin = fopen(file , "r" ) ;

	fscanf(fin , "%d" , &size) ;
	for(int i = 0 ; i < size ; i++){
		 fscanf(fin , "%d" , &d) ;
		 temp_[i] = d ;
	}

	fclose(fin);
  ///////////////////////////////////////////////////////////////////

	for(n = 1; n <= 8; n++)
	{
		for(int i = 0; i < size; i++)
			data[i] = temp_[i];

		gettimeofday(&start_t, NULL);

		// semaphores initialize
		///////////////////////////
		sem_init(&S1, 0, 0);
		sem_init(&S2, 0, 1);
		sem_init(&S3, 0, 1);

		for(int i = 0; i < 8; i++)
			sem_init(&done[i], 0, 0);

		/////////////////////////
		sem_init(&Close, 0, 0);
    /////////////////////////////////////////////
		// threadpool initialize
		pool.threadcount = n;

		pool.pool_end = 0;

		pool.threads = (pthread_t*)malloc(pool.threadcount * sizeof(pthread_t));

		for(int i = 0; i < pool.threadcount; i++)
		{
			pthread_create(&pool.threads[i], NULL, threading, &pool.pool_end);
		}
		/////////////////////////////////////////////
		// tasklist initialize
		tasklist_init(size);

		// create master thread
		pthread_create(&masterthread, NULL, main_threading, &pool);

		for(int i = 0; i < 8; i++)
			sem_wait(&done[i]);

		////////////////////////////////////
    //pool destroy
		pool.pool_end = 1;

		free(pool.threads);

		//////////////////////////////
		//sem destroy
		sem_destroy(&S1);
		sem_destroy(&S2);
		sem_destroy(&S3);

		for(int i = 0; i < 8; i++)
			sem_destroy(&done[i]);
		sem_destroy(&Close);
    //////////////////////////////////////////////////

		gettimeofday(&end_t, NULL);

		int sec = end_t.tv_sec - start_t.tv_sec;
		int usec = end_t.tv_usec - start_t.tv_usec;
		float elp;
		elp = sec*1000.0+(usec/1000.0);
		cout << "thread pool with " << n << "threads ." << endl ;
		cout << "thread Elapsed time: " << setprecision(10) << elp << "  msec." << endl;

		///////////////////////////////////////////////////////
		// write to file
		FILE *fp;
		switch(n){
		case 1:
			fp = fopen("output_1.txt", "w");
			break;
		case 2:
			fp = fopen("output_2.txt", "w");
			break;
		case 3:
			fp = fopen("output_3.txt", "w");
			break;
		case 4:
			fp = fopen("output_4.txt", "w");
			break;
		case 5:
			fp = fopen("output_5.txt", "w");
			break;
		case 6:
			fp = fopen("output_6.txt", "w");
			break;
		case 7:
			fp = fopen("output_7.txt", "w");
			break;
		case 8:
			fp = fopen("output_8.txt", "w");
			break;

		}
		for(int i=0; i<size; i++)
		{
			fprintf(fp, "%d ", data[i]);
		}
		fclose(fp);
	}
	return 0;
}
////////////////////////////////////////////
/////////////////////////////////////////////
////////////////////////////////////////////
void tasklist_init(int size)
{
	tasklist = &tasks[0];

	taskcount = 1;
	readycnt = 0;

	tasklist->argu[0] = 1;	         // taskID
	tasklist->argu[1] = 0;	         // left
	tasklist->argu[2] = size - 1;	   // right
	tasklist->task = taskfunction;
	tasklist->nexttask = NULL;

	tasktail = tasklist;

	readylist = NULL;
}

void bubblesort(int left, int right)
{
	for(int i = left; i < right - 1; i++)
	{
		for(int j = left; j < right - i - 1; j++)
		{
			if(data[j] > data[j + 1])
				swap(j, j + 1);
		}
	}
}
int partition(int left, int right, int pivot)
{
	int Pivot_left = left - 1;
	int Pivot_right = right;

	while(1)
	{
		while(data[++Pivot_left] < pivot);

		while(Pivot_right > 0 && data[--Pivot_right] > pivot);

		if(Pivot_left >= Pivot_right)
			break;
		else
			swap(Pivot_left, Pivot_right);
	}

	swap(Pivot_left, right);

	return Pivot_left;
}


void* main_threading(void *n)
{
	threadpool *pool = (threadpool *)n;
	task_t *taskptr;

	sem_post(&S1);

	while(1){
		sem_wait(&S2);

		if(pool->pool_end == 1)
		{
			for(int i = 0; i < pool->threadcount; i++)
				sem_post(&S1);
			break;
		}

		if(taskcount == 0)
		{
			// no available task in list
			sem_wait(&S3);

			if(readycnt > 0){
				tasklist = readylist;

				taskptr = readylist;
				while(taskptr->nexttask != NULL)
					taskptr = taskptr->nexttask;

				tasktail = taskptr;

				taskcount = readycnt;
				readylist = NULL;
				readycnt = 0;

				for(int i = 0; i < taskcount; i++)
					sem_post(&S1);
			}

			sem_post(&S3);
			sem_post(&S2);
		}
		else
			sem_post(&S2);
	}
	pthread_exit(NULL);
}

void taskfunction(void *arg)
{
	// partition and append tasks to readylist, or sort
	int *arg_int = (int *)arg;
	int taskID = arg_int[0];
	int left = arg_int[1], right = arg_int[2];

	if(taskID < 8)
	{
		// partition
		int p;
		task_t *taskptr;

		p = partition(left, right, data[right]);

		// append to the readylist
		sem_wait(&S3);

		if(readylist == NULL)
			readylist = &tasks[taskID * 2 - 1];
		else
		{
			taskptr = readylist;
			while(taskptr->nexttask != NULL)
				taskptr = taskptr->nexttask;

			taskptr->nexttask = &tasks[taskID * 2 - 1];
		}

		tasks[taskID * 2].argu[0] = taskID * 2 + 1;
		tasks[taskID * 2].argu[1] = p + 1;
		tasks[taskID * 2].argu[2] = right;
		tasks[taskID * 2].task = taskfunction;
		tasks[taskID * 2].nexttask = NULL;

		tasks[taskID * 2 - 1].argu[0] = taskID * 2;
		tasks[taskID * 2 - 1].argu[1] = left;
		tasks[taskID * 2 - 1].argu[2] = p - 1;
		tasks[taskID * 2 - 1].task = taskfunction;
		tasks[taskID * 2 - 1].nexttask = &tasks[taskID * 2];

		readycnt = readycnt + 2;

		sem_post(&S3);
	}
	else
	{
		// sorting
		//using bubble sort , it would have a small problem ,so I finially modify it

    bubble(left, right);

		sem_post(&done[taskID - 8]);
	}
}

void* threading(void *n)
{
	task_t *currtask;
	int *pool_end = (int *)n;

	while(1)
	{
		sem_wait(&S1);

		if(*pool_end == 1)
		{
			sem_post(&S2);
			break;
		}

		// catch the task
		sem_wait(&S2);

		currtask = tasklist;
		tasklist = tasklist->nexttask;

		taskcount--;

		sem_post(&S2);

		// do the task
		currtask->task(currtask->argu);
	}
	pthread_exit(NULL);
}

void bubble(int left, int right)
{
	if(right - left <= 0)
		return;
	else
	{
		int pivot = data[right];
		int p = partition(left, right, pivot);
		bubble(left, p - 1);
		bubble(p + 1, right);
	}
	return;
}
void swap(int a, int b)
{
	int temp = data[a];
	data[a] = data[b];
	data[b] = temp;
}
