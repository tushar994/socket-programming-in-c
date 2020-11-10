#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h> 
#define PORT 8000
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
long long int RAM = 10000;


int send_file(long long int size, int file_desc, int sock){
    // printf("size is now:%lld\n",size);
    // printf("started reading\n");
    char arr[RAM+10];
    printf("size of file is: %lld\n",size);
    int read_ret;
    int cycles = 0;
    while ( size>0 ) 
    {   
        // printf("loop %lld\n",size);
        cycles++;
        // printf("size is now:%lld\n",size);
        // we see how much we need to read this time
        // it starts at the staring of the place we need to read from
        long long int current_read;
        if(size>RAM){
            current_read = RAM;
        }
        else{
            current_read = size;
        }
        // we read the appropriate thing and store it in arr
        // printf("first read not done\n");
        read_ret = read(file_desc, arr, current_read);
        arr[read_ret] = '\0';
        // printf("first read done\n");
        read_ret = send(sock, arr, strlen(arr), 0); ////// SENDSOCBOY
        // printf("SENT FILE\n");
        // printf("read not done\n");    
        read_ret = read(sock,arr,1000);////// READSOCBOY
        arr[read_ret]= '\0';
        // printf("read done : %s\n",arr);
        // send(sock, "DONE", strlen("DONE"), 0);
        // we flip te contents of arr, and store this into arr2
        // we write contents of arr2 into the file
        // write (f_read, arr, current_read); 
        

        // printf("we got %s\n",arr);
        // we decrease size by the apprpriate amount
        size-=current_read;

        // we find the new location to read from
        
        // we see how much is done, and show in the loading thing
        // double percentage = ((double)((count-size)*100))/((double)(count));
        // print_loader(percentage);
        
    } 
    // printf("cycles : %d",cycles);
    // printf("last step\n");
    read_ret = send(sock, "END", strlen("END"), 0); ////// SENDSOCBOY
    // printf("read end func arr: END\n");
    // read_ret = read(sock,arr,1000);
    // arr[read_ret] = '\0';
    // printf("last one is %s\n",arr);
    // printf("exitted\n");
    return 0;
}


int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;  
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // This is to lose the pesky "Address already in use" error message
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR ,
                                                  &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc. 
    address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
    address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Port bind is done. You want to wait for incoming connections and handle them in some way.
    // The process is two step: first you listen(), then you accept()

    // returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
    printf("A\n");
    fd_set readfds;
    //  this is the loop that executes the server as a whole

    while(1){
        // printf("started looking for new");
        FD_ZERO(&readfds);
        if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // this loop servics one whole client
        // printf("Working with a new client now\n");
        int run_flag = 1;
        while(run_flag){
            // printf("looping\n");
            FD_ZERO(&readfds);
            FD_SET( new_socket , &readfds);
            // printf("prims\n");
            // read( new_socket , buffer, 1024);
            // printf("read %s\n",buffer);
            // printf("wait\n");
            int activity = select( new_socket+1 , &readfds , NULL , NULL , NULL);   
            // printf("out\n");
            if ((activity < 0) && (errno!=EINTR))   
            {   
                printf("select error");   
            }   
            if (FD_ISSET( new_socket , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                // printf("he did something\n");
                valread = read( new_socket , buffer, 1024); ////// READSOCBOY
                if (valread  == 0)   
                {   
                    //Somebody disconnected , get his details and print  
                    // printf("he exitted\n");
                            
                    //Close the socket and mark as 0 in list for reuse  
                    close( new_socket );
                    // printf("closed\n");
                    run_flag=0;  
                }   
                        
                //if the client has asked for a file
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    buffer[valread] = '\0';
                    printf("opening file : %s\n",buffer);
                    // printf("attempting reading from %s\n",buffer);
                    int f_write = open(buffer, O_RDONLY); 
                    struct stat file1Stat;

                    int okay1 = stat(buffer, &file1Stat);
                    if(okay1==0 && S_ISDIR(file1Stat.st_mode) && f_write>=1){
                        close(f_write);
                        f_write = 0;
                    }


                    if(f_write<1){
                        send(new_socket , "NO" , strlen("NO") , 0 );////// SENDSOCBOY
                        // printf("file didnt exist\n");
                        // perror("r2");
                    }
                    else{
                        long long int size = lseek(f_write, 0, SEEK_END);
                        sprintf(buffer,"%lld",size);
                        lseek (f_write, 0 , SEEK_SET);
                        printf("size string: %s\n",buffer);
                        // printf("length of buffer %d\n",strlen(buffer));
                        send(new_socket , buffer , strlen(buffer) , 0 ); ////// SENDSOCBOY 
                        int ret_buf = read(new_socket,buffer,1000);////// READSOCBOY
                        buffer[ret_buf] = '\0';
                        if(strcmp(buffer,"NO")==0){
                            printf("download couldnt be done because of issues on client side\n");
                            send(new_socket , "OKAY" , strlen("OKAY") , 0 );
                        }
                        else{
                            // commence download!!!
                            send_file(size,f_write, new_socket);
                        }
                        close(f_write);
                        // printf("done\n");
                    }                     
                }   
            }
        }
        // printf("looking for new\n");
    }
    return 0;
}
