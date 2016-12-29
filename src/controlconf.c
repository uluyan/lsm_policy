#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	char confile[256];
	char pid[256];
	char flags[256];
	int fd;
	struct stat buf;

	strcpy(pid, argv[1]);
	strcpy(flags, argv[2]);

	int i;
	for(i=0;i<strlen(pid);i++){
		confile[i] = pid[i];
	}

	for(i=0;i<strlen(flags);i++){
		confile[i+strlen(pid)] = flags[i];
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
