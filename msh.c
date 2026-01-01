// SO-P2-22/23

// MSH main file
// Write your msh source code here

//#include "parser.h"
#include <stddef.h>         /* NULL */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <ctype.h>

#define MAX_COMMANDS 8


// files for redirection
char filev[3][64];

//to store the execvp second parameter
char *argv_execvp[8];


void siginthandler(int param)
{
	printf("****  Exiting MSH **** \n");
	//signal(SIGINT, siginthandler);
	exit(0);
}


/* Timer */
pthread_t timer_thread;
unsigned long  mytime = 0;

void* timer_run ( )
{
	while (1)
	{
		usleep(1000);
		mytime++;
	}
}

/**
 * Get the command with its parameters for execvp
 * Execute this instruction before run an execvp to obtain the complete command
 * @param argvv
 * @param num_command
 * @return
 */
void getCompleteCommand(char*** argvv, int num_command) {
	//reset first
	for(int j = 0; j < 8; j++)
		argv_execvp[j] = NULL;

	int i = 0;
	for ( i = 0; argvv[num_command][i] != NULL; i++)
		argv_execvp[i] = argvv[num_command][i];
}


/**
 * Main shell  Loop  
 */
int main(int argc, char* argv[])
{
	
	/**** Do not delete this code.****/
	int end = 0; 
	int executed_cmd_lines = -1;
	char *cmd_line = NULL;
	char *cmd_lines[10];

	if (!isatty(STDIN_FILENO)) {
		cmd_line = (char*)malloc(100);
		while (scanf(" %[^\n]", cmd_line) != EOF){
			if(strlen(cmd_line) <= 0) return 0;
			cmd_lines[end] = (char*)malloc(strlen(cmd_line)+1);
			strcpy(cmd_lines[end], cmd_line);
			end++;
			fflush (stdin);
			fflush(stdout);
		}
	}

	pthread_create(&timer_thread,NULL,timer_run, NULL);

	/*********************************/

	char ***argvv = NULL;
	int num_commands;


	while (1) 
	{
		int status = 0;
		int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		// Prompt 
		write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

		// Get command
		//********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
		executed_cmd_lines++;
		if( end != 0 && executed_cmd_lines < end) {
			command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
		}
		else if( end != 0 && executed_cmd_lines == end){
			return 0;
		}
		else{
			command_counter = read_command(&argvv, filev, &in_background); //NORMAL MODE
		}
		//************************************************************************************************


		/************************ STUDENTS CODE ********************************/
		if (command_counter > 0) {
			if (command_counter > MAX_COMMANDS){
				printf("Error: Maximum number of commands is %d \n", MAX_COMMANDS);
			}
			else {	
				// We check if the first argument is "mycalc"			
				if((strcmp (argvv[0][0],"mycalc") == 0)) {     
					// Create the required error messages
					char calc_error_msg[100] = "[ERROR] The command structure is mycalc <operand_1> <add/mul/div> <operand_2>\n";
					char zero_error_msg[75] = "[ERROR] Cannot divide by zero\n";
					              
					// Check if any necessary parameters for mycalc are missing to return the error through standard output
                        		if ((argvv[0][1] == NULL) || (argvv[0][2] == NULL) || (argvv[0][3] == NULL) ) 
                        		{
                        			if((write(1, calc_error_msg, strlen(calc_error_msg))) <0){
								perror ("Error writing the error");
						}
						// We exit so mycalc doesn't continue executing
						exit(-1);
                            			
                       		}
                       		
                       		// Check if an extra parameter is passed in mycalc to return the error through standard output
                       		if ((argvv[0][4] != NULL) ) 
                        		{
                        			if((write(1, calc_error_msg, strlen(calc_error_msg))) <0){
								perror ("Error writing the error");
						}
						// We exit so mycalc doesn't continue executing
						exit(-1);
                            			
                       		}
                       		
                       		/* We initialize the necessary variables to loop through each character of the passed argument
                       		and verify if it is an integer by character comparison. We use a variable
                       		called 'condition' that serves as a flag to return errors in specific cases. */
                       		int i = 0, condition = 1;
                       		// We check if operand_1 contains anything other than a number, and if so, we break the loop and change the condition
                       		while(argvv[0][1][i]) {
                       			
						if(argvv[0][1][i] >= '0' && argvv[0][1][i] <= '9') {
						    condition = 0;
						    break;
						}
						i++;
					    }
					    
					// If a non-numeric operand is found, we throw the error to standard output
					if(condition) {
											
						if((write(1, calc_error_msg, strlen(calc_error_msg))) <0){
								perror ("Error writing the error");
						}
						// We exit so mycalc doesn't continue executing
						exit(-1);
					    }
					
					// We reinitialize the condition variable with its original value
					condition = 1;
					// We check if operand_2 contains anything other than a number, and if so, we break the loop and change the condition
					while(argvv[0][3][i]) {
						
						if(argvv[0][3][i] >= '0' && argvv[0][3][i] <= '9') {
						    condition = 0;
						    break;
						}
						i++;
					    }
					
					// If a non-numeric operand is found, we throw the error to standard output
					if(condition) {
						
						if((write(1, calc_error_msg, strlen(calc_error_msg))) <0){
								perror ("Error writing the error");
						}
						// We exit so mycalc doesn't continue executing
						exit(-1);
					    }
					    
					    
					// If another command is passed, we throw the error to standard output
                       		if (command_counter > 1){
						if((write(1, calc_error_msg, strlen(calc_error_msg))) <0){
								perror ("Error writing the error");
						}
						// We exit so mycalc doesn't continue executing
						exit(-1);
					}
					
					// If the format is correct, we perform the operation:
                       		else{
                       			// Create the strings that will hold the different messages associated with their operation type
                       			char sum_msg[100], mul_msg[100], div_msg[100];
                       			// We cast the arguments corresponding to the operators to integers
                       			int num1 = atoi(argvv[0][1]);
					        int num2 = atoi(argvv[0][3]);
					        // We declare and initialize the Acc environment variable
					        char *env_var;
						int acc = 0;
					        
					        // We check if the argument corresponding to the operation is "add"
					        if (strcmp(argvv[0][2], "add") == 0) {
					        	// We store the result of adding both operands in a variable called sum
					        	int sum = num1 + num2;
					        	// We get the value of the Acc environment variable and check if Acc is null to determine if it is already defined
							env_var = getenv("Acc");
							if (env_var != NULL) {
								acc = atoi(env_var);
															
							}
							
							/* We update the environment variable after casting it to an integer and save it in an auxiliary string
							in order to set the new calculated value for the environment variable */
							acc += sum;
							char new_env_var[10];
						        sprintf(new_env_var, "%d", acc);
						        setenv("Acc", new_env_var, 1);
						        
						        /* We get the value of the environment variable again, but to assign a value to the output string that will be shown
						         on the standard error output */							
							sprintf(sum_msg, "[OK] %i + %i = %i; Acc %s\n",num1, num2, sum, getenv("Acc"));							
							if((write(2, sum_msg, strlen(sum_msg))) <0){
									perror("Error writing the error");
							
					        	
					        	}
					        }
					       
					        // We check if the argument corresponding to the operation is "mul"
					        else if (strcmp(argvv[0][2], "mul") == 0) {
					        
					        	/* We assign the respective operational values to the string related to the multiplication and show it on the
					        	standard error output */
					        	
					        	sprintf(mul_msg, "[OK] %d * %d = %d\n", num1, num2, num1 * num2);							
							if((write(2, mul_msg, strlen(mul_msg))) <0){
									perror("Error writing the error");
							}
					        }
					        
					        // We check if the argument corresponding to the operation is "div"
					    	else if (strcmp(argvv[0][2], "div") == 0) {
					    	
					    		// If operand_2 is 0, we must show the related error on the standard output
							if (num2 == 0) {
								if((write(1, zero_error_msg, strlen(zero_error_msg))) <0){
									perror("Error writing the error");
						    		}
						    		
							}
							else {
								/* We assign the respective operational values to the string related to the division and show it on the
					        		standard error output */
								
								sprintf(div_msg, "[OK] %d / %d = %d; Remainder %d\n", num1, num2, num1 / num2, num1 % num2);
						    		if((write(2, div_msg, strlen(div_msg))) <0){
									perror("Error writing the error");
						       	}
							}
					   	}
					        else {
					        
					        	/* If the corresponding argument is not equal to any of the previous ones, the operation has been passed incorrectly and we show the error
					        	on the standard output */
						       if((write(1, calc_error_msg, strlen(calc_error_msg))) <0){
								perror ("Error writing the error");
							}
					    	}
                       		
                       			
                       		                       		                       		                       		                       		
                       		}

			
				}
				// We check if the first argument is "mytime"
				else if ((strcmp (argvv[0][0],"mytime") == 0)) {
					// Create the required error message
					char time_error_msg[100] = "[ERROR] The command structure is mytime\n"; 
					// If another argument is passed, we throw the error to the error output (our choice since it was not specified)
					if (argvv[0][1] != NULL){
						if((write(2, time_error_msg, strlen(time_error_msg))) <0){
							perror("Error writing the error");  
							
						}
						// We exit so mytime doesn't continue executing
						exit(-1);
						
					}
					
					// If another command is passed, we throw the error to the error output (our choice since it was not specified)
					if (command_counter > 1) 
                        		{
                            			if((write(2, time_error_msg, strlen(time_error_msg))) <0){
							perror("Error writing the error");   
							
						}
						// We exit so mytime doesn't continue executing
						exit(-1);
						
                       		}			
                       			
                       		// We perform the respective conversions based on the mytime variable provided
					int seconds = (mytime / 1000) % 60;
					int minutes = (mytime / (1000 * 60)) % 60;
					int hours = (mytime / (1000 * 60 * 60)) % 24;
					
					// The requested char format will be saved with the help of sprintf in a string called 'time'
					char time_str[10];
					sprintf(time_str, "%02d:%02d:%02d\n", hours, minutes, seconds);
					
					// We return the time string to the error output (as requested in the statement)
					if((write(2, time_str, strlen(time_str))) <0){
						perror("Error writing mytime");
					}
					
                		        	
	       		}
							
				else{	
				
					int fd[2]; // pipe read/write descriptor
					int pid; // process pid
					int pipe_n; // here the pipe will be created
					int pipe_buf; // placeholder for redirections
					int last_comm = command_counter-1; // # of the last command in the sequence
					int file_desc;				
												
					
					/* We iterate through each concatenated command we have, that is, through the command_counter.
					Each iteration creates a new fork, and depending on what it returns, a different action will be taken.
					 */
					for (int comm_n =0 ; comm_n < command_counter; comm_n++)
					{
						/* We check that the current command is not the last one before creating the pipe.
						Since it will not have a subsequent process to pass the output to. */
						if (comm_n != (last_comm)){
							if ((pipe_n = pipe(fd))<0){
								perror("Error creating the pipe");
								exit(-1);
							}
						}
						
						
						// Create the process
						pid = fork();

						switch (pid){
						// CASE PID = -1: ERROR IN THE CHILD
						case -1:
						if (command_counter > 1){
							if(close(fd[0]) < 0 || close(fd[1]) < 0){
								perror("Error closing descriptor");
								exit(-1);
							}
							
						}
						return (-1); 
					
						// CASE PID = 0: CHILD EXECUTION
						case 0:
						
						
						if (strcmp(filev[0],"0")!=0 )
								{
								
								
								    file_desc = open(filev[0], O_RDONLY);
								    if (file_desc == -1) {
								    	perror("File does not exist");
									exit(1);
								    }
								    else {
									dup2(file_desc, 0); 
									
											
								    }
								    
								    
								 
								}
						
						
						if (comm_n != 0){
							// If it is not the first process, we redirect the input
							if (dup2(pipe_buf,0) < 0){
								perror("Redirection error (input)");
								exit(-1);
							}
							// After redirecting the standard input, we clean the pipe
							if (close(pipe_buf) < 0){
								perror("Error closing the pipe input");
								exit(-1);
							}
						}
						if (comm_n != last_comm){	
							// If it is not the last process, we redirect the output
							if (dup2(fd[1],1) < 0){
								perror("Redirection error (output)");
								exit(-1);
							}
							// After redirecting the standard output, we clean the pipe
							if (close(fd[1]) <0 ){
								perror("Error closing the pipe output");
								exit(-1);
							}
						}
						
						
						if (comm_n == last_comm ){
						// We check if it is the last process to redirect the output of the last command of the pipes to the file if it exists.
							/*if (strcmp(filev[0],"0")!=0)
								{
								
								
								    file_desc = open(filev[0], O_RDONLY);
								    if (file_desc == -1) {
								    	perror("File does not exist");
									exit(1);
								    }
								    else {
									dup2(file_desc, 0); 
									
											
								    }
								    
								    
								 
								}	*/					  						
								
							if(strcmp(filev[1],"0")!=0)                           
									{	
										file_desc = open(filev[1], O_WRONLY | O_TRUNC); 
										if (file_desc == -1) {
								    			file_desc = open(filev[1], O_WRONLY | O_CREAT, 0666);
											if (file_desc == -1) {
												perror("Could not create the file");
												exit(1);
											} 
											else {									
												dup2(file_desc, 1); 

									    		}
								    		}
								    		else {
											dup2(file_desc, 1); 
											
										}
									
										
									}
								
								
							if(strcmp(filev[2],"0")!=0) 
									{	
									    file_desc = open(filev[2], O_WRONLY | O_TRUNC); 
									    if (file_desc == -1) {
								    			file_desc = open(filev[2], O_WRONLY | O_CREAT, 0666);
											if (file_desc == -1) {
												perror("Could not create the file");
												exit(1);
											} 
											else {									
												dup2(file_desc, 2); 

									    		}
								    	    }
								    	    else {
											dup2(file_desc, 2); 
											
									    }
									    
									}
							
									
									
											 
						}
						
						
											
						
						
						
						
						
						
						// We execute the current command
						execvp(argvv[comm_n][0], argvv[comm_n]);
						
						// If everything went well, it should not get here, so an error is printed
						perror("Error executing execvp\n");
						exit(-1);
	
						// CASE PID > 0: PARENT EXECUTION
						default:
						/* If it is the last command of the sequence, we close the input
						except in the case where there is only 1 command, since pipes will not be used */
						if (comm_n == last_comm){
							
							if(command_counter > 1){
								
								if (close(fd[0])<0){
									perror("Error closing the pipe input");
									exit(-1);
									}
							}	
						}
						/* If it is not the last command, we close the output and redirect the input
						to adapt to the next one */
						else {
							if (close(fd[1])<0){
								perror("Error closing the pipe output");
								exit(-1);
							}
							pipe_buf = fd[0];	
						}
						/* In case the process does not have to be in the background, the parent waits
						until the child finishes, if it is in the background it does nothing (this allows
						commands to be executed in the meantime) */
						if(in_background == 0){
						while (pid != wait(&status));
						if (status <0){
							perror("Error in child process");
						}
						}
					
					
						}
						
					
					}
								
					
				
				
				
				}
			
				// Print command
				//print_command(argvv, filev, in_background);
			}
		}
	}
	
	return 0;
}