#include <iomanip>
#include <string.h>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <sys/time.h>
#include <cstdlib>
#include <cstdio>
#define thread_count 16
#define NUM_ 16

using namespace std;

///////////////////////////////////////////////////////////
/*for those size > 16 */
void single_thread();
void multiple_thread();
void bubble_sort(unsigned int *, unsigned int, unsigned int);
void *threading(void *);
void swapping(int , int );
//////////////////////////////////////////////////////////
/*for thise size <=16 */
void single_thread_sort(vector<unsigned int>*);
void multiple_thread_sort(vector<unsigned int>*);
void bubble_sort_(vector<unsigned int>*,int,int);
void *task(void*);
void swap(vector<unsigned int>*,int,int);
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/*for those size <= 16 */
class packet{
public:
    int start,end;
    vector<unsigned int> *arr;
};

packet *message;
pthread_t* thread_handles;
sem_t* semaphores;
sem_t* semaphores_p;
sem_t master_key;
fstream *output;
fstream *output2;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*for those size > 16 */
unsigned int *data_;
unsigned int *data;
unsigned int arr_size;
pthread_t threads[NUM_];
unsigned int pivot1, pivot2, pivot3, pivot4, pivot5, pivot6, pivot7;
sem_t semaphores_0, semaphores_1, semaphores_2, semaphores_3, semaphores_4,
    semaphores_5, semaphores_6, semaphores_7, semaphores_8,
    semaphores_9, semaphores_10, semaphores_11, semaphores_12,
    semaphores_13, semaphores_14, semaphores_15, semaphores_16;
