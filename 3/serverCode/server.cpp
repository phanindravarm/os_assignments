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
void sendMessage(int clientSocket, const string &message)
{
    uint32_t messageLength = htonl(message.size());
    send(clientSocket, &messageLength, sizeof(messageLength), 0);
    send(clientSocket, message.c_str(), message.length(), 0);
}
void sendFile(int clientSocket, string fileName)
{
    ifstream file(fileName);
    string message = "Sending file......";
    if (!file)
    {
        message = "File Not found";
        sendMessage(clientSocket, message);
        return;
    }
    sendMessage(clientSocket, message);

    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.seekg(0, ios::beg);
    string stringFileSize = to_string(fileSize);
    sendMessage(clientSocket, stringFileSize);
    char content[1024];
    while (file)
    {
        file.read(content, 1024);
        int dataRead = file.gcount();
        send(clientSocket, content, dataRead, 0);
    }
    return;
}

void fileRequests(int clientSocket)
{
    char command[1024];

    int length = read(clientSocket, command, sizeof(command));
    command[length] = '\0';

    vector<string> files;
    stringstream fileStream(command);
    string file;
    while (fileStream >> file)
    {
        files.push_back(file);
    }
    for (int i = 1; i < files.size(); i++)
    {

        sendMessage(clientSocket, files[i]);
        sendFile(clientSocket, files[i]);
    }
    sendMessage(clientSocket, "END OF FILES");
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
    cout << "Listetning at port 8000" << endl;
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0)
    {
        cerr << "Error accepting client connection!" << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Client Conneected" << endl;
    char message[] = "Hello client";
    char buffer[1024];
    send(clientSocket, message, sizeof(message), 0);
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesReceived] = '\0';
    cout << "Message from client : " << buffer << endl;
    while (true)
    {
        fileRequests(clientSocket);
    }
    close(serverSocket);
    close(clientSocket);

    return 0;
}
