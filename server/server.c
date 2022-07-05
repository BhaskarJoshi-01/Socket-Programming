
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define PORT 8000
#define bs 5000
#define ll long long int
#define pm per /= 10;
#define pd per % 10
#define sz 10000

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[bs] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // This is to lose the pesky "Address already in use" error message
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;         // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address - listens from all interfaces.
    address.sin_port = htons(PORT);       // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Port bind is done. You want to wait for incoming connections and handle them in some way.
    // The process is two step: first you listen(), then you accept()
    if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // printf("adklfjlksad\n");
        // fflush(NULL);

        // Port bind is done. You want to wait for incoming connections and handle them in some way.
        // The process is two step: first you listen(), then you accept()

        // returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        while (1)
        {
            int flag = 0;
            // write(2, "above read\n", 12);
            // fflush(NULL);
            valread = read(new_socket, buffer, bs);
            buffer[valread] = '\0';
            // printf("Inside 2nd while 1 %s  \n", buffer);
            // fflush(NULL);
            if (strcmp(buffer, "exit") == 0)
            {
                printf("Client has disconnected\n");
                break;
            }
            printf("%s is the buffer\n", buffer);

            int fd = open(buffer, O_RDONLY);
            if (fd < 0)
            {
                perror("Error reading file ");
                flag = 1;
                // continue;
            }
            if (flag == 1)
            {
                printf("file does not exists\n");
                send(new_socket, "DoNotEx", 8, 0);
                continue;
            }
            long long b_read = 0;
            long long size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            // sprintf(buffer, "%d", size);

            send(new_socket, (char *)&size, sizeof(size), 0);
            if (!size || size == -1)
            {
                continue;
            }
            // else
            while (1)
            {
                int rd1 = read(fd, buffer, bs);

                if (rd1 < 0)
                {
                    printf("%d fd is \n", fd);
                    fflush(NULL);
                    perror("Error reading file");
                }
                buffer[rd1] = '\0';

                // printf("%s is the buffer\n", buffer);
                int wt2 = send(new_socket, buffer, strlen(buffer), 0);
                if (wt2 < 0)
                {
                    perror("Error copying file");
                }

                b_read = rd1 + b_read;

                // write(1, "\rCopying ..... [", 16);

                // printf("the size is %d and b_read is  %d ", size, b_read);
                // fflush(NULL);

                if (b_read == size)
                {
                    break;
                }
            }
            printf("Closing fd for the file\n");
            fflush(NULL);

            close(fd);
            // i++;
        }
    } // read infromation received into the buffer
    return 0;
}
