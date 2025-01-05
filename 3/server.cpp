#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <arpa/inet.h>

using namespace std;

void sendFile(string file, int clientSocket)
{
    cout << "Entered sendFile function<<" << endl;
    ifstream myfile(file, ios::binary);
    myfile.seekg(0, ios::end);
    int fileSize = myfile.tellg();
    myfile.seekg(0, ios::beg);
    int hey = fileSize;
    cout << "filesize in server : " << hey << endl;
    fileSize = htonl(fileSize);
    cout << "sending file size " << endl;
    send(clientSocket, &fileSize, sizeof(fileSize), 0);
    cout << "sending file content " << endl;
    for (int i = 0; i < hey; i++)
    {
        char ch = myfile.get();
        cout << "ch : " << ch << endl;
        send(clientSocket, &ch, sizeof(ch), 0);
    }
    cout << "done file  " << endl;
    myfile.close();
    cout << "Exiting sendFile function" << endl;
}
void fileRequests(int clientSocket)
{
    while (true)
    {
        char command[1024] = {0};
        cout << "recv command from client" << endl;

        recv(clientSocket, command, sizeof(command), 0);
        string request(command);
        cout << "request : " << request << endl;
        if (request == "exit")
        {
            break;
        }

        vector<string> files;
        stringstream fileStream(request);
        string file;
        while (fileStream >> file)
        {
            files.push_back(file);
        }

        if (files[0] == "get")
        {
            vector<string> filesToSend;
            for (size_t i = 1; i < files.size(); i++)
            {
                struct stat buffer;
                if (stat(files[i].c_str(), &buffer) == 0)
                {
                    filesToSend.push_back(files[i]);
                }
                else
                {
                    char error[1024] = "no such file";

                    send(clientSocket, error, sizeof(error), 0);
                }
            }

            if (filesToSend.empty())
            {
                char error[1024] = "no such file";
                cout << "sending error message" << endl;
                send(clientSocket, error, sizeof(error), 0);
            }
            else
            {
                char doneMessage[1024] = "ok";
                cout << "sending ok msg " << endl;
                send(clientSocket, doneMessage, sizeof(doneMessage), 0);
                int fileCount = htonl(filesToSend.size());
                cout << "sending fileCount" << endl;
                send(clientSocket, &fileCount, sizeof(fileCount), 0);

                for (const string &fileName : filesToSend)
                {
                    cout << "file Name : " << fileName.c_str() << endl;
                    cout << "file Name Size : " << fileName.length() << endl;
                    cout << "sending fileName" << endl;
                    send(clientSocket, fileName.c_str(), fileName.length(), 0);
                    sendFile(fileName, clientSocket);
                }
            }
        }
        else
        {
            char error[1024] = "no such command";
            send(clientSocket, error, sizeof(error), 0);
        }
    }
}
int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cerr << "Binding failed!" << endl;
        return -1;
    }

    listen(serverSocket, 5);
    cout << "Server is listening on port 8080..." << endl;

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0)
    {
        cerr << "Error accepting client connection!" << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Client connected!" << endl;

    fileRequests(clientSocket);

    close(clientSocket);
    close(serverSocket);
    return 0;
}