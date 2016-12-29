#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	char filename[256];
	int fd;
	struct stat buf;
	
	if (argc == 1) 
		*filename = '\0'; //empty string
	else if (argc == 2){
		if (strlen(argv[1]) >= 256){ 
			printf("The controlled path is too long! please check it and try again! \n");
			exit(1);
		}
		strcpy(filename, argv[1]);
		if (stat(filename,&buf) != 0){
			printf("The file(or directory) may not exist! \n");
			exit(1);
		}
	}else{
		printf("Commandline parameters are wrong! please check them! \n");
		printf("Usage: %s, or %s file(directory) name! \n",argv[0],argv[0]);
		exit(1);
	}

	if (stat("/dev/controlfile",&buf) != 0){
		if (system("mknod /dev/controlfile c 123 0") == -1){
			printf("Cann't create the devive file ! \n");
			printf("Please check and try again! \n");
			exit(1);
		}
	}
	fd =open("/dev/controlfile",O_RDWR,S_IRUSR|S_IWUSR);
	if (fd > 0)
	{
		write(fd,filename,strlen(filename));
	}
	else {
		perror("can't open /dev/controlfile \n");
	 	exit (1);
	}
	close(fd);
}
