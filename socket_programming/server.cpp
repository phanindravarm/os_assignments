#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;
int main()
{
    // creating serverSocket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // creating a descriptor so that os knows where is serverSocket
    // AF_INET : Communicating between processes connected by IPV4(do not ask what is IPV4)
    // SOCK_STREAM : Communication type like TCP,UDP
    // 0 : Automatically choses protocol based on above two
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);                                         // setting port number and convert it to one format which i am not sure of
    serverAddress.sin_addr.s_addr = INADDR_ANY;                                   // setting the server such that it accepts from any available connections either local or remote
    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); // bind all information declare above to serverSocket
    listen(serverSocket, 5);                                                      // serverSocket listens and max connections in queue is 5
    int clientSocket = accept(serverSocket, nullptr, nullptr);                    // creating a descriptor so that os knows where is clietSocket and it communicates with this
    if (clientSocket < 0)
    {
        cout << "Error in accepting client connection" << endl;
        return -1;
    }
    char message[1024] = "hello world !";
    send(clientSocket, message, sizeof(message), 0); // sending message
}