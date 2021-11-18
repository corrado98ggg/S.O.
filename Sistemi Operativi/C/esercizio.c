#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int fd;
	char c;

	if(argc != 3) {
		puts("Errore nel numero di param");
		exit(1);
	}

	if((fd = open(argv[1], O_RDWR)) <0){
		puts("errore in apertura file");
		exit(2);
	}

	if(strlen(argv[2]) != 1){
		puts("errore non carattere");
		exit(3);
	}
	
	while(read(fd, &c, 1)){
		if(c == argv[2][0]){
			lseek(fd, -1L, 1);

			/*SI DEVE riportare indietro il file pointer*/
			write(fd, " ", 1);
		}
		return 0;
	}
}
