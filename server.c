// Name: Shakya, Abin
// Project: PA-2(Programming)
// Instructor: Feng Chen
// Class: cs7103-au21
// LogonID: cs710309


//importing all the required libraries and packages to use
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define IP_PROTOCOL 0 
#define SIZE_OF_NET_BUF  32 
#define cipherKey 'S' 
#define sendrecvflag 0 
#define nofile "File Not Found!" 
#define SIZE 1024

#define PORT 2222

#define basepath "/classes/cs7103/cs710309/abin/users/"



/*
*  This function concatenates two strings
*/
char* concat(const char *s1, const char *s2)
{
   char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 is for the null-terminator
   strcpy(result, s1);
   strcat(result, s2);
   return result;
}

/*
*  This function servers to receive file from the client.
*/
void receive_file(char *foldername,int sockfd){
   int n;
   FILE *fp;
   char *filename = foldername;
   char buffer[SIZE];
   printf("filename=\n");
   printf("foldername=%s\n",foldername);
   fp = fopen(filename, "w");

   if (fp == NULL) {
   
      perror("[-]Error in uploading file.");
      strcpy(buffer,"Error in uploading file. Please double check for user existence");
   
      send(sockfd, buffer, strlen(buffer), 0);
      bzero(buffer, strlen(buffer));
   }
   else{
      printf("\nFile Successfully opened!\n");
      while (1) {
         n = recv(sockfd, buffer, SIZE, 0);
         if (n <= 0){
            printf("End of file");
            break;
            return;
         }
         fprintf(fp, "%s", "buffer");
         bzero(buffer, SIZE);
   }
   }
   return;
}

/*
*  This function servers to  send file to the client.
*/
void sendFile(FILE *fp, int sockfd){
   int n;
   char data[SIZE] = {0};
   char msg[] = "Start downloading";
   send(sockfd, msg, sizeof(msg),0);

   while(fgets(data, SIZE, fp) != NULL) {
      if (send(sockfd, data, sizeof(data), 0) == -1) {
         perror("[-]Error in sending file.");
         exit(1);
      }
      bzero(data, SIZE);
   }
}

/*
*  list all created users
*/
int listUsers(char * buf,int socket) {

   DIR *dirp = opendir(basepath);
   struct dirent *direntp;

   if (dirp != NULL) {
      while ((direntp = readdir(dirp)) != NULL){
      
      //this line skip current and parent directory
         if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
          //shows directory witin it
            if (direntp->d_type == 4 ){
               strcat(buf,"\n");
               strcat(buf,direntp->d_name);
            }  
         }
      }
      send(socket, buf, strlen(buf),0);
      bzero(buf, strlen(buf));
    
   }
   else {
      printf("Could not open directory");
   }
   closedir(dirp);
   return 1;
}


/*
*  Function to list all users's files
*/



int listUserFiles(char * buf, int socket) {
      
   DIR *dirp = opendir(concat(basepath,buf));
   struct dirent *direntp;

   if (dirp != NULL) {
    
      while ((direntp = readdir(dirp)) != NULL){
      
       //skip current and parent directory
         if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
         //for only files
            strcat(buf,"\n");
            strcat(buf,direntp->d_name);
         }
      
      }
      send(socket,buf,strlen(buf),0);
      bzero(buf, strlen(buf));
   }
   else {
      printf("Could find the user");
      strcat(buf," ");
      send(socket,buf,strlen(buf),0);
      bzero(buf, strlen(buf));
   
   }
   closedir(dirp);
   return 1;
}


/*
*  Fnction to list all files of a user 
*/
int listFolderFiles(char * buf, int socket) {
   DIR *dirp = opendir(concat(basepath,buf));
   struct dirent *direntp;

   if (dirp != NULL) {
      printf("No users found");
      while ((direntp = readdir(dirp)) != NULL){
      
       //skip current and parent directory
         if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
         //only files allowed
            strcat(buf,"\n");
            strcat(buf,direntp->d_name);
         }
      
      }
      send(socket,buf,strlen(buf),0);
      bzero(buf, strlen(buf));
   }
   else {
      printf("Could find the user");
   }
   closedir(dirp);
   return 1;
}

