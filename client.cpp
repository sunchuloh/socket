#include <iostream>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <wchar.h>
#include <locale.h>

using namespace std;

class ETH
{

private:
    int sockfd;
    struct sockaddr_in Addr_Server; // IP(L3 Header) , Port(L4 Header)

public:
    // Constructor
    ETH(char *, int); // Require Both IP Address & Port.
    ~ETH();
    void Transmit(char *);
    char Receive();
    void Receive(char *, int);

    int GetSockFD();

    ETH *Get_This();
};

ETH ::ETH(char *Address, int Port)
{

    char c;
    cout << "--- Construcing Ethernet Instance ---" << endl;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&Addr_Server, sizeof(Addr_Server));

    Addr_Server.sin_family = AF_INET;
    Addr_Server.sin_port = htons(Port);
    Addr_Server.sin_addr.s_addr = inet_addr(Address);

    if (connect(sockfd, (struct sockaddr *)&Addr_Server, sizeof(Addr_Server)) == 0)
    {

        cout << "--- Get SYN(1) + ACK(1) Flag From Server Sucessfully [ " << Address << " ] ---" << endl;
        cout << "--- Client Enters Establishment To Server [ " << Address << " ] ---" << endl;
    }
}

int ETH ::GetSockFD()
{

    return sockfd;
}

void ETH ::Transmit(char *Buffer)
{

    int state = write(sockfd, Buffer, strlen(Buffer));
    if (state == -1)
    {

        cout << "--- Fail To Transmit Ethernet Data to Server ---" << endl;
    }
}

char ETH ::Receive()
{

    char c;
    int state = read(sockfd, &c, 1);
    if (state == -1)
    {

        cout << "--- Fail To Receive Ethernet Data From Server ---" << endl;
    }
}

void ETH ::Receive(char *Buffer, int size)
{

    int state = read(sockfd, Buffer, size);
    if (state == -1)
    {

        cout << "--- Fail To Receive Ethernet Data From Server ---" << endl;
    }
}
ETH ::~ETH()
{

    cout << "--- Destructing Object : " << this << " ---" << endl;
    close(sockfd);
}

ETH *ETH ::Get_This()
{

    return this;
}

void *Rx_Key_Thread(void *argu);
void *Rx_Msg_Thread(void *argu);
void Error_Handle();

pthread_t Rx_Key_Tid;
pthread_t Rx_Msg_Tid;

ETH *pETH;

int main(int argc, char *argv[])
{

    char Address[1024];
    int Port;

    cout << "Server Address : ";
    cin >> Address;

    cout << "Server Port : ";
    cin >> Port;

    pETH = new ETH(Address, Port);


    if ( pthread_create(&Rx_Key_Tid,NULL,Rx_Key_Thread,NULL) == 0 ) printf("Create Rx_Key_Thread Routine : [ OK ]\n");
    else Error_Handle(); 
    if(pthread_create(&Rx_Msg_Tid,NULL,Rx_Msg_Thread,NULL) == 0 ) printf("Create Rx_Msg_Thread Routine : [ OK ]\n"); 
    else Error_Handle(); 


    if( pthread_join(Rx_Key_Tid,NULL) == 0 ) printf("Join Rx_Key_Thread Routine : [ OK ]\n"); 
    else Error_Handle();



    printf("Exit The Program\n"); 
    delete pETH; 
    return 0;

  
}

void *Rx_Key_Thread(void *argu)
{

    printf("Under Rx_Key_Thread Routine\n");
    char Buffer[1024];
    int SocketFD = pETH->GetSockFD();
    int state;
    char c; 
    int i = 0; 

    bzero(Buffer, 1024); 
    getchar(); 

    while (1)
    {

        c = getchar();
        Buffer[i++] = c;
        if( c == '\b') putchar(' ');
        else if ( c == '\n')
        {

            Buffer[--i] = '\0';
            printf("Client : %s\n",Buffer);
            write(SocketFD,Buffer,strlen(Buffer));
            if( strcmp(Buffer,"quit") == 0 )
            {

                if ( shutdown(SocketFD,SHUT_RDWR) == 0 ) printf("Shutdown SocketFD Write/Read Stream : [ OK ]\n");
                else Error_Handle();


                
            }
            bzero(Buffer,1024);
            i = 0 ; 

        }
    

    }


    printf("End of Rx_Key_Thread Routine\n"); 

 
}

void *Rx_Msg_Thread(void *argu)
{

    printf("Under Rx_Key_Thread Routine\n");
    
    char Buffer[1024];
    char c;
    int SocketFD = pETH->GetSockFD();

    while (1)
    {

        bzero(Buffer, 1024);
        int Status = read(SocketFD, Buffer, 1024);

        if ((Status != 0) && (Status != -1))
        {

            printf("From Host : %s\n", Buffer);
        }
        else if ((Status == 0) || (Status == -1))
        {

            Error_Handle();
            break;
        }
    }

  
    if( close(SocketFD) == 0 ) 
    {
               
        printf("Close Socket : [ OK ]\n");
        if( pthread_cancel(Rx_Key_Tid) == 0 )  printf("Cancel Rx_Key_Thread Routine : [ OK ]\n");
        else Error_Handle(); 
    
    }
    else Error_Handle(); 


    printf("End of Rx_Msg_Thread Routine\n");


}
void Error_Handle()
{

    char *Msg = new char[1024];
    bzero(Msg, 1024);
    sprintf(Msg, "Error Code < %d > ", errno);
    perror(Msg);
    delete[] Msg;
}
