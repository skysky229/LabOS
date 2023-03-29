#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static int pipefd1[2],pipefd2[2];

void INIT(void){
     if (pipe(pipefd1)<0 || pipe(pipefd2)<0){
          perror("pipe");
          exit(EXIT_FAILURE);
     }
}

void WRITE_TO_PARENT(void){
     //∗ send parent a message through pipe∗/
     char msg[] = "Hello parents!";
     write(pipefd1[1], msg, sizeof(msg));
     printf ("Child send message to parent!\n") ;
}

void READ_FROM_PARENT(void){
     //∗ read message sent by parent from pipe∗/
     char msg[1000];
     read(pipefd2[0], msg, sizeof(msg));
     printf("Child receive message from parent! And that msg is %s\n",msg);
}

void WRITE_TO_CHILD(void){
     //∗ send child a message through pipe∗/
     char msg[] = "Hello child!";
     write(pipefd2[1], msg, sizeof(msg));
     printf ("Parent send message to child!\n");
}

void READ_FROM_CHILD(void){
     //∗ read the message sent by child from pipe ∗/
     // TO DO
     char msg[1000];
     read(pipefd1[0], msg, sizeof(msg));
     printf("Parent receive message from child! And that msg is %s\n",msg);
}

int main(int argc, char* argv[]) {
     INIT();
     pid_t pid;
     pid = fork();
     // set a timer, process will end after 1 second .
     alarm(10);

     if ( pid==0){ // parent process
          close(pipefd1[1]); // close write end of pipefd1
          close(pipefd2[0]); // close read end of pipefd2
          while(1){
               sleep(rand()%2+1);
               WRITE_TO_CHILD(); //use pipefd2[1]
               READ_FROM_CHILD(); // use pipefd1[0]
          }
     }else{ // child process
          close(pipefd1[0]); // close read end of pipefd1
          close(pipefd2[1]); // close write end of pipefd2
          while(1){
               sleep(rand()%2+1);
               READ_FROM_PARENT(); // use pipefd2[0]
               WRITE_TO_PARENT(); // use pipfd1[1]
          }
     }
     return 0;
}