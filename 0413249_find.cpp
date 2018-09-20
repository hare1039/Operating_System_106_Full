#include <dirent.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <getopt.h>
#include <ctime>
#include <iostream>

using namespace std;

bool check_opt[4];
long int inode = -1;
char *name = NULL;
long long minsize , maxsize ;

void find(DIR *dir, char *path)
{
		struct dirent *dirptr;
		char fpath[1000000];
		double fsize;

		if( (dir=opendir(path)) == NULL)
		{
				exit(EXIT_FAILURE);
		}
		else
		{
				while((dirptr = readdir(dir)) != NULL)
				{
						if(!strcmp(dirptr->d_name,".")||!strcmp(dirptr->d_name,".."))
								continue;
						struct stat fstat;

						sprintf(fpath, "%s/%s", path, dirptr->d_name);
						stat(fpath, &fstat);

						fsize = fstat.st_size / 1048576.0;
						//cout << "check fsize" << fsize << endl;
						if((fstat.st_mode & S_IFMT) == S_IFDIR /*&& strcmp(dirptr->d_name, ".") && strcmp(dirptr->d_name, "..")*/)
						{
							DIR *dir = opendir(fpath);
							find(dir, fpath);
						}

						if(check_opt[2] == true && fstat.st_ino != inode)
							continue;

						if(check_opt[3] == true && strcmp(dirptr->d_name, name) != 0)
							continue;

						if(check_opt[0] == true && minsize > fsize)
							continue;

						if(check_opt[1] == true && maxsize < fsize)
							continue;

						cout << fpath << "  " << fstat.st_ino << "  " << (double)fstat.st_size / (1<<20) << "MB\n";
				}
		}
		return;
}

int main(int argc, char** argv)
{
		int c;
		char path[1000000];

		DIR *dir;

		strcpy(path, argv[1]);
		if(path[strlen(path) - 1] == '/')
			path[strlen(path) - 1] = '\0';
		/*if(argc == 1)
			cout << "Unknown path , Try again " << endl;*/
    check_opt[0] = false, check_opt[1] = false , check_opt[2] = false, check_opt[3] = false;
		for(int i=2;i<argc;i++)
		{
				if(!(i&1))
				{
						if(strcmp(argv[i],"-inode")==0)
						{
								sscanf(argv[i+1],"%ld",&inode);
								check_opt[2] = true;
						}
						else if(strcmp(argv[i],"-name")==0)
						{
							  name = (char*)malloc(255 * sizeof(char));
								strcpy(name , argv[i+1]);
								check_opt[3] = true;
						}
						else if(strcmp(argv[i],"-size_min")==0)
						{
							  //cout << "check min size : " << minsize << endl;
								sscanf(argv[i+1], "%lld", &minsize);
								//cout << "check min size : " << minsize << endl;
								check_opt[0] = true;
						}
						else if(strcmp(argv[i],"-size_max")==0)
						{
								sscanf(argv[i+1], "%lld", &maxsize);
								check_opt[1] = true;
						}
				}
		}
		dir = opendir(path);
		//cout << "check min size : " << minsize << endl;
		//cout << "check node : " << inode << endl;
		find(dir, path);
		free(name);
		return 0;
}
