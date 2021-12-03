// Name: Shakya, Abin
// Project: PA-2(Programming)
// Instructor: Feng Chen
// Class: cs7103-au21
// LogonID: cs710309

//importing all the necessary libraries for generalUser
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000
#define SIZE 1024

#define basepath "/classes/cs7103/cs710309/abin/users/"


// #define basepath "/Users/"

#define user_credential "user_credentials.txt"
/*
*
*  This function concatenate two strings
*/
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// Download File funciton for NormalUser
void download_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
     printf("%s", buffer); 
     

    if (n <= 0){
      printf("end of file");
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

//upload file function for Normal User
void upload_file(FILE *fp, int sockfd){

  int n;
  char data[SIZE] = {0};
  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

//Open file function for Normal User
void send_file(int sockfd, char *file_name){
  FILE *fp;
  // printf("filename=%s\n",file_name);
   fp = fopen(file_name, "r");
    if (fp == NULL) {

      perror("[-]Error in reading file.");

    }else{
      printf("\nFile Successfully opened!\n"); 
    }

    upload_file(fp, sockfd);

    printf("\nFile data sent succefully!\n"); 
}


int verify_username_and_password(char *username, char*password){
  FILE *fp;
  int size = 1024, pos;
  int c;
  char *buffer = (char *)malloc(size);
  char *password_2;
  int username_exist = 0;
  int password_match = 0;


  fp = fopen(user_credential,"r");
  if (fp == NULL){
    printf("[-] Error opening authentication file\n");
    return 0;
  }
  else{
    do { // read all lines in file
        pos = 0;
        do{ // read one line
          c = fgetc(fp);
          if(c != EOF) buffer[pos++] = (char)c;
          if(pos >= size - 1) { // increase buffer length - leave room for 0
            size *=2;
            buffer = (char*)realloc(buffer, size);
          }
        }while(c != EOF && c != '\n');
        buffer[pos] = 0;
        if (username_exist == 1){
          buffer[strlen(buffer)-1] = '\0'; 
          password_2 = buffer;
          if (strcmp(password,password_2)==0){
            password_match = 1;
            return 1;
          }
          else{
            return 0;
          }
        }
        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer,username)==0){
          username_exist = 1;
        }
      } while(c != EOF); 
      fclose(fp);
    return 0;
  }
}

//Main function to run the GeneralUser C file, and also to establish the network connection with the server
int main(){
  int clientSocket, ret;
  struct sockaddr_in serverAddr;
  char buffer[1024];

  char username[100];
  char password[100];
  int k,l;
  char input_1='\0';
  char input_2 = '\0';

  int verify;

  printf("Username:");
  while (input_1 !='\n'){
    input_1 = getchar();

    if(input_1 !='\n'){
      username[k] = input_1;
    }
    k++;
  }
  printf("Password:");
  while (input_2 !='\n'){
    input_2 = getchar();

    if(input_2 !='\n'){
      password[l] = input_2;
    }
    l++;
  }
  printf("\n%s %s\n",username,password);
  }
  
