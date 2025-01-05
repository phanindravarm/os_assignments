#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;
int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0)
    {
        cout << "Error in accepting client connection" << endl;
        return -1;
    }
    char message[1024] = "hello world !";
    send(clientSocket, message, sizeof(message), 0);
}