#include  <stdio.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_NUM_COMMANDS 64
#define FOREGROUND_GROUP_ID 1234

int num_bg_procs = 0;
pid_t* bg_procs;
pid_t foreground_proc = -1;

/* Splits the string by space and returns the array of tokens
*
*/

int count_tokens(char** tokens){
	int n_tokens = 0;
	while(tokens[n_tokens]){
		n_tokens++;
	}
	return n_tokens;
}

void add_bg_proc(pid_t proc){
	bg_procs = realloc(bg_procs, (num_bg_procs+1)*sizeof(pid_t));
	bg_procs[num_bg_procs] = proc;
	num_bg_procs++;
	return;
}

void remove_bg_proc(pid_t proc){
	for(int i=0; i<num_bg_procs; i++){
		if(bg_procs[i]==proc){
			for(int j=i; j<num_bg_procs-1; j++){
				bg_procs[j] = bg_procs[j+1];
			}
			bg_procs = realloc(bg_procs,(num_bg_procs-1)*sizeof(pid_t));
			num_bg_procs--;
			return;
		}
	}
	printf("ERROR: remove_bg_proc(proc) -> proc not found in bg_procs\n");
	return;
}

void terminate_all_bg_procs(){
	for(int i=0; i<num_bg_procs; i++){
		kill(bg_procs[i],9);
		waitpid(bg_procs[i],NULL,WNOHANG);
	}
	free(bg_procs);
}

void handle_sigint(int signal){
	if(foreground_proc>0){
		kill(foreground_proc,SIGINT);
	}
	kill(-FOREGROUND_GROUP_ID,SIGINT);
}

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void execute_simple_commands(char* tokens[]){
	printf("Entered the function\n");
	pid_t child = fork();
	if(child<0){
		perror("Error creating a child process");
	}
	else if(child>0){
		foreground_proc = child;
		int x = waitpid(child,NULL,0);
		foreground_proc = -1;
		if(x==-1){
			perror("Error");
		}
		int y = waitpid(-1,NULL,WNOHANG);
		if(y>0){
			remove_bg_proc(y);
			printf("Shell: Background process finished\n");
		}
	}
	else{
		execvp(tokens[0],tokens);
		perror("Error executing the command");
	}
}

void execute_serially(char*** commands, int num_commands){
	for(int i=0; i<num_commands; i++){
		pid_t child = fork();
		if(child <0){
			perror("Error creating a child process");
		}
		else if(child>0){
			foreground_proc = child;
			int x = waitpid(child,NULL,0);
			foreground_proc = -1;
			if(x==-1){
				perror("Error");
			}
			int y = waitpid(-1,NULL,WNOHANG);
			if(y>0){
				remove_bg_proc(y);
				printf("Shell: Background process finished\n");
			}
			
		}
		else{
			execvp(commands[i][0],commands[i]);
			perror("Error executing the command");
		}
	}
	
}

void execute_parallely(char*** commands,int num_commands){
	for(int i=0; i<num_commands; i++){
		pid_t child = fork();
		if(child<0){
			perror("Error creating a child process");
		}
		else if(child == 0){
			execvp(commands[i][0],commands[i]);
			perror("Error executing the command");
		}
	}
	for(int i=0; i<num_commands; i++){
		setpgid(0,FOREGROUND_GROUP_ID);
		int x = wait(NULL);
		if(x==-1){
			perror("Error");
		}
	}
}

