// Name: Shakya, Abin
// Project: PA-2(Programming)
// Instructor: Feng Chen
// Class: cs7103-au21
// LogonID: cs710309


//importing all the necessary libraries for AdminUser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2222
#define SIZE 1024

// Download File function for Adminuser
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

//Download File function for Adminuser
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

//Main function to establish the network connection of Adminuser with the server
int main(){
  int clientSocket, ret;
  struct sockaddr_in serverAddr;
  char buffer[1024];

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

    printf("AdminUser: \t");
 
    
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
          printf("[-] Admin User Exit !! Disconnected from Server.\n");
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
        printf("\n-------------------------------\n"); 
  
  }
  return 0;
}
//End of Adminuser c file. 
