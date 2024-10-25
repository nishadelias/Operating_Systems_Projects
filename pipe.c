#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  // ./pipe counts as one  argument
  if (argc <= 1) {
    perror("ERROR: must provide at least one argument");
    errno = EINVAL;
    exit(errno);
  }
  else if (argc == 2) {
    // execute the one command using execlp

    // create a child process
    pid_t pid = fork();

    if (pid < 0) {
      perror("fork");
      exit(errno);
    }
    else if (pid == 0) {
      // child process
      // first arg is program name and second is command to execute
      execlp(argv[1], argv[1], (char *)NULL);

      // if execlp returns then it failed
      perror("execlp");
      exit(errno);
    }
    else {
      // parent must wait for child to finish so that no orphan process
      int status;
      waitpid(pid, &status, 0);
      if (WEXITSTATUS(status) != 0) {
	perror("ERROR: Child process did not exit correclty");
      }
    }

 
  }
  // argc >= 3
  else {
    int prev_pipe[2];
    pid_t pid;
    int status;
    // array large enough to store all child pids
    pid_t *child_pids = malloc(sizeof(pid_t) * (argc - 1));

    // loop through commands except last one
    for (int i = 1; i < argc; i++) {
      
      // create new pipe except for last command
      int curr_pipe[2];
      
      if (i < argc - 1 && pipe(curr_pipe) < 0) {
	perror("ERROR: Could not create pipe");
	exit(errno);
      }

      pid = fork();
      if (pid < 0) {
	perror("ERROR: fork failed");
	exit(errno);
      }
      // child process
      else if (pid == 0) {
	
	// if not first command, get input from previous pipe
	if (i > 1) {
	  dup2(prev_pipe[0], 0);
	  // close read end of prev  pipe
	  close(prev_pipe[0]);
	  // close write end of prev pipe
	  close(prev_pipe[1]);
	}
	
	// if not last command, output to curr pipe
	if (i < argc - 1) {
	  dup2(curr_pipe[1], 1);
	  close(curr_pipe[0]);
	  close(curr_pipe[1]);
	}
	
	execlp(argv[i], argv[i], (char *)NULL);

	perror("ERROR: execlp failed");
	exit(errno);
      }
  
      // parent process
      else {
	// close write end of prev pipe
	if (i > 1) {
	  close(prev_pipe[0]);
	  close(prev_pipe[1]);
	}

	// save curr pipe as prev pipe for next iteration
	if (i < argc - 1) {
	  prev_pipe[0] = curr_pipe[0];
	  prev_pipe[1] = curr_pipe[1];
	}
	
	// save for later waiting
	child_pids[i - 1] = pid;
      }
    
    }

    // close the last pipe in the chain
    // this check is only necessary if multiple pipes
    if (argc > 3) {
      close(prev_pipe[0]);
      close(prev_pipe[1]);
    }

    // wait for all children
    for (int j = 0; j < argc - 1; j++) {
      waitpid(child_pids[j], &status, 0);
      if (WEXITSTATUS(status) != 0) {
	perror("ERROR: A child process did not exit correclty");
      }
    }

    free(child_pids);
  }
  return 0;
}
