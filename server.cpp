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

#define SOCKET_CREATION_ERROR -1
#define BIND_OK 0
#define LISTEN_OK 0
#define LISTEN_ERROR -1
#define SOCKET_ACCEPT_ERROR -1
#define WRITE_ERROR -1
#define WRITE_EOF 0
#define READ_ERROR -1
#define READ_EOF 0
#define TCP_TRANSMIT_ERROR -1
#define TCP_TRANSMIT_OK 0

#define TCP_RECEIVE_ERROR -1
#define TCP_RECEIVE_OK 0

using namespace std;

class Ethernet
{

private:
   
   
    struct sockaddr_in *pGuest_Addr;
    int *pHostFD;

    bool Bind_Flag = false;
    bool Listen_Flag = false;
    int Binding_State;
    int Listening_State = -1;

    int SocketFD;

    int Port_Num;
    int Guest_Num;
    int Est_Num = 0;
    struct sockaddr_in Host_Addr;

public:
    /* --- Constructor --- */
    Ethernet(const int &_Port_Num, const int &_Cli_Num);
    Ethernet(const sockaddr_in &s);

    /* --- Destructor --- */
    ~Ethernet();

    // This Pointer
    Ethernet *GetThis();
    Ethernet &GetInstace();

    /* ---Getter --- */

    int GetHostFD(int Idx) const;

    struct sockaddr_in &GetHostAddr();
    struct sockaddr_in &GetGuestAddr(int Idx);

    int GetPortNum();
    int GetGuestNum();
    int &GetListening_State();
    int &GetBinding_State();
    int GetSocketFD();

    int GetEstNum();

    bool GetBindFlag();
    bool GetListenFlag();

    int GetIndexPointer();

    /* --- Setter --- */

    struct sockaddr_in &SetGuestAddr(int idx);
    struct sockaddr_in &SetGuestAddr(const sockaddr_in &s, int idx);

    int &SetEstNum();
    int SetListen();

    int &SetHostFD(int Idx);

    void SetHostFD(int _HostFD, int Idx);

    void SetEstNum(const int &Number);

    void SetPortNum(int Value);
    void SetBindFlag(bool Flag);

   
    void SetListening_State(int Value);

    /* --- Generic Method */
    void CountEstNum(); 
    int Transmit(char *Buffer, int Idx);
    char Receive(int Idx);
    void Receive(char *Buffer, int Size);
    int Receive(int idx, char *Buffer, int size);
    int Receive(int idx, char &c);

};

/* --- Constructor --- */
Ethernet ::Ethernet(const int & _Port_Num, const int &_Guest_Num)
{
    

    Guest_Num = _Guest_Num;
    Port_Num = _Port_Num;

    cout << "--- Creating Ethernet Instance ---" << endl;
    cout << "--- Guest Number : " << Guest_Num << " --- " << endl;
    cout << "--- Port Number : " << Port_Num << " --- " << endl;

   

    pHostFD = new int[Guest_Num];
    pGuest_Addr = new struct sockaddr_in[Guest_Num];

    bzero(pHostFD, sizeof(int) * Guest_Num);
    bzero(pGuest_Addr, sizeof(struct sockaddr_in) * Guest_Num);
    bzero(&Host_Addr, sizeof(Host_Addr));

    Host_Addr.sin_family = AF_INET;
    Host_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Host_Addr.sin_port = htons(Port_Num);

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD == SOCKET_CREATION_ERROR)
    {
        cout << "--- Socket Creation : Error ---" << endl;
        exit(0);
    }
    else
    {

        cout << "--- Socket Creation : OK ---" << endl;
        cout << "--- Socket File Descriptor : " << SocketFD << " ---" << endl;
        int state = bind(SocketFD, (struct sockaddr *)&Host_Addr, sizeof(Host_Addr));
        if (state == BIND_OK)
        {
            int state;
            int Temp_SocketFD = SocketFD;
            cout << "--- Socket Binding : OK ---" << endl;
            Bind_Flag = true;
            SetBindFlag(true);

            state = listen(Temp_SocketFD, Guest_Num);
            if (state == LISTEN_OK)
            {

                cout << "--- Socket Listening : OK ---" << endl;
                Listen_Flag = true;
            }
            else if (state == LISTEN_ERROR)
            {

                cout << "--- Socket Listening : ERROR ---" << endl;
                Listen_Flag = false;
            }
        }
        else if (state != BIND_OK)
        {
            cout << "--- Socket Binding : Error ---" << endl;
            Bind_Flag = false;
            SetBindFlag(false);
        }
    }
}