/*
*  Delete file by user
*/

int deleteUserFile(char *folderName,char *buf,int socket){

   DIR *dirp = opendir(concat(basepath,folderName));
   DIR *user_dirp;
   struct dirent *direntp;
   int fileRemoved=0;
   char * path= concat(basepath,folderName);
   path = concat(path,"/");
   if (dirp != NULL) {
   
      while ((direntp = readdir(dirp)) != NULL){
         //skip current and parent directory
         if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
            // //only files
            if (strcmp(buf,direntp->d_name)==0){
                //lcoation of file
               path = concat(path,buf);
               printf("%s",path);
               remove(path);
               fileRemoved=1;
               break;
              
            }        
         }
      }
      if (fileRemoved==0){
         strcpy(buf,"Failed to delete a file");
         send(socket, buf, strlen(buf),0);
         bzero(buf, strlen(buf));
      }
      else{
       //this part to prevent empty folder
         strcpy(buf,"File deleted successfully");
         send(socket, buf, strlen(buf),0);
         bzero(buf, strlen(buf));
      }
    
   }
   else {
      strcat(buf," ");
      send(socket, buf, strlen(buf),0);
      bzero(buf, strlen(buf));
   
      printf("Could not open dir");
   }
   return 1;
}


/*
*  Delete file
*/


int deleteFile(char *buf,int socket){

   printf("basepath=%s\n",basepath);
   DIR *dirp = opendir(basepath);
   DIR *user_dirp;
   struct dirent *direntp;
   int fileRemoved=0;
   char * path;
   printf("heyyyyy = %s\n",dirp);
   if (dirp != NULL) {
      printf("kkk\n");
      while ((direntp = readdir(dirp)) != NULL){
         
         printf("direntp->dname=%s",direntp->d_name);
       //skip current and parent directory
         if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
         
            //only show directory i.e 4 mean folder
            if (direntp->d_type == 4){
              
               path = concat(basepath,direntp->d_name);
               path = concat(path,"/");
               printf("%s",path);
               user_dirp = opendir(concat(basepath,direntp->d_name));
               if (user_dirp != NULL) {
                  while ((direntp = readdir(user_dirp)) != NULL){
                      if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
                     
                     // //only files
                        if (strcmp(buf,direntp->d_name)==0){
                        //lcoation of file
                           path = concat(path,buf);
                           printf("%s",path);
                           remove(path);
                           fileRemoved=1;
                           break;
                        
                        }
                     }
                  }
               }
               else{
                  printf("Could not open user dir");
               }
            }
         }
         
      }
   
   
      if (fileRemoved==0){
         strcpy(buf,"Failed to delete a file");
         send(socket, buf, strlen(buf),0);
         bzero(buf, strlen(buf));
      }
      else{
       //to prevent empty folder
         strcpy(buf,"File deleted");
         send(socket, buf, strlen(buf),0);
         bzero(buf, strlen(buf));
      }
    
   }
   else {
      strcat(buf," ");
      send(socket, buf, strlen(buf),0);
      bzero(buf, strlen(buf));
      printf("could not open dir");
   }
   return 1;
}

/*
*  This function is to display the file owner
*/


int tansferFileOwner(char *fromUser,char *toUser,char *filename,int socket){

   printf("%s",fromUser);
   printf("%s",toUser);

   char buf[1024]; 
   if(rename(fromUser,toUser)!=0){
      perror("rename() error");
   
      strcpy(buf,"Failed to transfer ownership");
      send(socket, buf, strlen(buf),0);
      bzero(buf, strlen(buf));
      return 1;
   }

   strcpy(buf,"File owner transfered");
   send(socket, buf, strlen(buf),0);
   bzero(buf, strlen(buf));

   return 1;

}



/*
*  This function is to find the file owner
*/


