#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>
using namespace std;
void receiveFile(int clientSocket, string fileName)
{
    ofstream file(fileName);
    int fileSize;
    cout << "recv fileSize" << endl;
    recv(clientSocket, (char *)&fileSize, sizeof(fileSize), 0);
    fileSize = ntohl(fileSize);
    cout << "fileName : " << fileName << endl;
    cout << "fileSize : " << fileSize << endl;
    cout << "recv content" << endl;
    for (int i = 0; i < fileSize; i++)
    {
        char ch;
        recv(clientSocket, &ch, sizeof(ch), 0);
        cout << "ch  : " << ch << endl;
        file << ch;
    }
    cout << "exited recive file Loop : " << endl;
}

void commandFiles(int clientSocket, string command)
{
    cout << "sending command to srever : " << endl;
    send(clientSocket, command.c_str(), command.length(), 0);
    char response[1024];
    cout << "recv response from server" << endl;
    recv(clientSocket, response, sizeof(response), 0);
    string message(response);

    if (message == "no such file" || message == "no such command")
    {
        cout << "wrong";
    }
    else
    {
        int vectorSize;
        cout << "recv vectorSize from server" << endl;
        recv(clientSocket, (char *)&vectorSize, sizeof(vectorSize), 0);
        vectorSize = ntohl(vectorSize);
        for (int i = 0; i < vectorSize; ++i)
        {
            cout << "vectorSize : " << vectorSize << endl;
            string fileName;
            cout << "recv fileName as response" << endl;
            recv(clientSocket, response, sizeof(response), 0);
            cout << "response : " << response << endl;
            fileName = string(response);
            receiveFile(clientSocket, fileName);
        }
    }
    cout << "exited command file loop " << endl;
}
int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.29.155");

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("connect");
        return -1;
    }
    char message[] = "Hello, server!";
    char buffer[1024] = {0};
    string command;
    while (true)
    {
        cout << "entered command loop" << endl;
        cout << "client> ";
        getline(cin, command);

        if (command == "exit")
        {
            break;
        }

        else if (command.substr(0, 3) == "get")
        {
            commandFiles(clientSocket, command);
        }
        else
        {
            cout << "Invalid command!" << endl;
        }
    }

    close(clientSocket);
    return 0;
}