/* --- Generic Method --- */
int Ethernet ::Transmit(char *Buffer, int idx)
{
    int HostFD;
    int state;
    if (idx > Est_Num)
    {

        return TCP_TRANSMIT_ERROR;
    }
    else
    {

        HostFD = pHostFD[idx];
        state = write(HostFD, Buffer, sizeof(Buffer));
        if ((state != WRITE_ERROR) && (state != WRITE_EOF))
        {

            return TCP_TRANSMIT_OK;
        }
        else
        {

            return TCP_TRANSMIT_ERROR;
        }
    }
}

int Ethernet ::Receive(int idx, char &c)
{

    int HostFD;
    int state;
    
    if (idx > Est_Num)
    {
        return TCP_RECEIVE_ERROR;
    }
    else
    {

        HostFD = pHostFD[idx];
        state = read(HostFD, &c, 1);
        if ((state != READ_ERROR) && (state != READ_EOF))
        {

            return TCP_RECEIVE_OK;
        }
        else
        {
            return TCP_RECEIVE_ERROR;
        }
    }
}

char Ethernet ::Receive(int Idx)
{

    int HostFD;
    int state;
    char c;
    if (Idx > Est_Num)
    {
    }
    else
    {

        HostFD = pHostFD[Idx];
        state = read(HostFD, &c, 1);
        if ((state != READ_ERROR) && (state != READ_EOF))
        {

            return c;
        }
        else
        {
        }
    }
}

int Ethernet :: Receive(int idx, char *Buffer, int size)
{
    
        int state;
        state = read(pHostFD[idx], Buffer, size);
        return state; 

        

        
}



void Ethernet ::Receive(char *Buffer, int size)
{

    int state = read(SocketFD, Buffer, size);
    if (state == -1)
        cout << "TCP Receive : Error" << endl;
}

void Ethernet :: CountEstNum()
{


Est_Num++; 

}



/* --- Setter --- */

struct sockaddr_in &Ethernet ::SetGuestAddr(int idx)
{

    return pGuest_Addr[idx];
}
struct sockaddr_in &Ethernet ::SetGuestAddr(const sockaddr_in &s, int idx)
{

    pGuest_Addr[idx] = s;
}

int &Ethernet ::SetEstNum()
{

    return Est_Num;
}

void Ethernet ::SetBindFlag(bool Flag)
{

    Bind_Flag = Flag;
}

int Ethernet ::SetListen()
{

    int state;

    if (Listen_Flag == false)
    {

        state = listen(SocketFD, Guest_Num);

        return state;
    }
    else
    {

        return LISTEN_OK;
    }
}

int &Ethernet ::SetHostFD(int Idx)
{

    return pHostFD[Idx];
}

void Ethernet ::SetHostFD(int _HostFD, int Idx)
{

    pHostFD[Idx] = _HostFD;
}

void Ethernet ::SetEstNum(const int &Number)
{

    Est_Num = Number;
}

void Ethernet ::SetListening_State(int Value)
{

    this->Listening_State = Value;
}

/* --- Getter --- */

int Ethernet ::GetHostFD(int Idx) const
{

    return pHostFD[Idx];
}

struct sockaddr_in &Ethernet ::GetHostAddr()
{

    return Host_Addr;
}

struct sockaddr_in& Ethernet ::GetGuestAddr(int Idx)
{

    return pGuest_Addr[Idx];
}

bool Ethernet ::GetListenFlag()
{

    return Listen_Flag;
}

int Ethernet::GetEstNum()
{

    return Est_Num;
}

Ethernet *Ethernet ::GetThis()
{

    return this;
}

int Ethernet ::GetGuestNum()
{

    return Guest_Num;
}

int Ethernet ::GetPortNum()
{

    return Port_Num;
}

int Ethernet ::GetSocketFD()
{

    return SocketFD;
}

bool Ethernet ::GetBindFlag()
{

    return Bind_Flag;
}


/* --- Destructor --- */
Ethernet ::~Ethernet()
{
   
    cout << "Destructing Ethernet Instance of " << this << " is Done " << endl; 
    cout << "The Number of Channel is " << Est_Num << endl; 

    for ( int i = 0 ; i < Est_Num ; i++ )
    cout << "Guest Address on index of " << i << " is " << inet_ntoa(pGuest_Addr[i].sin_addr); 
           
    for ( int i = 0 ; i < Est_Num ; i++ )
    {
    close(pHostFD[i]); 
    cout << "Close Active Channel to Guest on index of " << i << endl; 
    
    }

    cout <<"Close Socket" << endl; 
    close(SocketFD);
    
    delete [] pHostFD;
    delete [] pGuest_Addr; 
  
    

}

Ethernet *pETH; 
// pthread_t *pKeyButton_Rx_Task_Thread_TID;
pthread_t *pETH_Rx_Task_Thread_TID;

