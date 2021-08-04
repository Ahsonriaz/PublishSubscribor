#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

//only sending one message??

//create a socket

int client_funct()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {

        cout << "socket not created" << endl;
        return -1;
    }

    //create hint structure for the server we are connecting
    int port = 54000; //what happens if client and server have different ports?
    string ipaddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipaddress.c_str(), &hint.sin_addr);

    //connect to the remote server using networking
    int connectresult = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if (connectresult == -1)
    {

        cout << "connection to remote server failed." << endl;
        return -2;
    }

    //while loop for operation

    char buff[4096];
    string userinput;

    do
    {

        //enter text get entire line
        cout << " You: ";
        getline(cin, userinput);

        //send to server
        int sendresult = send(sock, userinput.c_str(), userinput.size() + 1, 0);

        //check if sending result failed

        if (sendresult == -1)
        {

            cout << "could not send to server";
            //jump back to sending message do not terminate.
            continue;
        }

        //wait for response
        memset(buff, 0, 4096);
        int bytesrecieved = recv(sock, buff, 4096, 0);

        //display response
        cout << "Server: " << string(buff, 0, bytesrecieved) << endl;

    } while (true);

    close(sock);

    return 0;
}

int main()
{

    client_funct();
}