int fileOwner(char *buf,int socket){

   DIR *dirp = opendir(basepath);
   DIR *user_dirp;
   struct dirent *direntp;
   char *owner;
   int ownerfound=0;

   if (dirp != NULL) {
   
      while ((direntp = readdir(dirp)) != NULL){
      
           if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
           //only show directory i.e 4 mean folder
            if (direntp->d_type == 4){
              
               owner = direntp->d_name;
              
               user_dirp = opendir(concat(basepath,direntp->d_name));
               if (user_dirp != NULL) {
                  while ((direntp = readdir(user_dirp)) != NULL){
                  
                    //skip current and parent directory
                     if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
                     
                     // //only files
                        if (strcmp(buf,direntp->d_name)==0){
                           strcpy(buf,"Owned by :");
                           strcat(buf,owner);
                           ownerfound = 1;
                           break;
                        
                        }
                     }
                  }
               }
               else{
                  printf("Could not open user dir");
               }
            }
         }
         
      }
      
      if (ownerfound==0){
         strcpy(buf,"File Owner not found");
         send(socket, buf, strlen(buf),0);
         bzero(buf, strlen(buf));
      }
      else{
       //to prevent empty folder
         strcat(buf," ");
         send(socket, buf, strlen(buf),0);
         bzero(buf, strlen(buf));
      }
    
   }
   else {
      strcat(buf," ");
      send(socket, buf, strlen(buf),0);
      bzero(buf, strlen(buf));
   
      printf("Could not open dir");
   }
   return 1;
}


/*
* This function is to list all files of all users
*/


int listAllFiles(char *buf,int socket,int showOwner){

   bzero(buf, strlen(buf));
   DIR *dirp = opendir(basepath);
   DIR *user_dirp;
   struct dirent *direntp;

   if (dirp != NULL) {
   
      while ((direntp = readdir(dirp)) != NULL){
      
       //skip current and parent directory
         if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
         
            //only show directory i.e 4 mean folder
            if (direntp->d_type == 4){
              
              //show owner name before files or not
               if (showOwner==1){
                  strcat(buf,"\n\nUsername: ");
                  strcat(buf,direntp->d_name);
                  strcat(buf,"\n");
               }
            
               user_dirp = opendir(concat(basepath,direntp->d_name));
               if (user_dirp != NULL) {
                  while ((direntp = readdir(user_dirp)) != NULL){
                  
                    //skip current and parent directory
                     if (strcmp(direntp->d_name,".")!=0 && strcmp(direntp->d_name,"..")!=0){
                     
                     //only files
                        strcat(buf,"\n");
                        strcat(buf,direntp->d_name);
                     }
                  }
               }
               else{
                  printf("Could not open user dir");
               }
            }
         }
      }
   
   //to prevent empty folder
      strcat(buf," ");
      printf("folder count %lu %s",strlen(buf),buf);
      send(socket, buf, strlen(buf),0);
      bzero(buf, strlen(buf));
    
   }
   else {
      strcat(buf," ");
      send(socket, buf, strlen(buf),0);
      bzero(buf, strlen(buf));
   
      printf("could not open dir");
   }
   closedir(dirp);
   closedir(user_dirp);
   return 1;
}


/*
*This function is to parse the client command and execute the corresponding function then send via socket
*/


