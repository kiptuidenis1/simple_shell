#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

/**
 * display_prompt - Displays a simple prompt.
 */
void display_prompt(void)
{
	printf("#cisfun$ ");  /* Display a simple prompt */
}

/**
 * read_command - Reads a command line from stdin.
 *
 * Return: The command line string.
 */
char *read_command(void)
{
	char *command = NULL;
	size_t buffer_size = 0;

	ssize_t num_chars = getline(&command, &buffer_size, stdin);

	if (num_chars == -1)
	{
		free(command);
		return (NULL);
	}

	return (command);
}

/**
 * execute_command - Executes a command.
 * @command: The command to execute.
 */
void execute_command(char *command)
{
	pid_t pid;

	command[strcspn(command, "\n")] = '\0';  /* Remove the trailing
						  *newline character
						  */

	pid = fork();  /* Fork a child process */
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		/* Child process */
		char *argv[2];

		argv[0] = command;
		argv[1] = NULL;

		if (execvp(command, argv) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process */
		int status;

		waitpid(pid, &status, 0);  /* Wait for the child process
					    *to finish
					    */
	}
}

/**
 * main - Entry point of the program.
 *
 * Return: Always 0.
 */
int main(void)
{
	while (1)
	{
		char *command = read_command();  /* Read the command line */

		display_prompt();  /* Display the prompt */
		if (command == NULL)
		{
			printf("\n");
			break;  /* Exit the shell if end of file (Ctrl+D) is
				 *encountered
				 */
		}

		execute_command(command);  /* Execute the command */

		free(command);  /* Free the allocated memory */
	}

	return (0);
}
