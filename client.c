// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8000
#include <unistd.h> 
#include <sys/types.h> 
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
long long int RAM = 10000;


void print_loader(double percentage){
    
    char value[5];

    value[4] = (char)(((int)(percentage*((double)100)))%10)+ '0';
    value[3] = (char)(((int)(percentage*((double)10)))%10) + '0';
    value[2] = (char)(((int)(percentage))%10) + '0';
    value[1]= (char)(((int)(percentage/((double)10)))%10) + '0';
    value[0]= (char)(((int)(percentage/((double)99)))%10) + '0';

    
    char okay[10];

    for(int i=1;i<=3;i++){
        okay[i] = value[i-1];
    }
    okay[4] = '.';
    okay[5] = value[3];
    okay[6] = value[4];

    okay[7] = '%';
    okay[0] = '\r';
    okay[8] = '[';
    okay[9] = '\0';
    // if(percentage/100 != 0){
    //     okay[1] = (char)(percentage/100) + '0';
    //     okay[2] = (char)((percentage%100)/10) + '0';
    //     okay[3] = (char)((percentage%10)) + '0';
    // }
    // else if((percentage%100)/10 != 0){
    //     okay[2] = (char)((percentage%100)/10)+ '0';
    //     okay[3] = (char)((percentage%10))+ '0';
    // }
    // else if((percentage%10) != 0){
    //     okay[3] = (char)((percentage%10))+ '0';
    // }
    // write(STDOUT_FILENO, okay, strlen(okay));
    printf("%s",okay);

    for(int i=1;i<=percentage/5;i++){
        // write(STDOUT_FILENO, "#", strlen("#"));
        printf("#");
    }
    for(int i=(percentage/5)+1;i<=20;i++){
        // write(STDOUT_FILENO, " ", strlen(" "));
        printf(" ");
    } 
    // write(STDOUT_FILENO, "]", strlen("]"));
    printf("]");
}

int func(char file_name[],long long int size, int sock){ 
    // arr is the name of the file being downloaded    

    // send(sock, input[i], strlen(input[i]), 0);
    long long int count = size;
    char arr[RAM+10];
    int file_name_len = strlen(file_name);
    char cwd[1024];
    // gets the current directory
    char* bruh = getcwd(cwd, sizeof(cwd));
    int path_len = strlen(cwd);

    char name[1024];

    for(int i=0;i<path_len;i++){
        name[i] = cwd[i];
    }
    name[path_len]= '/';

    for(int i=0;i<file_name_len;i++){
        name[path_len+1+i] = file_name[i];
    }
    name[path_len+1+file_name_len] = '\0';
    // we now have the name of the file, along with it's path !!!


    
    // Open the file for WRITE and READ only. 
    int f_read = open(name, O_RDWR | O_CREAT  | O_TRUNC, 0600); 

    if(f_read<0){
        perror("r67");
        send(sock, "NO", strlen("NO"), 0);////// SENDSOCBOY
        int read_ret = read(sock, arr, 1000);
        return 1;
    }

    send(sock, "REC", strlen("REC"), 0);////// SENDSOCBOY
    // printf("size of file is: %lld\n",size);
    // int cycles = 0;
    // printf("entered and dooing: %lld\n",size);
    while ( size>0 ) 
    {      
        // printf("loop %lld\n",size);

        // printf("reading\n");
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
        
        int read_ret = read(sock, arr, current_read+5);////// READSOCBOY
        arr[read_ret] = '\0';
        // printf("READ FILE %d\n",read_ret);
        send(sock, "REC", strlen("REC"), 0);////// SENDSOCBOY
        write (f_read, arr, current_read); 
        // we flip te contents of arr, and store this into arr2
        // we write contents of arr2 into the file
        


        // we decrease size by the apprpriate amount
        size-=current_read;

        // we find the new location to read from
        
        // we see how much is done, and show in the loading thing
        double percentage = ((double)((count-size)*100))/((double)(count));
        print_loader(percentage);
        // printf("end\n");
    } 
    printf("\n");
    int okay = read(sock, arr, 1000);////// READSOCBOY
    arr[okay]='\0';
    // printf("read end func arr: %s\n",arr);
    close(f_read);
    return 0;

} 



int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
                                                // which is meant to be, and rest is defined below

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converts an IP address in numbers-and-dots notation into either a 
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    char* commmand_line_line;
    commmand_line_line = (char *)malloc(1000*sizeof(char));
    size_t linecap = 0;
    char* input[100];

    while(1){
        printf("client> ");
        // take input
        int linelen = getline(&commmand_line_line, &linecap, stdin);
        int length_input = strlen(commmand_line_line);
        commmand_line_line[length_input-1] = '\0';
        // tokenise input with spaces
        input[0] = strtok(commmand_line_line," ");
        int input_index = 1;
        while(input[input_index-1]!=NULL){
            input[input_index] = strtok(NULL," ");
            input_index++;
        }
        // input_index is the number of words in the given input
        input_index--;

        if(strcmp(input[0],"exit")==0){
            return 0;
        }
        else if(strcmp(input[0],"get")==0){
            // we loop through all the files asked for
            for(int i=1;i<input_index;i++){
                // send them the name of the file
                printf("asking server for file : %s\n",input[i]);
                send(sock, input[i], strlen(input[i]), 0);////// SENDSOCBOY
                // printf("sent him a file name\n");
                valread = read( sock , buffer, 1000);////// READSOCBOY
                // printf("valread: %d\n",valread);
                buffer[valread]= '\0';
                // printf("got a respoonse from him\n");
                // if they say that they dont have the file then we print that the requested file does not exist
                if(strcmp(buffer,"NO")==0){
                    printf("The file asked for does not exist\n");
                }
                else{
                    buffer[valread] = '\0';
                    // printf("size string:%s\n",buffer);
                    int len = strlen(buffer);
                    long long int size = 0;
                    for(int i=0;i<len;i++){
                        size*=10;
                        size+= (int)buffer[i] - (int)'0';
                    }
                    printf("retrieving file...\n");
                    

                    func(input[i],size,sock);
                    // send(sock, "REC", strlen("REC"), 0);
                }
            }
        }
        else{
            printf("invalid command\n");
        }
    }
    return 0;
}
