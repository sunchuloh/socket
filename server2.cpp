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
// #include "./server.h"

using namespace std;

class TCP
{

private:
    struct sockaddr_in *Cli_Addr;
    struct Channel *Channel;

    int *EstFD;
    bool* Con_State;

    bool Bind_Flag = false;
    bool Listen_Flag = false;
    int Binding_State;
    int Listening_State = -1;

    int sockfd;
    int Queue_Num = 0; 

    int Port_Number;
    int Client_Number;
    int Active_Channel_Number = 0;
    struct sockaddr_in Serv_Addr;

public:
    /* --- Constructor --- */

    TCP(const int &_Port_Num, const int &_Cli_Num);
    TCP(const sockaddr_in &s);

    /* --- Destructor --- */
    ~TCP();

    // This Pointer
    TCP *Get_This();
    TCP &Get_Instace();


      /* --- Setter --- */

    struct sockaddr_in &Set_Cli_Addr(int idx);
    struct sockaddr_in &Set_Cli_Addr(const sockaddr_in &s, int idx);

    int &Set_ActiveChannelNum();

    bool& Set_ConState(int idx); 

    int &Set_EstFD(int idx);

    void Set_EstFD(int Estfd, int idx);

    void Set_ActiveChannelNum(const int &Number);

    void Set_ChannelActive(int idx);

    void Set_PortNum(int value);
    void Set_BindFlag(bool flag);
    int& Set_QueueNum(); 

    void Set_Listening_State(int Value);


    /* ---Getter --- */

    int Get_EstFD(int i) const;
    int Get_QueueNum() const; 

    struct sockaddr_in &Get_Cli_Addr(int k);
    struct sockaddr_in Get_Serv_Addr();

    int Get_PortNum();
    int Get_GuestNum();
    int &Get_Listening_State();
    int &Get_Binding_State();
    int Get_SocketFD();

    int Get_ActiveChannelNum();

    bool Get_BindFlag();
    bool Get_ListenFlag();
    bool Get_ChannelActiveState(int idx);

    bool Get_ConState(int idx); 

    int getClientNumber();


  

    /* --- Generic Method */
    void EnableChannel(int i);
    void DisableeChannel(int i);

    void ListActiveChannel();
    void CountActiveChannelNum();
    int Transmit(char *Buffer, int i);
    char Receive(int i);
    void Receive(char *Buffer, int Size);
    int Receive(int i, char *Buffer, int size);
    int Receive(int i, char &c);
};

/* --- Constructor --- */
TCP ::TCP(const int &_Port_Numer, const int &_Client_Numer )
{

    Client_Number = _Client_Numer;
    Port_Number = _Port_Numer;

    cout << "--- Creating Ethernet Instance ---" << endl;
    cout << "--- Guest Number : " << Client_Number << " --- " << endl;
    cout << "--- Port Number : " << Port_Number << " --- " << endl;

    Con_State = new bool[Client_Number]; 
    EstFD = new int[Client_Number];
    Cli_Addr = new struct sockaddr_in[Client_Number];

    // bzero(pChannelFD, sizeof(int) * Guest_Num);

    bzero(Con_State,sizeof(bool)*Client_Number); 
    bzero(EstFD, sizeof(int) * Client_Number);
    bzero(Cli_Addr, sizeof(struct sockaddr_in) * Client_Number);
    bzero(&Serv_Addr, sizeof(Serv_Addr));

    Serv_Addr.sin_family = AF_INET;
    Serv_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Serv_Addr.sin_port = htons(Port_Number);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error : ");
        exit(0);
    }
    else
    {

        cout << "Socket Creation : OK " << endl;
        cout << "Socket File Descriptor : " << sockfd << endl;
        int state = bind(sockfd, (struct sockaddr *)&Serv_Addr, sizeof(Serv_Addr));
        if (state == 0)
        {
            int state;
            int tempfd = sockfd;
            cout << "Socket Binding : OK " << endl;
            state = listen(tempfd, Client_Number);
            if (state == 0)
            {

                Listen_Flag = true;
            }
            else if (state == -1)
            {

                perror("Error : ");
                Listen_Flag = false;
            }
        }
        else if (state != 0)
        {
            perror("Error :");
        }
    }
}

