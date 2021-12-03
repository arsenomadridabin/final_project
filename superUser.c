// Name: Shakya, Abin
// Project: PA-2(Programming)
// Instructor: Feng Chen
// Class: cs7103-au21
// LogonID: cs710309


//importing all the necessary libraries for SuperUser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define PORT 8000
#define SIZE 1024

#define user_credential "admin_credentials.txt"

// Download File function for Super User
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

//Download File function for SuperUser
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
          buffer[strlen(buffer)] = '\0'; 
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

char* get_username(void){
  static char username[100];
  int k=0;
  char input_1='\0';

  printf("Username:");
  while (input_1 !='\n'){
    input_1 = getchar();

    if(input_1 !='\n'){
      username[k] = input_1;
    }
    k++;
  }

  return username;
}

char* get_password(void){
  static char pw[100];
  int j=0;
  char input_1='\0';

  printf("Password:");
  while (input_1 !='\n'){
    input_1 = getchar();

    if(input_1 !='\n'){
      pw[j] = input_1;
    }
    j++;
  }
  return pw;
}


//Main function to establish the network connection of SuperUser with the server
int main(){
  int clientSocket, ret;
  struct sockaddr_in serverAddr;
  char buffer[1024];
  int verify;

  char *username = get_username();
  char *password = get_password();
  
  verify = verify_username_and_password(username,password);
  if (verify == 1){
    printf("[+]Logged in Successfully\n");
  }
  else{
    printf("Incorrect Username/Password\n");
    return 1;
  }

  //Creating Socket
  clientSocket = socket(AF_INET, SOCK_STREAM,0);
  if(clientSocket <0){
    printf("[-]Error in Connection.\n");
    exit(1);
  }
  printf("[+]Client Socket is sucessfully.\n");

  memset(&serverAddr, '\0', sizeof(serverAddr));


  //Specify an address for the socket
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  //Connecting to server
  ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  if(ret<0){
    printf("[-]Error in connecting to Server.\n");
    exit(1);
  }
  printf("[+]Successfully connected to Server.\n");
  int n;
  char input;
  while(1){

//reset values and buffers
    bzero(buffer, sizeof(buffer)); 
    input='\0';
    n =0;

    printf("user@admin: \t");
 
    
    while (input !='\n'){
      input = getchar();
      if(input !='\n'){
        buffer[n] = input;
      }
      n++;
    }

     if (strlen(buffer)==0){
      continue;
    }
   
   
    send(clientSocket, buffer, strlen(buffer),0);

    if(strncmp(buffer, "exit",4)==0){
          close(clientSocket);
          printf("[-] Super User Exit !! Disconnected from Server.\n");
          exit(1);
    }
    
      while(1){
          bzero(buffer, sizeof(buffer)); 
          if(recv(clientSocket, buffer, 1024,0)<0){
            printf("[-]Error on Receiving the Data from Server");
          }
          else if (strncmp(buffer, "exit",4) == 0) {
            printf("Server exited");

            exit(1);
          }else{
            printf("\nFrom Server:----- \t %s\n", buffer);
          
            break;
          }
      }
        printf("\n********************************\n"); 
  
  }
  return 0;
}
