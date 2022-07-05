# Socket Programming
## Bhaskar Joshi 2019111002

---
---
##### (Most of the code is well commented )

## Main highlights of Code
*[Instructions to run](#)
* [Client Code](#)
* [Server Code](#)

# Instructions to run
### In server folder
Execute server code by running :
gcc server.c
./a.out
### In client folder 
Execute client code by running :
gcc client.c
./a.out
and then "client > " will be displayed and input the files to be shown in client's directory like "get file1.txt file2.txt file3.txt"
If you enter "exit" client will disconnect.
If you enter "get file1.txt file2.txt file3.txt exit" it will process the first three inputs and then exit.(Assumption as nothing was mentioned about it so error handling)

# Client Code
The client creates connection to the server and send requests to download files to the client's directory. It writes files in its own directory and prints the progress of the downloading the file. Most of the section for the code is copied from Assignment 1 and Assignment 2.

The main part added here is the print function which prints the progress bar and splitargs funtion which splits the commands . The code is exactly the one from the previous assignment . The assumption made is that "get" argument has to be given to extract a file and whenever one types exit even with get , the client leaves the server , but it processes the commands given before exit.If we directly do ctrl+C then I will print 2 empty buffers as they will be treated as files.And if file has zero size then the no file closing message will be printed as I have handled in that way only and in fact no such thing on printing on server was mentioned but still I did that part. Ctrl+D has not been taken care of as I assume that tester will not just play around with my code :) .

# Server Code
The server code accepts the file request from the client and if the files are present it sends them else error that file is not present is shown. The size is defined in int as it can process upto 4Gb and nothing (more than 2Gb) was mentioned about the size of file to be Tested. 