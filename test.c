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
  printf("\nusername = %s and password=%s",username,password);
 }