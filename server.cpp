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

class Socket;
class Host;

void *Ax_Cli_Thread(void *argu);
void *Rx_Key_Thread(void *argu);
void *Rx_Msg_Thread(void *argu);

void Error_Handle();

pthread_t Ax_Cli_Tid;
pthread_t Rx_Key_Tid;

// unique_ptr<class Socket> pSocket;
// unique_ptr<class Host[]> pHost;

Socket *pSocket;

#define BIND_OK 0
#define LISTEN_OK 0
#define ACCEPT_ERROR -1

/* Host Class */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Host Class */

class Host
{

private:
    struct sockaddr_in Client_Addr;

    int Binding_Flag;
    int Listening_Flag;

    pthread_t Rx_Msg_Tid;

    int Client_Number = 0;

    int SessionFD;
    bool Session_Flag;

public:
    /* Static  */

    /* --- Default Constructor ---*/
    Host();

    /* --- Destructor --- */
    ~Host();

    /* --- Getter ---  */

    Host *Get_ThisPointer();
    Host &Get_Instance();

    bool Get_Session();
    struct sockaddr_in Get_ClientAddr();
    int Get_SessionFD();
    int Get_ClientNum();
    int Get_Listening_Flag();
    int Get_Binding_Flag();
    int Get_Established_File_Descriptor();

    pthread_t Get_Rx_Msg_Tid();
    pthread_t *Get_Rx_Msg_Tid_Pointer();

    int Get_SessionList();

    /* --- Setter ---  */

    void Set_Session(bool Flag);
    int &Set_SessionFD();
    void Set_SessionFD(int FD);
    void Set_ClientAddr(const struct sockaddr_in &s);
    struct sockaddr_in &Set_ClientAddr();

    void Set_Port_Number(int Value);
    void Set_Client_Number(int Value);
    void Set_Listening_Flag(int Value);
    void Set_Binding_Flag(int Value);
    void Set_Established_File_Descriptor(int File_Descriptor, int Index);

    /* --- Generic Method --- */

    void Transmit(char *Buffer, int File_Descriptor);
    void Transmit(char *Buffer);
    char Receive();
    char Receive(int File_Descriptor);
    void Receive(char *Buffer, int Size);

    bool CheckSession();

    /* --- OVerloaDinG --- */
};
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Default Constructor  --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

