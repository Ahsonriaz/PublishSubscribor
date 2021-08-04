#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

void dopro(int sock);
void sendmess(int sock, string Serveroutput);
void getmessage(char buf[4096], int bytes);
mutex mutex_;

int server_func()
{

    int listening = socket(AF_INET, SOCK_STREAM, 0);

    if (listening <= 0)
    {
        cout << "port not opened" << endl;
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;

    hint.sin_port = htons(54000);

    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1)
    {

        cout << "cannot bind to  IP/PORT";
        return -2;
    }

    if (listen(listening, 10) == -1)
    {

        cout << "can't listen";
        return -3;
    }

    sockaddr_in client;
    socklen_t clientsize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    listen(listening, 10);

    char buf[4096];
    int bytes;
    string userinput;

    while (true)
    {
        int clientsocket = accept(listening, (sockaddr *)&client, &clientsize);

        cout << endl
             << endl
             << "\033[1;0m"
             << " New Client Has Connected" << endl
             << endl;

        thread client(dopro, clientsocket);

        client.detach();
    }
    return 0;
}

void dopro(int sock)
{

    char buf[4096];
    int bytes;
    string userinput;

    while (true)
    {
        //unique_lock<mutex> lock(mutex_);

        //clear buffer
        memset(buf, 0, 4096);

        //wait for a message
        int bytes = recv(sock, buf, 4096, 0);
        if (bytes < 0)
        {

            cout << "The client disconnected1" << endl;
            break;
        }

        else if (bytes == 0)
        {

            cout << "The client disconnected" << endl;
            close(sock);
        }
        
        
        //display
        cout << "\033[1;32m"<< "Recieved: "<< "\033[1;31m" << string(buf, 0, bytes) << endl;

        string echo= string(buf,0,bytes);

        //send to server
        int sendresult = send(sock, echo.c_str(), echo.size() + 1, 0);
    }
}

int main()
{

    cout << "SERVER CONNECTED" << endl;
    server_func();
}

// //display message

//write client :
//new client thread only for that client
//one main thread only for accept - blocking
//asynchronus comm with timeout, all clients single thread, file descriptor non blocking
//after client, go to while loop to accept second client
//new client new thread

//when client close socket, disconnect thread/kill

// TCP/IP->connection oriented, needs an assigned endpoints

// Server
// socket()-> bind()->listen ()->accept() -> read() /write()->close

// client
// socket()->connect()->read/write->close

// structures used? sockaddr

// network vyte order -> big endian
// linux -> intel -> little edian
// convert little endian to big endian -> host order to network order

//debugger
//configure debugger

