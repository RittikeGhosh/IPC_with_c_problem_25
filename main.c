#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 1000

void caseSwap(char* str)
{
	for(int i = 0; str[i] != '\0'; i++)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		else if(str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	}
}


int main()
{
	system("clear");
	printf("#########################################################\n");
	printf("#                                                       #\n");
	printf("#                         WELCOME                       #\n");
	printf("#      This Program illustrates the working of IPC      #\n");
	printf("#        The program will transform letter cases        #\n");
	printf("#            e.g Hello World! --> hELLO wORLD!          #\n");
	printf("#                                                       #\n");
	printf("#########################################################\n\n\n");

	char p1_buffer[MAX_LENGTH];  // for input and pass to the second program
	char p2_buffer[MAX_LENGTH];  // for reading from first process and process text
	char outputBuffer[MAX_LENGTH];  // for reading from second process and output string

	int p1_pipeFd[2], p2_pipeFd[2];  // fd for the pipe : [0:read, 1:write]

	int err_code1 = pipe(p1_pipeFd);	 // creating first/parent prog pipe
	int err_code2 = pipe(p2_pipeFd);  // creatint second/child prog pipe

	// error check
	if (err_code2 == -1 || err_code1 == -1)
	{
		printf("Error Occured ! process aborted ...");
		return 0;
	}


	printf("=> Executing First program[pid:%d]  \n", getpid());
	// string input
	printf("Enter the String[MAX_LENGTH = 1000] : ");
	scanf("%[^\n]%*c", p1_buffer);

	int fork_id = fork();
	// process p1
	if(fork_id > 0)
	{
		// delete unwanted pipe channels
		close(p1_pipeFd[0]);
		close(p2_pipeFd[1]);

		printf("*(Passing string to the next process[pid:%d])\n", fork_id);
		// write the string to the pipe p1_pipeFd[1]
		write(p1_pipeFd[1], p1_buffer, sizeof(p1_buffer));
		printf("---------------------------------------------------\n\n");
		wait(NULL);  // wait for the child process to finish

		sleep(1);
		printf("=> Back to first process[pid:%d]\n", getpid());
		printf("[Reading the data in the read channel of process 2 pipe]\n");
		// read from the p2_pipeFd[0] which is comming from second process
		int size = read(p2_pipeFd[0], outputBuffer, sizeof(outputBuffer));
		printf("String returned by the second process[pid:%d]: %s\n", fork_id, outputBuffer);
		printf("---------------------------------------------------\n");

		// final output...
		sleep(1);
		printf("\nFinally, (...)\n=>\t| %s ---> %s |\n\n", p1_buffer, outputBuffer);
	}
	else
	{
		sleep(1);
		// closing unwanted pipe channels
		close(p1_pipeFd[1]);
		close(p2_buffer[0]);

		printf("=> Executing second process[%d]\n", getpid());
		printf("[Reading the data in the read channel of process 1 pipe]\n");
		// reading from the p1_pipeFd[0]
		read(p1_pipeFd[0], p2_buffer, sizeof(p2_buffer));
		printf("String recieved from the first process [%d] : %s \n", getppid(), p2_buffer);	

		//case transform
		caseSwap(p2_buffer);

		printf("Processed String is : %s\n", p2_buffer);
		printf("*(Passing the processed data back to first process[%d])\n", getppid());
		write(p2_pipeFd[1], p2_buffer, sizeof(p2_buffer));
		printf("---------------------------------------------------\n");
	}

	printf("(process id: %d finished)\n\n", getpid());
	return 0;
}