Host ::Host()
{
    //  cout << "Host Instance [ " << this << " ] is Created " << endl;
    bzero(&Client_Addr, sizeof(Client_Addr));
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Destructor --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
Host ::~Host()
{

    cout << "Destructing Host Instance [ " << this << " ] : OK" << endl;
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Static --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Setter --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
void Host ::Set_ClientAddr(const struct sockaddr_in &s)
{

    Client_Addr = s;
}

struct sockaddr_in &Host ::Set_ClientAddr()
{

    return Client_Addr;
}

void Host ::Set_Session(bool Flag)
{

    Session_Flag = Flag;
}

int &Host ::Set_SessionFD()
{

    return SessionFD;
}

void Host ::Set_SessionFD(int FD)
{

    SessionFD = FD;
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Getter --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
Host *Host ::Get_ThisPointer()
{

    return this;
}

Host &Host ::Get_Instance()
{

    return *this;
}

struct sockaddr_in Host ::Get_ClientAddr()
{

    return Client_Addr;
}

int Host ::Get_SessionFD()
{

    return this->SessionFD;
}

int Host ::Get_Binding_Flag()
{

    return Binding_Flag;
}
int Host::Get_Listening_Flag()
{

    return Listening_Flag;
}

int Host ::Get_ClientNum()
{

    return Client_Number;
}

pthread_t Host ::Get_Rx_Msg_Tid()
{

    return Rx_Msg_Tid;
}
pthread_t *Host ::Get_Rx_Msg_Tid_Pointer()
{

    return &Rx_Msg_Tid;
}

/* 활성화된 Sessino List 반환*/
int Host ::Get_SessionList()
{
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Generic Method --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
bool Host ::CheckSession()
{

    return Session_Flag;
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- OveRLoadInG ---*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/

/* Global */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Global */

class Socket
{

private:
    struct sockaddr_in Server_Addr;
    bool Quit_Flag = false;
    int Port_Number;
    int Queue_Size;
    int Bind_State;
    int SocketFD;
    int Session_Number = 0;
    Host *pHost;

public:
    /* --- Constructor --- */

    Socket(const struct sockaddr_in &s, const int &n);

    /* --- Destructor --- */
    ~Socket();

    /* --- Setter --- */

    int &Set_SessionNum();

    int Set_Listen();
    void Set_Binding_Flag(int Flag);
    void Set_Listening_Flag(int Flag);

    /* --- Getter --- */

    int Get_SessionNum();

    int Get_SocketFD();
    int Get_QueueSize();

    int Get_Binding_Flag();
    int Get_Listeing_Flag();

    Host &Get_Host(int idx);

    /* --- Generic Method --- */

    void PrintSession();
};
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Constructor */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
Socket ::Socket(const struct sockaddr_in &s, const int &n)
{

    Queue_Size = n;

    pHost = new Host[Queue_Size];

    bzero(pHost, sizeof(Host) * Queue_Size);
    bzero(&Server_Addr, sizeof(Server_Addr));

    Server_Addr.sin_family = s.sin_family;
    Server_Addr.sin_addr.s_addr = s.sin_addr.s_addr;
    Server_Addr.sin_port = s.sin_port;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (SocketFD == -1)
    {
        Error_Handle();
        exit(0);
    }
    else
    {

        printf("Create Socket : [ OK ]\n");
        printf("Socket File Descriptor : %d\n", SocketFD);

        int Value = bind(SocketFD, (struct sockaddr *)&Server_Addr, sizeof(Server_Addr));

        if (Value == BIND_OK)
        {
            printf("Bind Socket : [ OK ]\n");
            Bind_State = Value;
        }
        else if (Value != BIND_OK)
        {
            Error_Handle();
            Bind_State = Value;
            exit(0);
        }
    }
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Destructor */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
Socket ::~Socket()
{
    cout << "Destructing Socket Instance [ " << this << " ] : OK" << endl;
    delete[] pHost;
}

void Socket ::PrintSession()
{

    for (int i = 0; i < Queue_Size; i++)
    {

        if (Get_Host(i).CheckSession())
        {

            int SessionFD = Get_Host(i).Get_SessionFD();
            char *Addr = inet_ntoa(Get_Host(i).Get_ClientAddr().sin_addr);

            printf("Host[%d] : Enabled\n", i);
            printf("`-Cli Addr : %s\n", Addr);
            printf("`-SessionFD : %d\n", SessionFD);
        }
    }
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Setter */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

int &Socket ::Set_SessionNum()
{

    return Session_Number;
}

int Socket ::Set_Listen()
{

    int state;
    if (Bind_State == BIND_OK)
        state = listen(SocketFD, Queue_Size);
    return state;
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Getter */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

int Socket ::Get_SocketFD()
{

    return SocketFD;
}
int Socket ::Get_QueueSize()
{

    return Queue_Size;
}

int Socket ::Get_SessionNum()
{

    return Session_Number;
}

Host &Socket ::Get_Host(int idx)
{

    return pHost[idx];
}

/* main */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* main */

int main(int argc, char *argv[])
{

    struct sockaddr_in Server_Addr;

    int Count = 0;
    /* Queue Size means The Number of Client You Want to Accpet.  */
    /*  지정된 Queqe Size만큼 Client의 접속을 허용한다. */
    int Queue_Size;
    int Port_Num;

    printf("Enter Port Number : ");
    scanf("%d", &Port_Num);
    printf("Enter Client Number : ");
    scanf("%d", &Queue_Size);

    bzero(&Server_Addr, sizeof(Server_Addr));
    Server_Addr.sin_family = AF_INET;
    Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Server_Addr.sin_port = htons(Port_Num);

    pSocket = new Socket(Server_Addr, Queue_Size);
    // pSocket->CreateHostInstace();

    // pSocket.reset(new class Socket(Server_Addr, Queue_Size));

    if (pSocket->Set_Listen() == LISTEN_OK)
    {
        printf("Socket Listening : [ OK ]\n");
    }
    else
    {

        Error_Handle();
        exit(0);
    }

    // pHost.reset(new class Host[Queue_Size]);

    if (pthread_create(&Ax_Cli_Tid, NULL, Ax_Cli_Thread, NULL) != 0) printf("Create Ax_Cli_Tid Thread Routine : [ OK ]\n");
        Error_Handle();

    if (pthread_create(&Rx_Key_Tid, NULL, Rx_Key_Thread, NULL) != 0) printf("Create Rx_Key_Thread Routine : [ OK ]\n"); 
        Error_Handle();

        

    if (pthread_join(Rx_Key_Tid, NULL) == 0)
        printf("Join Rx_Key_Thread Routine : [ OK ]\n");
    else
        Error_Handle();

    printf("Exit The Program\n");
    return 0;
}
void *Ax_Cli_Thread(void *argu)
{
    
    int SocketFD = pSocket->Get_SocketFD();
    int Queue_Size = pSocket->Get_QueueSize();

    while (1)
    {

        struct sockaddr_in Client_Addr;
        bzero(&Client_Addr, sizeof(Client_Addr));
        int len = sizeof(Client_Addr);

        /* Shutdown(SocketFD,SHUT_RD)를 통해 accpet()함수 Abort */
        int FD = accept(SocketFD, (struct sockaddr *)&Client_Addr, (socklen_t *)&len);

        if (FD == -1)
        {
            printf("Aborting accpet() : [ OK ]\n");
            Error_Handle();

            break;
        }
        else if (FD != -1)
        {
            int i;
            /* Searching Disabled Session */
            for (int k = 0; k < Queue_Size; k++)
            {

                if (!pSocket->Get_Host(k).CheckSession())
                {
                    printf("Host[%d] : Disabled\n", k);
                    i = k;
                    break;
                }
            }

            pSocket->Get_Host(i).Set_ClientAddr(Client_Addr);
            pSocket->Get_Host(i).Set_SessionFD(FD);
            pSocket->Get_Host(i).Set_Session(true);

            pSocket->Set_SessionNum()++;

            printf("!!! --- New Session is Enabled --- !!!\n");
            printf("Session Number : [ %d ] \n", i);
            printf("Host[%d]'s Client Address : [ %s ]\n", i, inet_ntoa(pSocket->Get_Host(i).Get_ClientAddr().sin_addr));
            printf("Host[%d]'s SessionFD : [ %d ]\n", i, FD);
            if (pthread_create(pSocket->Get_Host(i).Get_Rx_Msg_Tid_Pointer(), NULL, Rx_Msg_Thread, &i) != 0)
                Error_Handle();
        }
    }

    printf("End of Ax_Cli_Thread Routine\n");
}

void *Rx_Msg_Thread(void *argu)
{
    // unique_ptr<char[]> Buffer ( new char[1024]);

    int idx = *(int *)argu;
    char *Buffer = new char[1024];

    int SessionFD = pSocket->Get_Host(idx).Get_SessionFD();
    char *Address = inet_ntoa(pSocket->Get_Host(idx).Get_ClientAddr().sin_addr);

    printf("Rx_Msg_Thread[%d] Routine Enabled\n", idx);

    while (1)
    {

        bzero(Buffer, 1024);
        /* shutdown(SessionFD,SHUT_RD)를 통해 read()함수 Abort */
        int Status = read(SessionFD, Buffer, 1024);

        if ((Status != 0) && (Status != -1))
        {

            printf("From [ %s ] : %s\n", Address, Buffer);
        }

        else if (Status == 0)
        {

            /* EOF From Client */

            Error_Handle();
            printf("Aborting read() : [ OK ]\n");
            break;
        }

        else if (Status == -1)
        {

            Error_Handle();
            break;
        }
    }

    printf("End of Rx_Msg_Thread[%d] Routine\n", idx);

    if (close(SessionFD) == 0)
    {
        printf("Close Host[%d]'s Session : [ OK ]\n", idx);
        pSocket->Get_Host(idx).Set_Session(false);
        pSocket->Set_SessionNum()--;
    }
    else
        Error_Handle();

    delete[] Buffer;
}

void *Rx_Key_Thread(void *argu)
{

    printf("Under Rx_Key_Thread Routine\n");

 

    int Queue_Size = pSocket->Get_QueueSize();
    char Buffer[1024];
    int idx = 0;
    int SocketFD = pSocket->Get_SocketFD();

    char c;

    bzero(Buffer, 1024);
    getchar();


    printf("Command list\n");
    printf("(1).quit\n");
    printf("`- Quit Both Server and Client Program\n"); 
    printf("(2).list\n");
    printf("`- Show The List of Session\n");

    while (1)
    {

        c = getchar();
        Buffer[idx++] = c;

        if (c == '\b')
        {
            putchar(' ');
        }
        else if (c == '\n')
        {

            Buffer[--idx] = '\0';

            printf("Console : %s\n", Buffer);

            for (int n = 0; n < Queue_Size; n++)
            {

                if (pSocket->Get_Host(n).CheckSession() == true)
                {
                    Host &HostInst = pSocket->Get_Host(n);

                    int SessionFD = HostInst.Get_SessionFD();

                    write(SessionFD, Buffer, strlen(Buffer));
                }
            }

            if (strcmp("quit", Buffer) == 0)
            {

                printf("You Order Quit\n");

                if (shutdown(SocketFD, SHUT_RD) == -1)
                    perror("Error ");
                else
                {

                    printf("Shutdown SocketFD Read Stream : [ OK ]\n");

                    if (pthread_join(Ax_Cli_Tid, NULL) != 0)
                        perror("Error ");
                    else
                    {

                        printf("Join Ax_Cli_Thread Routine : [ OK ]\n");
                        if (close(SocketFD) != 0)
                            Error_Handle();

                        else
                            printf("Close Socket : [ OK ]\n");
                    }
                }

                for (int k = 0; k < Queue_Size; k++)
                {

                    if (pSocket->Get_Host(k).CheckSession())
                    {
                        int SessionFD = pSocket->Get_Host(k).Get_SessionFD();
                        write(SessionFD, "--- Server is Shutdown ---\n", strlen("--- Server is Shutdown ---\n"));

                        if (shutdown(SessionFD, SHUT_RDWR) == -1)
                            Error_Handle();

                        else
                        {
                            printf("Shutdown Host[%d] Read/Write Stream : [ OK ]\n", k);
                            if (pthread_join(pSocket->Get_Host(k).Get_Rx_Msg_Tid(), NULL) != 0)
                                Error_Handle();
                            else
                            {
                                printf("Join Rx_Msg_Thread[%d] Routine : [ OK ]\n", k);
                                if (close(SessionFD) != 0)
                                    Error_Handle();

                                else
                                {
                                    printf("Close Host[%d]'s Session : [ OK ]\n", k);
                                    pSocket->Get_Host(k).Set_Session(false);
                                    pSocket->Set_SessionNum()--;
                                }
                            }
                        }
                    }
                }

                delete pSocket;
                break;
            }

            else if (strcmp("list", Buffer) == 0)
            {

                pSocket->PrintSession();
            }
            bzero(Buffer, 1024);
            idx = 0;
        }
    }

    printf("End of Key_Rx_Thread Routine\n");
}

void Error_Handle()
{

    char Buffer[1024];
    bzero(Buffer, 1024);
    sprintf(Buffer, "Error(%d) : %s", errno, strerror(errno));
    printf("%s\n",Buffer); 
   
}
