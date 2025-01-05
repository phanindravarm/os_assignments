#include <iostream>
#include <netinet/in.h>
using namespace std;
int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // creating a descriptor so that os knows where is clientSocket
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cout << "Error in establishing connection" << endl;
        return -1;
    }
    char message[1024];
    recv(clientSocket, message, sizeof(message), 0); // recieving message from server
    cout << "Message from server : " << message << endl;
}