/* --- Generic Method --- */

char TCP ::Receive(int i)
{
}
void TCP ::Receive(char *Buffer, int Size)
{
}
int TCP ::Receive(int i, char *Buffer, int size)
{
}
int TCP ::Receive(int i, char &c)
{
}

void TCP ::CountActiveChannelNum()
{

    Active_Channel_Number++;
}


/* --- Setter --- */

struct sockaddr_in &TCP ::Set_Cli_Addr(int k)
{

    return Cli_Addr[k];
}
struct sockaddr_in &TCP ::Set_Cli_Addr(const sockaddr_in &s, int i)
{

    Cli_Addr[i] = s;
}

void TCP ::Set_BindFlag(bool Flag)
{

    Bind_Flag = Flag;
}



int &TCP ::Set_EstFD(int idx)
{

    return EstFD[idx];
}

void TCP ::Set_EstFD(int idx, int fd)
{

    EstFD[idx] = fd;
}

void TCP ::Set_ActiveChannelNum(const int &Number)
{

    Active_Channel_Number = Number;
}

void TCP ::Set_Listening_State(int Value)
{

    this->Listening_State = Value;
}

int& TCP :: Set_QueueNum()
{


return Queue_Num; 

}

bool& TCP :: Set_ConState(int idx)
{

return Con_State[idx]; 

}

/* --- Getter --- */

int TCP :: Get_QueueNum() const
{

    return Queue_Num; 
}

int TCP ::Get_EstFD(int idx) const
{

    return EstFD[idx];
}

struct sockaddr_in TCP ::Get_Serv_Addr()
{

    return Serv_Addr;
}

struct sockaddr_in &TCP ::Get_Cli_Addr(int i)
{

    return Cli_Addr[i];
}

bool TCP ::Get_ListenFlag()
{

    return Listen_Flag;
}

int TCP ::Get_ActiveChannelNum()
{

    return Active_Channel_Number;
}

TCP *TCP ::Get_This()
{

    return this;
}


int TCP ::Get_SocketFD()
{

    return sockfd;
}

bool TCP ::Get_BindFlag()
{

    return Bind_Flag;
}

bool TCP :: Get_ConState(int i)
{


return Con_State; 



}


int TCP :: getClientNumber(){



return Client_Number; 

}

/* --- Destructor --- */
TCP ::~TCP()
{
    int State; 
    cout << "Destructing TCP Instance " << this << " is Done" << endl;

    State = close(sockfd);

    if (State == -1)
    {

        perror("Error : ");
    }
    else if (State == 0)
    {

        cout << "close socket : " << sockfd << " OK "; 
    }

    delete[] EstFD;
    delete[] Cli_Addr;
}

bool gQuit = false;

TCP *Server;
pthread_t* TCP_Rx_TID; 
int SocketFD; 




pthread_t TCP_Ax_TID;
pthread_t Key_Rx_TID; 


void *TCP_Rx_Routine(void *argu);
void *Key_Rx_Routine(void *);
void *TCP_Ax_Routine(void *argu);

int main(int argc, char *argv[])
{

    int Count = 0;
    int Guest;
    int Port;

    cout << "Enter a Port : ";
    cin >> Port;
    cout << "Enter the number of client : ";
    cin >> Guest;

    Server = new TCP(Port, Guest);
    TCP_Rx_TID = new pthread_t[Guest];
    SocketFD = Server->Get_SocketFD(); 


    if (pthread_create(&Key_Rx_TID, NULL, Key_Rx_Routine, NULL) != 0)
        perror("Error : ");

    if (pthread_create(&TCP_Ax_TID, NULL, TCP_Ax_Routine, NULL) != 0)
        perror("Error : ");

    while (1)
    {

    }

   
   

    return 0;
}

