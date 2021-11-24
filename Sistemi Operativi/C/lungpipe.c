#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>


int count;

int main(){
	int piped[2];
	char c = 'x';


	if(pipe(piped) < 0){ printf("erorre\n"); exit(1);}

	for(count = 0;;){
		write(piped[1], &c, 1);

		if((++count % 1024)==0){
			printf("%d caratteri nelle pipe\n", count);
		}
	}
	exit(0);
}
