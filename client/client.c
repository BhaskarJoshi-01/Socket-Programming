// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8000
#define bs 5000
#define ll long long int
#define pm per /= 10;
#define pd per % 10
#define sz 10000

void print(ll tot, ll curr)
{
    //the value was asked in percent upto two decimal places
    // when i used 100 or 1000 as dividing size the code ran fast but
    // the error was that in 100 and 1000 decimal places were fixed to .99 or .x9
    // where x is number between 0-9.
    // this was because the round off of per calculated inside while loop
    ll per = (curr * sz);
    per /= tot;
    ll c = per;
    c /= 200;
    //the string size could be taken anything above ~50

    char str[123];
    ll i = 0;
    ll j = 0;

    //this loop finds if there should be # or - in percent showing

    for (; i < (50 + j); ++i)
    {
        if ((i - c) < j)
            str[i] = '#';
        else if ((i - c) >= j)
            str[i] = '-';
    }
    //this is printing completion percent (XXX.XX%)
    str[i] = ']', i++;
    str[i] = ' ', i++;
    str[i] = '(', i++;
    j = per;
    str[5 + i] = '0' + (pd), pm;
    str[4 + i] = '0' + (pd), pm;
    str[3 + i] = '.';
    str[2 + i] = '0' + (pd), pm;
    str[i + 1] = '0' + (pd), pm;
    str[i] = '0' + (pd), i += 6;
    str[i] = '%', i++;
    str[i] = ' ';
    str[i] = ')', i++;
    if (j == sz)
    {
        str[i] = '\n', i++;
        str[i] = 'C', i++;
        str[i] = 'o', i++;
        str[i] = 'm', i++;
        str[i] = 'p', i++;
        str[i] = 'l', i++;
        str[i] = 'e', i++;
        str[i] = 't', i++;
        str[i] = 'e', i++;
        str[i] = 'd', i++;
        str[i] = '\n', i++;
    }
    write(1, str, i);
    //this was a little tiresome task and was done to change the approach and error used in my last code
}

char **splitargs(char *command)
{

    char **argarray = malloc(sizeof(char *) * 105);
    if (NULL == argarray)
    {
        printf("malloc Fails");
        printf("\n");
    }
    char *placeholder = strtok(command, " ");
    int pt = 0;
    int ff = 0;
    while (placeholder != NULL)
    {
        if (pt >= sizeof(argarray) / 8)
        {
            argarray = realloc(argarray, sizeof(char *) * 105);
            if (NULL == argarray)
            {
                printf("realloc fails");
                printf("\n");
            }
        }

        argarray[pt] = placeholder;
        placeholder = strtok(NULL, " ");
        pt++;
    }
    argarray[pt] = 0;
    return argarray;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[bs] = {0};
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
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    while (1)
    {
        printf("client > ");
        char *takecommand;
        size_t cmdlength;
        cmdlength = 0;
        fflush(NULL);
        int ret = getline(&takecommand, &cmdlength, stdin);

        for (int i = 0; i < cmdlength; ++i)
        {
            if (takecommand[i] == '\n')
            {
                takecommand[i] = ' ';
            }
        }
        // printf("%s", takecommand);
        if (strcmp(takecommand, " ") == 0 || takecommand[0] == '\0')
        {
            continue;
        }
        char **filename = splitargs(takecommand);
        int i = 1;
        if (strcmp(filename[0], "exit") == 0)
        {
            send(sock, "exit", 5, 0);
            // printf("arkldsfjk\n");
            if (close(sock) == -1)
            {
                perror("Closing connection\n");
            }

            return 0;
        }

        /////NOTE==if use wants that get command must be there then uncomment this///
        if (strcmp(filename[0], "get") != 0)
        {
            {

                printf("Use get command\n");
                continue;
            }
        }
        /////////////////////////////////////////////////////////////////////////////

        while (filename[i] != NULL)
        {
            // printf("%s  is the filename  \n ", filename[i]);
            if (strcmp(filename[i], "exit") == 0)
            {
                send(sock, "exit", 5, 0);
                // printf("arkldsfjk\n");
                if (close(sock) == -1)
                {
                    perror("Closing connection\n");
                }
                return 0;
            }

            if (strcmp(filename[i], " ") == 0 || filename[i][0] == '\0')
            {
                i++;
                continue;
            }
            send(sock, filename[i], strlen(filename[i]), 0); // send the message.
            long long size;
            int valread = read(sock, (char *)&size, sizeof(size));
            // printf("%Ld is the size\n", size);
            // buffer[valread] = '\0';
            long long int b_read = 0;
            if (((char *)&size)[0] == 'D')
            {
                i++;
                printf("File doesn't exists\n");
                continue;
            }
            // printf("%d is the size\n", size);
            // int size = atoi(buffer);
            // printf("the sissss is %d", size);
            if (size == -1)
            {
                i++;
                continue;
            }
            int fd = open(filename[i], O_CREAT | O_WRONLY, 0664);
            if (size == 0)
            {
                close(fd);
                printf("Completed 100%% This file has no text!!\n");
                fflush(NULL);
                i++;
                continue;
            }
            // printf("The size is %d \n", size);
            while (1)
            {
                int rd1 = read(sock, buffer, bs);
                // printf("random\n");
                if (rd1 < 0)
                {
                    perror("Error reading file");
                }
                buffer[rd1] = '\0';
                // printf("%s is the buff \n", buffer);
                int wt2 = write(fd, buffer, strlen(buffer));
                if (wt2 < 0)
                {
                    perror("Error copying file");
                }

                b_read = rd1 + b_read;
                // printf("the size is %d and b_read is  %d ", size, b_read);
                // fflush(NULL);
                // if (size == -1 || size == 0)
                // {
                //     printf("File does not exists\n");
                //     // continue;
                //     break;
                // }
                // else
                // {
                write(1, "\rCopying ..... [", 16);
                print(size, b_read);
                // printf("%d the inside size is\n", size);
                // float percent = (float)b_read / size;
                // printf("\r %% age completed=%f   ", percent * 100);
                // }
                if (b_read == size)
                {
                    break;
                }
            }
            // printf("here is closed\n");
            close(fd);

            i++;
        }
    }
    return 0;
}
