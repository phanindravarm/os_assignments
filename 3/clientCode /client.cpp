#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>
using namespace std;
void receiveMessage(int clientSocket, char *buffer)
{
    uint32_t messageLength;
    recv(clientSocket, &messageLength, sizeof(messageLength), 0);
    messageLength = ntohl(messageLength);
    recv(clientSocket, buffer, messageLength, 0);
    buffer[messageLength] = '\0';
}
void receiveFile(int clientSocket, string fileName)
{
    char message[1024];
    receiveMessage(clientSocket, message);

    if (string(message) == "File Not found")
    {
        cout << fileName << " not found" << endl;
        return;
    }
    ofstream file(fileName);
    char charFileSize[1024];
    receiveMessage(clientSocket, charFileSize);

    int fileSize = stoi(charFileSize);

    char content[1024];
    int dataReceived = 0;
    while (dataReceived < fileSize)
    {
        int dataToReceive = min(1024, fileSize - dataReceived);
        int dataRead = recv(clientSocket, content, dataToReceive, 0);
        if (dataRead > 0)
        {
            file.write(content, dataRead);
            dataReceived += dataRead;
        }
    }
    return;
}

void commandFiles(string command, int clientSocket)
{

    send(clientSocket, command.c_str(), command.length(), 0);

    int i = 1;
    char fileName[1024] = {0};
    while (1)
    {

        i++;
        receiveMessage(clientSocket, fileName);

        if (string(fileName) == "END OF FILES")
        {

            break;
        }
        else
        {

            receiveFile(clientSocket, string(fileName));
        }
    }
}

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("connect");
        return -1;
    }
    char message[] = "Hello server";
    char buffer[1024];
    write(clientSocket, message, sizeof(message));
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesReceived] = '\0';
    cout << "Message from server : " << buffer << endl;
    string command;
    while (true)
    {
        cout << "client> ";
        getline(cin, command);
        if (command == "exit")
        {
            break;
        }
        else if (command.substr(0, 3) == "get")
        {
            commandFiles(command, clientSocket);
        }
        else if (command == "")
        {
            continue;
        }
        else
        {
            cout << "Invalid command!" << endl;
        }
    }

    close(clientSocket);

    return 0;
}