sem_t semaphores_done0, semaphores_done1, semaphores_done2, 
    semaphores_done3, semaphores_done4, semaphores_done5, semaphores_done6;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{

   	char filename[20];
	FILE *fptr;

	char str[10], chr;


	pthread_t threadptr[16];
	struct timeval start, end;
	float elapsetime;


	cout << "> " ;
	scanf("%s", filename);
	string temp2 = string(filename);


    fptr = fopen(filename, "r");
	if(fptr == NULL)
	{
		cout << "ERROR: Unable to open the file. try enter again!!!!" << endl;
		exit(1);
	}

	if(fgets(str, 10, fptr) != NULL)
	{
		if(str[strlen(str) - 1] == '\n')
			str[strlen(str) - 1] = '\0';
		arr_size = atoi(str);
	}
	/////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	/*for those size > 16 */
	if(arr_size > 16)
   	{

	   	data = (unsigned int*)malloc(arr_size * sizeof(int));
		data_ = (unsigned int*)malloc(arr_size * sizeof(int));

		for(int i = 0;i < arr_size; i++)
		{
			int n = 0;
			while((chr = fgetc(fptr)) != ' ' && feof(fptr) != 1)
				str[n++] = chr;
			str[n] = '\0';

			data[i] = atoi(str);
			data_[i] = atoi(str);
		}

		fclose(fptr);

	    //multi thread

		
	    gettimeofday(&start,0);
	    multiple_thread();
	    gettimeofday(&end,0);
	    
	    float elapsetime;
	    elapsetime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
		cout << "multi thread Elapsed time: " << setprecision(10) << elapsetime << "  msec." << endl;

	    fptr = fopen("output1.txt", "w+");
	    
	    //output file
		for(int i = 0; i < arr_size; i++)
		{
			sprintf(str, "%d ", data[i]);
			fputs(str, fptr);
		}
		fclose(fptr);


	    //single thread
	    gettimeofday(&start,0);
	    single_thread();
	    gettimeofday(&end,0);
	    
	    elapsetime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
		cout << "single thread Elapsed time: " << setprecision(10) << elapsetime << "  msec." << endl;
		
		//output file

	    fptr = fopen("output2.txt", "w+");
		for(int i = 0; i < arr_size; i++)
		{
			sprintf(str, "%d ", data_[i]);
			fputs(str, fptr);
		}
		fclose(fptr);


        free(data);
        free(data_);
    }   
       
     /////////////////////////////////////////////////
     //////////////////////////////////////////////////
     /////////////////////////////////////////////////////
    /*for those size <= 16 */
    else
    {
        fstream input_file(temp2.c_str());
        vector<unsigned int> arr;

        string token;

        stringstream temp;
        temp << token;
        temp >> arr_size;

        for(int i=0; i<arr_size; i++)
        {
        	input_file >> token;
        	stringstream arr_;
        	unsigned int insert_num;
        	arr_ << token;
        	arr_ >> insert_num;
        	arr.push_back(insert_num);
        }

        vector<unsigned int> arr_(arr);

		/////////////////////////////////
		
		//multi thread

        
		gettimeofday(&start,0);
		multiple_thread_sort(&arr);
		gettimeofday(&end,0);
		
		float elapsetime;
	    elapsetime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
		cout << "multi thread Elapsed time: " << setprecision(10) << elapsetime << "  msec." << endl;
		
		//output
		
		output = new fstream();
    	output->open("output1.txt", ios::out|ios::trunc);
    	for(int i=0; i<arr.size(); i++)
    	{
        	(*output) << arr[i] << " ";
    	}
    	cout << endl;
    	output->close();
    	delete output;

	    //single thread
		gettimeofday(&start,0);
		single_thread_sort(&arr_);
		gettimeofday(&end,0);
		
	 	elapsetime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
		cout << "single thread Elapsed time: " << setprecision(10) << elapsetime << "  msec." << endl;
		
		//output

		output2 = new fstream();
    	output2->open("output2.txt", ios::out|ios::trunc);
    	for(int i=0; i<arr.size(); i++)
    	{
       		(*output2) << arr_[i] << " ";
    	}
    	cout << endl;
    	output2->close();
    	delete output2;
    }
    //////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
	return 0;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
/*for those size > 16 */

void *threading(void *n)
{
	pthread_t id = pthread_self();

	if(pthread_equal(id, threads[1]))
	{
		sem_wait(&semaphores_0);
	
		unsigned int i, j;
		pivot1 = 0;
		i = 0;
		j = arr_size;
		do{
			do i++;
			while(data[i]<data[pivot1]);
			do j--;
			while(data[j]>data[pivot1]);
			if(i<j)
			    swapping(i , j);
		}while(i<j);
		unsigned int temp = data[pivot1];
		data[pivot1] = data[j];
		data[j] = temp;
		pivot1 = j;
	
		sem_post(&semaphores_2);
		sem_post(&semaphores_3);
	}
	else if (pthread_equal(id, threads[2])){
		sem_wait(&semaphores_2);
	
		unsigned int i, j;
		pivot2 = 0;
		i = pivot2;
		j = pivot1;
		do{
			do i++;
			while(data[i]<data[pivot2]);
			do j--;
			while(data[j]>data[pivot2]);
			if(i<j)
			    swapping(i , j);
		}while(i<j);
		unsigned int temp = data[pivot2];
		data[pivot2] = data[j];
		data[j] = temp;
		pivot2 = j;
	
		sem_post(&semaphores_4);
		sem_post(&semaphores_5);
	}
	else if (pthread_equal(id, threads[3])){
		sem_wait(&semaphores_3);
	
		unsigned int i, j;
		pivot3 = pivot1+1;
		i = pivot3;
		j = arr_size;
		do{
			do i++;
			while(data[i]<data[pivot3]);
			do j--;
			while(data[j]>data[pivot3]);
			if(i<j)
			    swapping(i , j);
		}while(i<j);
		unsigned int temp = data[pivot3];
		data[pivot3] = data[j];
		data[j] = temp;
		pivot3 = j;
	
		sem_post(&semaphores_6);
		sem_post(&semaphores_7);
	}
	else if (pthread_equal(id, threads[4])){
		sem_wait(&semaphores_4);
	
		unsigned int i, j;
		pivot4 = 0;
		i = pivot4;
		j = pivot2;
		do{
			do i++;
			while(data[i]<data[pivot4]);
			do j--;
			while(data[j]>data[pivot4]);
			if(i<j)
			    swapping(i , j);
		}while(i<j);
		unsigned int temp = data[pivot4];
		data[pivot4] = data[j];
		data[j] = temp;
		pivot4 = j;
	
		sem_post(&semaphores_8);
		sem_post(&semaphores_9);
	}
	else if (pthread_equal(id, threads[5])){
		sem_wait(&semaphores_5);
	
		unsigned int i, j;
		pivot5 = pivot2+1;
		i = pivot5;
		j = pivot1;
		do{
			do i++;
			while(data[i]<data[pivot5]);
			do j--;
			while(data[j]>data[pivot5]);
			if(i<j)
			    swapping(i , j);
		}while(i<j);
		unsigned int temp = data[pivot5];
		data[pivot5] = data[j];
		data[j] = temp;
		pivot5 = j;
	
		sem_post(&semaphores_10);
		sem_post(&semaphores_11);
	}
	else if (pthread_equal(id, threads[6])){
		sem_wait(&semaphores_6);
	
		unsigned int i, j;
		pivot6 = pivot1+1;
		i = pivot6;
		j = pivot3;
		do{
			do i++;
			while(data[i]<data[pivot6]);
			do j--;
			while(data[j]>data[pivot6]);
			if(i<j)
			    swapping(i , j);
		}while(i<j);
		unsigned int temp = data[pivot6];
		data[pivot6] = data[j];
		data[j] = temp;
		pivot6 = j;
	
		sem_post(&semaphores_12);
		sem_post(&semaphores_13);
	}
	else if (pthread_equal(id, threads[7])){
		sem_wait(&semaphores_7);
	
		unsigned int i, j;
		pivot7 = pivot3+1;
		i = pivot7;
		j = arr_size;
		do{
			do i++;
			while(data[i]<data[pivot7]);
			do j--;
			while(data[j]>data[pivot7]);
			if(i<j)
			    swapping(i , j);
		}while(i<j);
		unsigned int temp = data[pivot7];
		data[pivot7] = data[j];
		data[j] = temp;
		pivot7 = j;
	
		sem_post(&semaphores_14);
		sem_post(&semaphores_15);
	}
	else if (pthread_equal(id, threads[8])){
		sem_wait(&semaphores_8);
	
		bubble_sort(data, 0, pivot4);
		
		sem_post(&semaphores_done0);
	}
	else if (pthread_equal(id, threads[9])){
		sem_wait(&semaphores_9);
	
		bubble_sort(data, pivot4, pivot2);
		sem_wait(&semaphores_done0);
		sem_post(&semaphores_done1);
	}
	else if (pthread_equal(id, threads[10])){
		sem_wait(&semaphores_10);
	
		bubble_sort(data, pivot2, pivot5);
		sem_wait(&semaphores_done1);
		sem_post(&semaphores_done2);
	}
	else if (pthread_equal(id, threads[11])){
		sem_wait(&semaphores_11);
	
		bubble_sort(data, pivot5, pivot1);
		sem_wait(&semaphores_done2);
		sem_post(&semaphores_done3);
	}
	else if (pthread_equal(id, threads[12])){
		sem_wait(&semaphores_12);
	
		bubble_sort(data, pivot1, pivot6);
		sem_wait(&semaphores_done3);
		sem_post(&semaphores_done4);
	}
	else if (pthread_equal(id, threads[13])){
		sem_wait(&semaphores_13);
	
		bubble_sort(data, pivot6, pivot3);
		sem_wait(&semaphores_done4);
		sem_post(&semaphores_done5);
	}
	else if (pthread_equal(id, threads[14])){
		sem_wait(&semaphores_14);
	
		bubble_sort(data, pivot3, pivot7);
		sem_wait(&semaphores_done5);
		sem_post(&semaphores_done6);
	}
	else if (pthread_equal(id, threads[15])){
		sem_post(&semaphores_0);
		sem_wait(&semaphores_15);
	
		bubble_sort(data, pivot7, arr_size);
		sem_wait(&semaphores_done6);
		sem_post(&semaphores_16);
	}
	else{	
	}
	pthread_exit(NULL);
}


void swapping(int num1, int num2)
{
	int temp = data[num1];
	data[num1] = data[num2];
	data[num2] = temp;
}

void single_thread(){
	bubble_sort(data_, 0, arr_size);
}
void multiple_thread(){
	int j, i;
	int k;
	k = sem_init(&semaphores_0, 0, 0);
	sem_init(&semaphores_1, 0, 0);
	sem_init(&semaphores_2, 0, 0);
	sem_init(&semaphores_3, 0, 0);
	sem_init(&semaphores_4, 0, 0);
	sem_init(&semaphores_5, 0, 0);
	sem_init(&semaphores_6, 0, 0);
	sem_init(&semaphores_7, 0, 0);
	sem_init(&semaphores_8, 0, 0);
	sem_init(&semaphores_9, 0, 0);
	sem_init(&semaphores_10, 0, 0);
	sem_init(&semaphores_11, 0, 0);
	sem_init(&semaphores_12, 0, 0);
	sem_init(&semaphores_13, 0, 0);
	sem_init(&semaphores_14, 0, 0);
	sem_init(&semaphores_15, 0, 0);
	sem_init(&semaphores_16, 0, 0);
	sem_init(&semaphores_done0, 0, 0);
	sem_init(&semaphores_done1, 0, 0);
	sem_init(&semaphores_done2, 0, 0);
	sem_init(&semaphores_done3, 0, 0);
	sem_init(&semaphores_done4, 0, 0);
	sem_init(&semaphores_done5, 0, 0);
	sem_init(&semaphores_done6, 0, 0);

	for(i=0; i<NUM_; i++){
		j = pthread_create(&threads[i], NULL, threading, NULL);
		
	}

	k = sem_wait(&semaphores_16);

}


void bubble_sort(unsigned int *arr, unsigned begin, unsigned int end)
{
	unsigned int i, j;
	for(i=begin; i<end-1; i++)
	{
		for(j=i+1; j<end; j++)
		{
			if(arr[i] > arr[j])
			{
				unsigned int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

//////////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////////////
/*for those size < 16 */

void *task(void *rank_)
{
    long rank = (long)rank_;
    sem_wait(&semaphores[rank]);

    if(rank == 0)
    {
        message[1].start = 0;
        message[1].end = message[1].arr->size();
        sem_post(&semaphores[1]);

        sem_wait(&semaphores[0]);
        vector<unsigned int> *arr = message[rank].arr;

        sem_post(&master_key);
    }
    else 
    {
        int start = message[rank].start;
        int end = message[rank].end;
        vector<unsigned int> *arr = message[rank].arr;


        if(rank < thread_count/2)
        {
            if((end-start) <= 1)
            {
                message[2*rank].start = start;
                message[2*rank].end = end;
                message[2*rank+1].start = start;
                message[2*rank+1].end = end;
            }
            else 
            {
                swap(arr, start, (rand()%(end-start))+start);
                unsigned int pivot = (*arr)[start];
                int i=start, j=end;
                while(i+1 != j)
                {
                    if((*arr)[i+1] < pivot)
                    {
                        i++;
                    }
                    else 
                    {
                        j--;
                        swap(arr, i+1, j);
                    }
                }
                swap(arr, start, i);

                message[2*rank].start = start;
                message[2*rank].end = i;
                message[2*rank+1].start = i+1;
                message[2*rank+1].end = end;
            }
            sem_post(&semaphores[2*rank]);
            sem_post(&semaphores[2*rank+1]);
        }
        else 
        {
            bubble_sort_(arr, start, end);
            sem_post(&semaphores_p[rank]);
            if(rank == thread_count/2) 
            {
                for(int k=thread_count/2; k<thread_count; k++)
                {
                    sem_wait(&semaphores_p[k]);
                }
                sem_post(&semaphores[0]);
            }
        }
    }

}

void swap(vector<unsigned int> *arr,int i,int j)
{
    unsigned int temp = (*arr)[i];
    (*arr)[i] = (*arr)[j];
    (*arr)[j] = temp;
}

void single_thread_sort(vector<unsigned int>* arr)
{
    bubble_sort_(arr, 0, arr->size());   
}

void multiple_thread_sort(vector<unsigned int>* arr){
    
    thread_handles = new pthread_t[thread_count];
    semaphores = new sem_t[thread_count];
    semaphores_p = new sem_t[thread_count];
    message = new packet[thread_count];

    
    for(long i=0; i<thread_count; i++){
        message[i].arr = arr;
        sem_init(&semaphores[i], 0, 0);
    }
    sem_init(&master_key, 0, 0);
    for(long i=0; i<thread_count; i++){
        pthread_create(&thread_handles[i], NULL, &task, (void*)i);
    }
    sem_post(&semaphores[0]);

    sem_wait(&master_key);
    for(int i=0; i<thread_count; i++){
        sem_destroy(&semaphores[i]);
    }

    delete [] thread_handles;
    delete [] semaphores;
    delete [] message;
}

void bubble_sort_(vector<unsigned int>* arr, int start, int end){
    for(int i=start+1; i<end; i++)
    {
        for(int j=start+1; j<end; j++)
        {
            if((*arr)[j-1] > (*arr)[j])
            {
                unsigned int temp = (*arr)[j];
                (*arr)[j] = (*arr)[j-1];
                (*arr)[j-1] = temp;
            }
        }
    }
}