int parseCmd(char *buf,int newSocket){


   FILE* fp; 
   int i = 0;
   char *cmd[5];

   char *p = strtok (buf, " ");
    
   printf("\nparsing command\n");

//To split command by space delimiter
   while (p != NULL)
   {
      cmd[i++] = p;
      p = strtok (NULL, " ");
   }
   printf("abin");
   printf("\nparsed Command %s %s %s\n",cmd[0],cmd[1],cmd[2]);

  // execute commands on the basis of splitted value
   if (strcmp(cmd[0],"sudo")==0){
      printf("superuser");
      if (strcmp(cmd[1],"-create")==0){
         printf("\ncreate a user\n");
         if(cmd[2]!=NULL){
            //bitwise OR to check the permission, Read, Write and Executable for Group, Users and others
            if (mkdir(concat(basepath,cmd[2]), S_IRWXG | S_IRWXU | S_IRWXO) != 0){
               //raise permission error
               perror("Error performing mkdir()");
               strcpy(buf,"Failed to create a user \n");
               send(newSocket, buf, strlen(buf), 0);
               bzero(buf, strlen(buf));            
            }
            else{
               printf ("%s created successfully \n",cmd[2]);
               strcpy(buf,"User created successfully \n");
               send(newSocket, buf, strlen(buf), 0);
               bzero(buf, strlen(buf));
            }
         }
         else{
            strcpy(buf,"incorrect command try hero -create username\n");
            send(newSocket, buf, strlen(buf), 0);
            bzero(buf, strlen(buf));
         }
      }
      else if(strcmp(cmd[1],"-delete")==0){
      
         printf("\ndelete a user\n");
         if(cmd[2]!=NULL){
            if (rmdir(concat(basepath,cmd[2])) != 0){
            
               perror("Error performing rmdir()");
               strcpy(buf,"Failed to delete a user\n");
               send(newSocket, buf, strlen(buf), 0);
               bzero(buf, strlen(buf));
            
            }
            else{
               printf ("%s deleted successfully \n",cmd[2]);
               strcpy(buf,"User deleted successfully\n");
               send(newSocket, buf, strlen(buf), 0);
               bzero(buf, strlen(buf));
            
            }
         
         }
         else{
            strcpy(buf,"incorrect command try hero -delete username\n");
            send(newSocket, buf, strlen(buf), 0);
            bzero(buf, strlen(buf));
         }
      }
      else if(strcmp(cmd[1],"del") == 0 && cmd[2]!=NULL){
      
         printf("\ndeleting a file \n");
      
         deleteFile(cmd[2],newSocket);
      
      }
      else if(strcmp(cmd[1],"fileown") == 0 && cmd[2]!=NULL){
      
         printf("\nfinding a file owner\n");
      
         fileOwner(cmd[2],newSocket);
      
      }
      else if (strcmp(cmd[1],"luf")==0){
      
         if (strcmp(cmd[2],"all")==0){
         
         // to list all files
            printf("\nlisting all files \n");
            listAllFiles(buf,newSocket,0);
         }
         else if (strcmp(cmd[2],"all")!=0){
         
          //to specify user owned files    
            printf("\nlisting %s files \n",cmd[1]);
            listUserFiles(cmd[2],newSocket);
            printf("\nlisting %s files \n",cmd[1]);
         
         }
       }
      else if(strcmp(cmd[1],"chgown")== 0){
         //sudo chgown fromOwner toOwner filename.txt
      
         char *fromUserPath = concat(basepath,cmd[2]);
         fromUserPath = concat(fromUserPath,"/");
         fromUserPath = concat(fromUserPath,cmd[4]);
      
      
         char *toUserPath = concat(basepath,cmd[3]);
         toUserPath = concat(toUserPath,"/");
         toUserPath = concat(toUserPath,cmd[4]);
      
         tansferFileOwner(fromUserPath,toUserPath,cmd[4],newSocket);
         
      }
      else if(strcmp(cmd[1],"listuser")== 0){
        //list all users
         printf("\nlisting all users \n");
         listUsers(buf,newSocket);  
                
      }
      else{
         printf("Invalid\n");
         strcpy(buf,"Not a valid command" );
         send(newSocket,buf,strlen(buf),0);
         bzero(buf, strlen(buf));
      
      }
   }
   else{
         printf("Normal  uSer commos");
        //Normal user commands
        if (strcmp(cmd[1],"listfile")==0){
         //specific user owned files    
         printf("\nlisting %s files \n",cmd[1]);
         listUserFiles(cmd[0],newSocket);
         printf("\nlisting %s files \n",cmd[1]);
               
      }
      else if (strcmp(cmd[1],"upfile")==0 && cmd[2]!=NULL){
         printf("\n uploading a file \n");
      
         char *filename = concat(basepath,cmd[0]);
         printf("%s",cmd[0]);
      
         char msg[] = "Start uploading";
         send(newSocket, msg, sizeof(msg),0);
         bzero(buf, strlen(buf));
      
         filename = concat(filename,"/");
         filename = concat(filename,cmd[2]);
         printf("%s",filename);
      
         receive_file(filename,newSocket);
      
      }
      else if(strcmp(cmd[1],"chgown")== 0){
      
         char *fromUserPath = concat(basepath,cmd[0]);
         fromUserPath = concat(fromUserPath,"/");
         fromUserPath = concat(fromUserPath,cmd[3]);
            
         char *toUserPath = concat(basepath,cmd[2]);
         toUserPath = concat(toUserPath,"/");
         toUserPath = concat(toUserPath,cmd[3]);
      
         tansferFileOwner(fromUserPath,toUserPath,cmd[3],newSocket);
      
       }
      else if (strcmp(cmd[1],"downfile")==0 && cmd[2]!=NULL){
         printf("\ndownload file \n");
         printf(" \nfilename %s \n",cmd[2]);
      
         char *filename = concat(basepath,cmd[0]);
         filename = concat(filename,"/");
         filename = concat(filename,cmd[2]);
      
         fp = fopen(filename, "r");
         if (fp == NULL) {
         
            perror("[-]Error in reading file.");
            strcpy(buf,"Error in reading file");
         
            send(newSocket, buf, strlen(buf), 0);
            bzero(buf, strlen(buf));
         
         }
         else{
            printf("\nFile Successfully opened!\n"); 
         
            sendFile(fp, newSocket);
         
            printf("\nFile data sent succefully!\n"); 
         }
             
      }
      else if(strcmp(cmd[1],"del") == 0 && cmd[2]!=NULL){
      
         printf("\ndeleting a file \n");
      
         deleteUserFile(cmd[0],cmd[2],newSocket);
      
      }
      else{
      
         strcpy(buf,"Invalid command. Please check again.");
         send(newSocket,buf,strlen(buf),0);
         bzero(buf, strlen(buf));
      }
   }
 
   return 1;

}


