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
#include <memory>

using namespace std;

/* Socket Class */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Socket Class */

#define BIND_OK 0
#define LISTEN_OK 0
#define ACCEPT_ERROR -1

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

    /* --- Generic Method --- */
};
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Constructor */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
Socket ::Socket(const struct sockaddr_in &s, const int &n)
{

    Queue_Size = n;
    bzero(&Server_Addr, sizeof(Server_Addr));

    Server_Addr.sin_family = s.sin_family;
    Server_Addr.sin_addr.s_addr = s.sin_addr.s_addr;
    Server_Addr.sin_port = s.sin_port;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (SocketFD == -1)
    {
        printf("--- Socket Creation : Fail ---\n");
        perror("Error : ");
        exit(0);
    }
    else
    {

        printf("--- Socket Creation : OK ---\n");
        printf("--- Socket File Descriptor : %d\n", SocketFD);

        int Value = bind(SocketFD, (struct sockaddr *)&Server_Addr, sizeof(Server_Addr));

        if (Value == BIND_OK)
        {
            printf("--- Socket Binding : OK ---\n");
            Bind_State = Value;
        }
        else if (Value != BIND_OK)
        {
            printf("--- Socket Binding : Fail ---\n");
            perror("Error : ");
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

    int Status = close(SocketFD);
    if (Status == -1)
    {
        char Error[1024];
        bzero(Error, 1024);
        sprintf(Error, "Error : %d", errno);
        perror(Error);
    }

    else
        printf(" --- Close Socket : OK --- \n");
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

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Generic Method */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

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
    pthread_t Rx_TID;

    int Client_Number = 0;

    int SessionFD;

    bool Session_Flag;

public:
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

    pthread_t Get_RxTID();
    pthread_t *Get_RxTID_Pointer();

    /* --- Setter ---  */

    pthread_t &Set_RxTID();
    void Set_RxTID(pthread_t tid);

    void Set_Session(bool Flag);
    int &Set_SessionFD();
    void Set_SessionFD(int FD);
    void Set_ClientAddr(struct sockaddr_in &s);
    struct sockaddr_in &Set_ClientAddr();
    void Set_ClientAddr(const sockaddr_in &);
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

    bzero(&Client_Addr, sizeof(Client_Addr));
}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Destructor --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
Host ::~Host()
{

    cout << "Destructing [ " << this << " ] : OK" << endl;
}


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

pthread_t &Host ::Set_RxTID()
{

    return Rx_TID;
}

void Host ::Set_RxTID(pthread_t tid)
{

    Rx_TID = tid;
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

pthread_t Host ::Get_RxTID()
{

    return Rx_TID;
}
pthread_t *Host ::Get_RxTID_Pointer()
{

    return &Rx_TID;
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

pthread_t Server_Ax_TID;
pthread_t Key_Rx_TID;

// unique_ptr<class Socket> pSocket;
// unique_ptr<class Host[]> pHost;
Socket *pSocket;
Host *pHost;

void *Server_Ax_Thread(void *argu);
void *Key_Rx_Thread(void *);
void *Server_Rx_Thread(void *argu);

void Error_Handle();

bool Quit = false;

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
    printf("Ennter Client Queue Size : ");
    scanf("%d", &Queue_Size);

    bzero(&Server_Addr, sizeof(Server_Addr));
    Server_Addr.sin_family = AF_INET;
    Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Server_Addr.sin_port = htons(Port_Num);

    pSocket = new Socket(Server_Addr, Queue_Size);

    // pSocket.reset(new class Socket(Server_Addr, Queue_Size));

    if (pSocket->Set_Listen() == LISTEN_OK)
    {
        printf("--- Socket Listening : OK ---\n");
    }
    else
    {

        printf("--- Socket Listening : Fail ---\n");
        perror("Error : ");
        exit(0);
    }

    pHost = new Host[Queue_Size];

    // pHost.reset(new class Host[Queue_Size]);

    if (pthread_create(&Server_Ax_TID, NULL, Server_Ax_Thread, NULL) != 0)
        Error_Handle();

    else
    {
        if (pthread_detach(Server_Ax_TID) != 0)
            Error_Handle();
    }

    if (pthread_create(&Key_Rx_TID, NULL, Key_Rx_Thread, NULL) != 0)
        Error_Handle();

    else
    {
        if (pthread_detach(Key_Rx_TID) != 0)
            Error_Handle();
    }

    while (1)
    {
        if (Quit == true)
        {
            delete pSocket;
           
            if (pthread_join(Server_Ax_TID, NULL) != -1 )
            {

                printf(" --- Thread Server Ax is Out --- \n");
              
               
            }

            else
            {

                Error_Handle();
            }

            for (int k = 0 ; k < Queue_Size; k++)
            {

                if (pHost[k].CheckSession() == true)
                {
                    pthread_t TID = pHost[k].Get_RxTID();
                    int SessionFD = pHost[k].Get_SessionFD();
                    int Status = close(SessionFD);
                    if (Status == -1)
                        Error_Handle();

                    else
                    {
                        void *buf;
                        printf("Close Session [%d] : OK\n", k);
                        pHost[k].Set_Session(false);
                        if (pthread_join(TID, &buf) != -1)
                        {
                            printf("Server Rx Thread Index [ %d ] is Deleted\n", *(int *)buf);
                        }
                        else
                        {
                            Error_Handle();
                        }
                    }
                }
            }
        }
    }

    return 0;
}
void *Server_Ax_Thread(void *argu)
{

    int SocketFD = pSocket->Get_SocketFD();
    int Queue_Size = pSocket->Get_QueueSize();

    while (1)
    {

        struct sockaddr_in Client_Addr;
        bzero(&Client_Addr, sizeof(Client_Addr));
        int len = sizeof(Client_Addr);
        int FD = accept(SocketFD, (struct sockaddr *)&Client_Addr, (socklen_t *)&len);

        if (FD == -1)
        {

            char Error[1024];
            bzero(Error, 1024);
            sprintf(Error, "Error No <%d> : ", errno);
            perror(Error);
            if (errno == EBADF)
            {

                printf("Bad Socket File Descriptor\n");
                break;
            }
        }
        else if (FD != -1)
        {
            int idx;
            /* Searching Disabled Session */
            for (int n = 0; n < Queue_Size; n++)
            {

                if (!pHost[n].CheckSession())
                {
                    idx = n;
                    break;
                }
            }
            pHost[idx].Set_ClientAddr() = Client_Addr;
            pHost[idx].Set_Session(true);
            pHost[idx].Set_SessionFD() = FD;
            pSocket->Set_SessionNum()++;

            printf("--- Socket Accept : OK ---\n");
            printf("--- New Session Information ---\n");
            printf("(1). Session Index : %d\n", idx);
            printf("(2). The Number of Active Session : %d\n", pSocket->Get_SessionNum());
            printf("(3). Session File Descriptor : %d\n", idx, pHost[idx].Get_SessionFD());
            printf("(4). Session Client Address : %s\n", inet_ntoa(pHost[idx].Get_ClientAddr().sin_addr));
            pthread_create(pHost[idx].Get_RxTID_Pointer(), NULL, Server_Rx_Thread, &idx);
        }
    }

    char *Buf = new char[1024];
    char *ptr = strcpy(Buf, "Thread Server Ax is Out\n");
    return (void *)ptr; 
}

void *Server_Rx_Thread(void *argu)
{
    int idx = *(int *)argu;
    char* Buffer = new char[1024]; 

    // unique_ptr<char[]> Buffer ( new char[1024]);

    int SessionFD = pHost[idx].Get_SessionFD();
    char *Address = inet_ntoa(pHost[idx].Get_ClientAddr().sin_addr);

    printf(" --- TCP Rx Thread --- \n");
    printf(" --- Session index : %d --- \n", idx);

    while (1)
    {

        bzero(Buffer, 1024);
        int Status = read(SessionFD, Buffer, 1024);

        if ( (Status != 0) && (Status != -1) )
        {

            printf("From [%s] : %s\n", Address, Buffer);
        }

        else if (Status == 0)
        {

            printf("Session <%d> Got EOF\n", idx);

            char Error[1024];
            bzero(Error, 1024);
            sprintf(Error, "Error No <%d> : ", errno);
            perror(Error);

            break;
        }

        else if (Status == -1)
        {

            char Error[1024];
            bzero(Error, 1024);
            sprintf(Error, "Error No <%d> : ", errno);
            perror(Error);
            break;
        }
    }

    int Status = close(SessionFD);
    if (Status == -1)
    {
        char Error[1024];
        bzero(Error, 1024);
        sprintf(Error, "Error No <%d> : ", errno);
        perror(Error);
    }
    else
    pHost[idx].Set_Session(false);


    delete [] Buffer; 
    return new int(idx);



}

void *Key_Rx_Thread(void *argu)
{

    printf("--- Key Rx Thread --- \n");
    int Queue_Size = pSocket->Get_QueueSize();
    char Buffer[1024];
    int idx = 0;
    char c;

    getchar();

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

            if (strcmp("quit", Buffer) == 0)
            {

                Quit = true;
                break;
                // 1. Close Session
                // 2. Close Thread
                // 3. Close Socket.
            }
            bzero(Buffer, 1024);
            idx = 0;
        }
    }

    return NULL;
}

void Error_Handle()
{

    char *Buf = new char[1024];
    bzero(Buf, 1024);
    sprintf(Buf, "Error No < %d > : ", errno);
    perror(Buf);
    delete[] Buf;
}
