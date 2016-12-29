#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int fd;
	struct stat buf;
	char confile[256];
	char pid[256];
	char flags[256];

	strcpy(pid, argv[1]);
	strcpy(flags, argv[2]);

	int i;
	for(i=0;i<strlen(pid);i++){
		confile[i] = pid[i];
	}

    confile[strlen(pid)] = ':';

	for(i=0;i<strlen(flags);i++){
		confile[i+strlen(pid)+1] = flags[i];
	}
	
	if (argc == 2) 
		*confile = '\0';

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
		write(fd,confile,strlen(confile));
	}
	else {
		perror("can't open /dev/controlfile \n");
	 	exit (1);
	}
	close(fd);
}
