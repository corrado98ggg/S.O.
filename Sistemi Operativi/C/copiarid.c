#include <stdio.h>
#include <unistd.h>

/*stesse prorprieta del comando cat, ma solo come filtro*/

int main(){
	char buffer [BUFSIZ];
	int nread;

	while((nread = read(0, buffer, BUFSIZ)) > 0)
/*lettura dello standard input fino a che ci sono caratteri*/
	write(1, buffer, nread);
/*scrittura dello standard output dei caratteri letti*/
	return 0;
}
