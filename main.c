#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
// #include <fnctl.h>

int main()
{
	char p1_buffer[200], p2_buffer[200], outputBuffer[200];  // for input/output string
	int p1_pipeFd[2], p2_pipeFd[2];  // fd for the pipe : [0:read, 1:write]

	int err_code1 = pipe(p1_pipeFd);	 // creating first/parent prog pipe
	int err_code2 = pipe(p2_pipeFd);  // creatint second/child prog pipe

	if (err_code2 == -1 || err_code1 == -1)
	{
		printf("Error Occured ! process aborted ...");
		return 0;
	}

	printf("Executing First program [%d]  \n", getpid());
	// string input
	printf("Ennter the String : ");
	scanf("%[^\n]%*c", p1_buffer);
	printf("\n\n");

	int fork_id = fork();
	if(fork_id > 0)
	{
		// delete unwanted pipe channels
		close(p1_pipeFd[0]);
		close(p2_pipeFd[1]);
	
		printf("Passing string to the next process...\n\n");
		// write the string to the pipe p1_pipeFd[1]
		write(p1_pipeFd[1], p1_buffer, strlen(p1_buffer));

		wait();  // wait for the child process to finish

		printf("Back to first process \n");
		// read from the p2_pipeFd[0] which is comming from second process
		int size = read(p2_pipeFd[0], outputBuffer, sizeof(outputBuffer));
		printf("Processed string is : %s", outputBuffer);

		// termination message
		printf("The process successfully completed \n");
	}
	else
	{
		// closing unwanted pipe channels
		close(p1_pipeFd[1]);
		close(p2_buffer[0]);

		printf("Now in the second process [%d] \n", getpid());
		printf("Reading the data in the pipe...\n");
		// reading from the p1_pipeFd[0] 
		read(p1_pipeFd[0], p2_buffer, sizeof(p2_buffer));
		printf("String recieved from the first process [%d] : %s \n", getppid(), p2_buffer);	

		//case transform 
		for(int i = 0; p2_buffer[i] != '\0'; i++)
		{
			if(p2_buffer[i] >= 'A' && p2_buffer[i] <= 'Z')
				p2_buffer[i] += 32;
			else if(p2_buffer[i] >= 'a' && p2_buffer[i] <= 'z')
				p2_buffer[i] -= 32;
		}

		printf("Processed String is : %s  \n", p2_buffer);
		printf("Passing the processed data back to first proces : [%d] -> [%d] \n\n", getpid(), getppid());
	}

	return 0;
}