void *ETH_Rx_Task_Thread(void *argu);
void *KeyButton_Rx_Task_Thread(void *);
void *ETH_Accept_Task_Thread(void *argu);

int main(int argc, char *argv[])
{

    pthread_t ETH_Accept_Task_Thread_TID;
    pthread_t ETH_Rx_Task_Thread_TID;
    pthread_t KeyButton_Rx_Task_Thread_TID;

    int Count = 0;
    int Guest_Num;
    int Port_Num;

    cout << "Enter a Port : ";
    cin >> Port_Num;
    cout << "Enter the number of client : ";
    cin >> Guest_Num;

    pETH = new Ethernet(Port_Num, Guest_Num);
    // pKeyButton_Rx_Task_Thread_TID = new pthread_t[Guest_Num];
    pETH_Rx_Task_Thread_TID = new pthread_t[Guest_Num];

    printf("ptr_ether->Get_GuestNum() : %d\n", pETH->GetGuestNum());
    printf("ptr_ether->Get_PortNum() : %d\n", pETH->GetPortNum());

    pthread_create(&ETH_Accept_Task_Thread_TID, NULL, &ETH_Accept_Task_Thread, NULL);
    pthread_create(&KeyButton_Rx_Task_Thread_TID, NULL, &KeyButton_Rx_Task_Thread, NULL);
   

    while (1)
    {
        
        //cout << "main Thread While Loop" << Count++ << endl;
        // sleep(1);
    }

    pthread_join(ETH_Accept_Task_Thread_TID, NULL);
    // pthread_join(ETH_Rx_Task_Thread_TID, NULL);
    pthread_join(KeyButton_Rx_Task_Thread_TID, NULL);

    return 0;
}

void *KeyButton_Rx_Task_Thread(void *argu)
{

    cout << "Under KeyButton Rx Task Thread Entry Point" << endl;

    // int idx = *(int *)argu;
    int k = 0;
    char c;
    char *Buffer = new char[1024];
    memset(Buffer, 0x0, sizeof(char) * 1024);

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
                cout << "--- Exit Process ---" << endl;
                memset(Buffer, 0x0, sizeof(Buffer) * 1024);
                delete [] pETH; 
                delete [] pETH_Rx_Task_Thread_TID; 
                delete[] Buffer;

                exit(1);
            }
            memset(Buffer, 0x0, sizeof(Buffer) * 1024);
            k = 0;
        }
    }

    return NULL;
}

void *ETH_Rx_Task_Thread(void *argu)
{
    
    char Buffer[1024];
    int index = *(int *)argu;
    int k = 0; 
    char c;
    int fd = pETH->GetHostFD(index); 
    cout << "Under TCP Rx Task Thread on index of " << index << endl; 
    cout << "File Descriptor : " << fd << endl; 
    int state; 
       
    bzero(Buffer,1024);
    while (1)
    {
        
        
      
        state = read(fd,Buffer,1024); 
        if( state != -1 && state != 0 )
        {

          cout << "From Guest : " << Buffer <<endl;
          bzero(Buffer,1024); 
        }
     
      
      
                  
                   

    }
    

    return argu;
}

void *ETH_Accept_Task_Thread(void *argu)
{



     struct sockaddr_in Guest_Addr;
     int len = sizeof(Guest_Addr); 
     int HostFD;
     int index = 0; 
     int cnt = 0;
     int k = 0;
  

   
  

    while (1)
    {   
        cout << "Socket Accepting in I/O Mode" << endl;
        int len = sizeof(pETH->GetGuestAddr(index)); 
              
       // int Value = accept(pETH->GetSocketFD(), (struct sockaddr *)&pETH->GetGuestAddr(k), (socklen_t *)&len);
        int NewFD = accept(pETH->GetSocketFD(), (struct sockaddr *)&pETH->GetGuestAddr(index), (socklen_t *)&len);
        if ( NewFD == SOCKET_ACCEPT_ERROR)
        {

            cout << "Socket Accepting : Error" << endl;
        }
        else
        {

            

            cout << "Socket Accepting : OK" << endl;


           // pETH->SetHostFD(index,NewFD);
            pETH->SetHostFD(index) = NewFD; 
            pETH->CountEstNum(); 
            
            cout << "Guest Address to index of "<<  index <<  " is " << inet_ntoa(pETH->GetGuestAddr(index).sin_addr) << endl; 
            cout << "The Number of Guest in Queue is " << pETH->GetEstNum() << endl; 
            cout << "pETH->GetHostFD(index) : " << pETH->GetHostFD(index) << endl; 

            int temp = index; 
            pthread_create(&pETH_Rx_Task_Thread_TID[index], NULL, &ETH_Rx_Task_Thread, (void*)&temp);
            index++;
       
              
                    
            
            
           
        }
    }
}
