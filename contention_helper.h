#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#define BLOCKSIZE 4096

pid_t *pids = NULL;
int NUM = 0;

size_t _get_file_size(const char *filename) {
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

void readFile(int num) {

	char *buf = (char *) malloc(10);
	size_t filesize = _get_file_size((const char *)buf);
	char *readBuf = (char *)malloc(BLOCKSIZE);
	memset(buf, '\0', 10);
	sprintf(buf, "%d.txt", num);
	int fd = open(buf, O_RDONLY);
	int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);
	while (1) {
		read(fd, readBuf, BLOCKSIZE);
		lseek(fd, 0, SEEK_SET);
	}

}

void startBackgroundJobs(int num_processes)
{
	NUM = num_processes;
	pids  = (pid_t *)malloc(num_processes * sizeof(pid_t));

	for (int i = 0; i < num_processes; ++i)
	{
		pid_t child = fork();
		if (child == 0) {
			readFile(i);
		}
		else {
			pids[i] = child;
		}
	}
}

void killJobs() {
	for (int i=0; i<sizeof(pid_t)*NUM/sizeof(pid_t); i++) {
	//	printf("Killing process %d PID: %d", i, (int)pids[i]);
		kill(pids[i], SIGKILL);
	}
	free(pids);
	NUM = 0;
}
