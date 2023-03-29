#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdbool.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<string.h>

int shmid;
int n = 50177, m = 5; 
double avgrating[1700];

int main(){
     shmid = shmget(IPC_PRIVATE, sizeof(double[n+5][m+5]),0644|IPC_CREAT);
     if(shmid<0){
	     perror("Fail to create shm \n");
	     exit(1);
     } else {
          double (*matrix)[m]; 
          matrix = shmat(shmid,0,0);
          char line[25];
          double userID, movieID, rating, timeStamp;
          double test[n+5][m+5];

          for(int i=0;i<n;i++)
               for(int j=0;j<m;j++){
                    matrix[i][j] = 0;
                    test[i][j] = 0;
               }
          

          int pid1 = fork();

          if (pid1 == 0){
               // child process
               // read the data from the first dataset, then process and put it to shared memory
               FILE* fp = fopen("movie-100k_1.txt", "r");
               int i = 0;
               while(fgets(line, sizeof(line), fp) != NULL){
                    sscanf(line,"%lf %lf %lf %lf", &userID, &movieID, &rating, &timeStamp);
                    // matrix[userID+n][movieID] += 1; // increase the count of rating of a user for a movie
                    // matrix[userID][movieID] += rating; // increase the sum of rating 
                    matrix[i][0] = userID; matrix[i][1] = movieID; matrix[i][2] = rating; matrix[i][3] = timeStamp;
                    //printf("%lf %lf %lf %lf \n", test[i][0], test[i][1], test[i][2], test[i][3]);
                    i++;
               }
               shmdt(matrix);
          } else {
               // parent process
               wait(NULL); // wait until child 1 complete
               // int pid2 = fork();
               // if (pid2 == 0){
               //      // read the data from the second dataset, then process and put it to shared memory
               //      FILE* fp = fopen("movie-100k_2.txt", "r");
               //      while(fgets(line, sizeof(line), fp) != NULL){
               //           sscanf(line, "%d %d %d %d", &userID, &movieID, &rating, &timeStamp);
               //           matrix[userID+n][movieID] += 1; // increase the count of rating of a user for a movie
               //           matrix[userID][movieID] += rating; // increase the sum of rating 
               //      }               
               // } else {
               //      wait(NULL); // wait until the child 2 complete
               //      for(int j=1;j<=m;j++){
               //           double count = 0;
               //           double sum = 0;
               //           for(int i=1;i<=n;i++){
               //                sum += matrix[i][j]; // calculate the sum of rating of a movie 
               //                count += matrix[i+n][j]; // calculate the number of rating for that movie
               //           }
               //           avgrating[j] = sum/count; // calculate the average for that movie
               //      }

               //      for(int i=1;i<=m;i++)
               //           printf("The average rating of movie with ID %d is %.4f \n", i, avgrating[i]);
               // }
               double (*matrix)[m]; 
               matrix = shmat(shmid,0,0);
               FILE* fp = fopen("movie-100k_1.txt", "r");
               int i = 0;               
               int count = 0;
               while(fgets(line, sizeof(line), fp) != NULL){
                    sscanf(line,"%lf %lf %lf %lf", &userID, &movieID, &rating, &timeStamp);
                    // matrix[userID+n][movieID] += 1; // increase the count of rating of a user for a movie
                    // matrix[userID][movieID] += rating; // increase the sum of rating 
                    if (matrix[i][0] != userID || matrix[i][1] != movieID || matrix[i][2] != rating || matrix[i][3] != timeStamp)
                         count++;
                    i++;
               }
               for(int i=0;i<n;i++){
                    for(int j=0;j<4;j++){
                         if (test[i][j] - matrix[i][j] != 0){
                              printf("test: %lf  matrix: %lf, at %d %d \n", test[i][j], matrix[i][j], i, j);
                              count++;
                         }
                    }
                    //printf("%lf %lf %lf %lf \n", matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
               }
               printf("number of difference: %d \n", count);
               shmdt(matrix);
          }
     }
}