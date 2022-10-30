#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;

int main()
{
    pid_t pid, pid1;
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork fail");
        return 1;
    }
    else if (pid == 0)
    {
        int status;
	    if ((status = system (NULL)) == -1)
		    perror ("system"), exit (1);
	    if (!status)
		    printf ("shell 不可用\n");
	    else {
  		    if ((status = system ("ls -al")) == -1)
			    perror ("system"), exit (1);
		    printf ("WEXITSTATUS (status) = %d\n", WEXITSTATUS (status));
	    }
    }
    else
    {
        //while(1) {sleep(2);}
        wait(NULL);
    }
    return 0;
}