// This is main function that primarily runs the programs and calls other functions as needed. 
int main(){
   //creating socket for listening
   int serverSocket, ret;
   struct sockaddr_in serverAddr;

   int newSocket;
   struct sockaddr_in newAddr;

   socklen_t addr_size;

   char buffer[1024];
   char command[1024];
   pid_t childpid;
   char net_buf[SIZE_OF_NET_BUF]; 
 
   DIR* dirp;
   int Downloading = 0;
  
  //Creating Server Socket
   serverSocket = socket(AF_INET, SOCK_STREAM, 0);
   if(serverSocket < 0){
      printf("[-]Error in connection.\n");
      exit(1);
   }
   printf("[+]Server Socket is created.\n");

   memset(&serverAddr, '\0', sizeof(serverAddr));
  
  //Address to the socket is defined here
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(PORT);
   serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   //Binding the socket
   ret = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
   if(ret < 0){
   
      perror("[-]Error binding.\n");
      exit(1);
   }
   printf("[+]Bind to port %d\n", PORT);
  
  //Listening upto to 10 clients
   if(listen(serverSocket, 10) == 0){
      printf("[+]Listening......\n");
   }
   else{
      printf("[-]Error Listening.\n");
      exit(1);
   }
   int n;
   while(1){
      addr_size= sizeof(newAddr);
      newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);
      if(newSocket < 0){
         exit(1);
      }
      printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
   
      if((childpid = fork()) == 0){
         close(serverSocket);
      
         while(1){
         
            bzero(buffer, strlen(buffer));
            if(recv(newSocket, buffer, 1024, 0)<0){
               printf("receiveing breaked");
            // break;
            }
         
            printf("\nFrom client: %s <-- \n",buffer);
          // pareseCommand(buffer);
            parseCmd(buffer,newSocket);
              
            if(strncmp(buffer, "exit", 4) == 0){
               printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
               break;
            }
         
         }
      }
   
   }
   close(newSocket);
   return 0;
}


//End of Server C code. 