void execute_pipes(char*** commands,int num_commands){
	// executing_command = -1;
	
	pid_t child = fork();
	if(child<0){
		perror("Error forking");
		exit(1);
	}
	else if(child>0){
		foreground_proc = child;
		int x = waitpid(child,NULL,0);
		foreground_proc = -1;
		if(x==-1){
			perror("Error");
		}
		int y = waitpid(-1,NULL,WNOHANG);
		if(y>0){
			remove_bg_proc(y);
			printf("Shell: Background process finished\n");
		}
	}
	else{
		int fd[2];
		if(pipe(fd)==-1){
			perror("pipe: ");
			return;
		}
		int child2 = fork();
		if(child2==-1){
			perror("Error forking");
			exit(1);
		}
		else if(child2==0){
			close(fd[0]);
			dup2(fd[1],STDOUT_FILENO);
			close(fd[1]);
			execvp(commands[0][0],commands[0]);
			perror("Error executing the command");
		}
		else{
			close(fd[1]);
			waitpid(child2,NULL,0);
			char buffer[MAX_INPUT_SIZE];
			ssize_t bytes;
			if ((bytes = read(fd[0], buffer, sizeof(buffer) - 1)) > 0) {
				buffer[bytes] = '\0';
			}
			char** tokens = tokenize(buffer);
			close(fd[0]);
			execvp(tokens[0],tokens);
			execvp(commands[0][0],commands[0]);
			perror("Error executing the command");	
		}

	}

}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	signal(SIGINT,handle_sigint);


	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
		int ntokens = count_tokens(tokens);
		if(ntokens==0) continue;

		int lastPos = -1;
		int num_commands = 0;
		char ***commands = (char ***)malloc(MAX_NUM_COMMANDS*sizeof(char **));
		char **command = (char **)malloc(MAX_NUM_TOKENS*sizeof(char *));

		int three_and_exec = 0;
		int two_and_exec = 0;
		int pipes = 0;

		int commandIndex = 0;
		for(int i=0; i<ntokens; i++){
			if(strcmp(tokens[i],"&&")==0) two_and_exec = 1;
			if(strcmp(tokens[i],"&&&")==0) three_and_exec = 1;
			if(strcmp(tokens[i],"|")==0) pipes=1;
		}
		if((two_and_exec && three_and_exec) || (three_and_exec && pipes) || (two_and_exec && pipes)) {
			printf("USe simples commands between complex operators like &&&, && and |\n");
			continue;
		}

		for(int i=0; i<=ntokens; i++){
			if(i==ntokens || strcmp(tokens[i],"&&")==0 || strcmp(tokens[i],"&&&")==0 || strcmp(tokens[i],"|")==0){
				command[commandIndex] = NULL;
				commands[num_commands++] = command;
				command = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
            	commandIndex = 0;
			}
			else{
				command[commandIndex] = (char *)malloc(MAX_TOKEN_SIZE*sizeof(char ));
				strcpy(command[commandIndex++],tokens[i]);
			}
			
		}

		if(num_commands > 1){
			if(two_and_exec)execute_serially(commands, num_commands);
			else if(three_and_exec){
				execute_parallely(commands, num_commands);
			}
			else if(pipes){
				execute_pipes(commands, num_commands);
			}
			for(int i=0; i<num_commands; i++){
				for(int j=0; commands[i][j]!=NULL; j++){
					free(commands[i][j]);
				}
				free(commands[i]);
			}
			free(commands);
			continue;
		}

		
		if(strcmp(tokens[0],"exit")==0){
			terminate_all_bg_procs();
			free(tokens[0]);
			free(tokens);
			break;
		}


		if(strcmp(tokens[0],"cd")==0){
			if(chdir(tokens[1])!=0){
				perror("ERROR");
				for (int i = 0; i < ntokens; ++i) {
					printf("%s ", tokens[i]);
				}
				printf("\n");
				
			}
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);
			continue;
		}

		int is_bg_proc = 0;
		
		if(strcmp(tokens[ntokens-1],"&")==0){
			is_bg_proc = 1;
			free(tokens[ntokens]);
			tokens[ntokens-1] = NULL;
		}
	
		pid_t child = fork();
		if(child<0){
			perror("Error creating a child process");
		}
		else if(child>0){
			if(!is_bg_proc){
				foreground_proc = child;
				int x = waitpid(child,NULL,0);
				foreground_proc = -1;
				if(x==-1){
					perror("Error");
					return 1;
				}
			}
			else{
				add_bg_proc(child);
			}
			int y = waitpid(-1,NULL,WNOHANG);
			if(y>0){
				remove_bg_proc(y);
				printf("Shell: Background process finished\n");
			}
		}
		else{
			if(is_bg_proc) setpgid(0,0);
			execvp(tokens[0],tokens);
			perror("Error executing the command");
		}
		




		
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
