#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // creating a descriptor so that os knows where is clientSocket
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cout << "Error in establishing connection" << endl;
        return -1;
    }
    cout << "Connected " << endl;
    char message[1024];
    recv(clientSocket, message, sizeof(message), 0); // recieving message from server
    cout << "Message from server : " << message << endl;
    close(clientSocket);
}