void *Key_Rx_Routine(void *argu)
{

    cout << "Under KeyButton Rx Task Thread Entry Point" << endl;

    // int idx = *(int *)argu;
    int k = 0;
    char c;
    char *Buffer = new char[1024];
    bzero(Buffer,1024); 

    getchar();

    while (1)
    {

        c = getchar();
        Buffer[k++] = c;
        if (c == '\b')
        {

            putchar(' ');
        }
        else if (c == '\n')
        {

            cout << "Console : " << Buffer;

            if (strcmp("quit\n", Buffer) == 0)
            {

                 
                gQuit = true;
                
                break; 
                /*
                Est Number -> Check Active State -> Close Sokcet. 
                */
              
                // memset(Buffer, 0x0, sizeof(Buffer) * 1024);
            }
            memset(Buffer, 0x0, sizeof(Buffer) * 1024);
            k = 0;
        }
    }

    delete[] Buffer;
    return NULL;
}

void *TCP_Ax_Routine(void *argu)
{

        
    int idx = 0;
    int cnt = 0;
 
    while (gQuit != true)
    {
       
        
        cout << "Socket Accepting in I/O Mode" << endl;
      
        struct sockaddr_in Client_Addr; 
        int len = sizeof(Client_Addr); 
        bzero(&Client_Addr,sizeof(Client_Addr)); 

        int connfd = accept(SocketFD, (struct sockaddr *)&Client_Addr, (socklen_t *)&len); 
        if (connfd == -1)
        {

            perror("Error : ");
        }
        else
        {

            char *Addr = inet_ntoa(Client_Addr.sin_addr);
            for( int i = 0 ; i < Server->getClientNumber() ;  i++)
            {

               if (  Server->Get_ConState(i) == false )
               {

                idx = i;
                break; 
                  

               }


            }            
                   
            
            Server->Set_Cli_Addr(idx) = Client_Addr;
            cout << "Client Acceptance : OK " << endl;

            




        
            Server->Set_EstFD(idx) = connfd; 
            Server->Set_ConState(idx) = true; 
            Server->Set_QueueNum()++; 

            cout << "Queue : " << Server->Get_QueueNum() << endl; 
            cout << "Client[" << idx << "]" << " : " << Addr;
            
            int temp = idx; 

            pthread_create(&TCP_Rx_TID[idx], NULL, TCP_Rx_Routine, (void *)&temp);
            pthread_detach(TCP_Rx_TID[idx]);
            
        }
    }
}

void *TCP_Rx_Routine(void *temp)
{

    
    char *Buffer = new char[1024];
    int idx = *(int *)temp; 
    int EstFD = Server->Get_EstFD(idx); 
    char* Addr = inet_ntoa(Server->Get_Cli_Addr(idx).sin_addr); 
       
    cout <<"\nUnder TCP Rx Routine[" << idx << "]" << endl; 
    cout <<"HostFD[" << EstFD << "]" << endl; 

    bzero(Buffer, 1024);
    while (gQuit != true)
    {
      
        int state = read(EstFD, Buffer, 1024);
        if ( (state != -1) &&  (state != 0))
        {

            cout << "From [ " << Addr << " ] : " << Buffer << endl;
           
        }
        else if (state == 0)
        {

            cout << "--- Client Close TCP Connection ---" << endl;
            break; 
        }
        else if (state == -1)
        {
            if (errno == ECONNRESET || errno == EPIPE)
            {

                cout << "--- Client Close TCP Connection ---" << endl;
                break; 
            }
            else
            {
                perror("Error : ");

            }
        }
    }


    int state = close(EstFD);
    if( state == -1 )
    perror("Error : "); 
    else
    {
        cout << "Close Host[" << idx << "] is Okay...!!!" << endl; 
        Server->Set_QueueNum()--; 
        Server->Set_ConState(idx) = false; 
        
    }
   
   
   delete[] Buffer;
    

}