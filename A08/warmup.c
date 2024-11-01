/*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 1/11/24
 * Description: A warmup to create children with 
 * fork that print pid along with different 
 * alphabet and numerical combinations . 
 ---------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	int status;
	printf("%d", getpid());
	printf("]  A0\n");
	pid = fork();
	if (pid != 0){
		 printf("%d", getpid());
        	 printf("]  B0\n");
		 }
	else {
		printf("%d", getpid());
		printf("]  B1\n");
		printf("%d", getpid());
                printf("]  Bye\n");
	}
	if (pid != 0){
		pid = wait(&status);
		pid = fork();
		if (pid != 0){

			printf("%d", getpid());
                	printf("]  C0\n");
                	printf("%d", getpid());
                	printf("]  Bye\n");
		}
		if (pid == 0){
			printf("%d", getpid());
                	printf("]  C1\n");
                	printf("%d", getpid());
                	printf("]  Bye\n");
		}
	}
	

  return 0;
}
