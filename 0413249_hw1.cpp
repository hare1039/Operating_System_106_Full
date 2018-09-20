#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <signal.h>
#define space " "

using namespace std;

static char *token;

void start_process(char *args[],int);
void io_redirect(char ,int);


void start_process(char *args[] , int background)
{
    pid_t child_pid;
    int status = 0;
    
    

    child_pid = fork();
    
    if (child_pid == 0) 
    {
    
        if (execvp(args[0], args) == -1)
            perror("error: try again");      
        exit(1);
    } 
    else if(child_pid < 0)
    {
        perror("error: try again !!!");
        exit(1);
    }
    else 
    {     
        if(background == 0) 
            waitpid(child_pid , &status , WUNTRACED); 
    }     
    
}
void io_redirect(char flag , int background) 
{
  
	char *args[99999];

	int num = 0;

	int status = 0;

	FILE* fp;

	pid_t pid;

	pid = fork(); 




	if(pid == 0) 
	{
	    args[num] = token;

	    while(token != NULL) 
	    {
	        token = strtok(NULL, " >");
	        num++;
	        args[num] = token;
	        
	    }


	    fp = freopen(args[num - 1], &flag, stdout);
	    if (fp == NULL) 
	    {
	        perror("error: try again !!!");
	        return;
	    }
	    args[num - 1] = NULL;


	    if((execvp(args[0], args) == -1)) 	    
	        perror("error: try again !!!");
	       

	    fclose(fp); 
	    _exit(0); 
	} 
	else if(pid < 0) 
	{
	    perror("error: try again !!!");
	    exit(1);
	}
	else 
	{
	    if(background == 0)
	        waitpid(pid , &status , WUNTRACED); 
	}
}
int main()
{
	char input[99999];
	char copy_input[99999];
	char *argu[99999];
	int background = 0;
    signal(SIGCHLD , SIG_IGN);
	while(1)
	{
        background = 0;
		cout << "> ";
		
		if(!fgets(input, 99999 , stdin))
			break;
			
		size_t length = strlen(input);
		
		if (input[length - 1] == '\n')
			input[length - 1] = '\0';
		
			    
		char *pos;
		int i = 0;
		
		strcpy(copy_input, input);

		token = strtok(input , space);
		

		if(!token)			
			continue;
			
        
		else if((pos = strchr(copy_input, '>'))) 
		{
			  
			  if ( *(pos + 1) == '>') 
			      io_redirect('a',background);
			  
			  else 
			      io_redirect('w',background);
			  background = 0;
			  
		} 
		else
		{
			while(token != NULL)
			{
				argu[i] = token;
				if(strcmp(argu[i] , "&") == 0)
				{
				     background = 1;
				     break;
				}
				i++;
				token = strtok(NULL, space);		    
			}
			argu[i]=NULL;		
			start_process(argu,background);
            background = 0;
		}

	}

}
