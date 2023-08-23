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
    int Port_Number;
    int Guest_Number;
    int Bind_State; 
    int SocketFD; 

    

public:
    
    /* --- Constructor --- */ 
 
    Socket(const struct sockaddr_in& s, const int& n);

    /* --- Destructor --- */
    ~Socket();

    /* --- Setter --- */

    int Set_Listen();

    void Set_Binding_Flag(int Flag);
    void Set_Listening_Flag(int Flag);
   

    /* --- Getter --- */
    int Get_SocketFD(); 
    int Get_GuestNum(); 
    int Get_Binding_Flag();
    int Get_Listeing_Flag(); 

    


};


Socket :: Socket(const struct sockaddr_in& s,const int& n)
{

    
    Guest_Number = n;
    bzero(&Server_Addr,sizeof(Server_Addr));
    
    Server_Addr.sin_family = s.sin_family;
    Server_Addr.sin_addr.s_addr = s.sin_addr.s_addr;
    Server_Addr.sin_port = s.sin_port;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
 
    if ( SocketFD == -1 )  
    {
        printf("--- Socket Creation : Fail ---\n");
        exit(0);
    }
    else
    {

        printf("--- Socket Creation : OK ---\n");
        printf("--- Socket File Descriptor : %d\n",SocketFD);
      
        int Value = bind(SocketFD, (struct sockaddr *)&Server_Addr, sizeof(Server_Addr));

        if ( Value == BIND_OK )
        {
            printf("--- Socket Binding : OK ---\n");
            Bind_State = Value;
                       
        }
        else if ( Value != BIND_OK)
        {
            printf("--- Socket Binding : Fail ---\n");
            Bind_State = Value; 
            exit(0);
        }
    }
}

Socket ::~Socket()
{

printf("--- Destructing Socket Instance [%d] : OK ---\n",this);


}

/* Setter */


int Socket :: Set_Listen()
{

int state; 
if( Bind_State == BIND_OK )
state = listen(SocketFD,Guest_Number);
return state;

}




/* ---  Getter --- */

int Socket :: Get_SocketFD()
{

    return SocketFD;
}
int Socket :: Get_GuestNum()
{

return Guest_Number; 

}



/* Host Class */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Host Class*/

class Host
{

private:
    
    

    
    struct sockaddr_in Guest_Addr;

    int Binding_Flag;   
    int Listening_Flag;

    int Port_Number;
    int Guest_Number;

    int HostFD; 

public:

    /* --- Default Constructor ---*/
    Host() {}

    /* --- Destructor --- */ 
    ~Host();

        

    /* --- Getter ---  */

    Host *Get_ThisPointer(); 
    Host &Get_Instance();     

   
    struct sockaddr_in &Get_GuestAddr();

    int& Get_HostFD(); 

    int Get_Port_Number();
    int Get_GuestNum();
    int Get_Listening_Flag();
    int Get_Binding_Flag();
    int Get_Established_File_Descriptor();

    /* --- Setter ---  */
    void Set_GuestAddr(const sockaddr_in& ); 

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


    /* --- OVerloaDinG --- */

  

};




/* --- Generic Method --- */
void Host :: Transmit(char *Buffer)
{

    int state = write(HostFD, Buffer, sizeof(Buffer));
    if (state == -1)
        cout << "TCP Transmit : Error" << endl;
}

void Host ::Transmit(char *Buffer, int File_Descriptor)
{

    int state = write(File_Descriptor, Buffer, sizeof(Buffer));
    if (state == -1)
        cout << "TCP Transmit : Error" << endl;
}

char Host ::Receive()
{

    char c;
    int state = read(HostFD, &c, 1);
    if (state == -1)
        cout << "TCP Receive : Error" << endl;
}

void Host ::Receive(char *Buffer, int size)
{

    int state = read(HostFD, Buffer, size);
    if (state == -1)
        cout << "TCP Receive : Error" << endl;
}

/* --- Destructor --- */
Host ::~Host()
{

    cout << "Destructing [ " << this << " ] : OK" << endl;
    close(HostFD);
}

/* --- Setter ---*/
void Set_GuestAddr(const sockaddr_in& s)
{

  

}

/* --- Getter --- */
Host *Host ::Get_ThisPointer()
{

    return this;
}

Host &Host ::Get_Instance()
{

    return *this;
}


struct sockaddr_in &Host ::Get_GuestAddr()
{

   
    return Guest_Addr;
}


int& Host :: Get_HostFD()
{


return this->HostFD; 

}


int Host ::Get_Binding_Flag()
{

    return Binding_Flag;
}
int Host::Get_Listening_Flag()
{

    return Listening_Flag;
}

int Host ::Get_GuestNum()
{

    return Guest_Number;
}


/* --- OveRLoadInG ---*/





/* Global */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Global */

pthread_t Host_Accept_Task_Thread_TID;
pthread_t Host_Rx_Task_Thread_TID;
pthread_t KeyButton_Receive_Task_Thread_TID;


Host *pHost;
Socket *pSocket;

void *Host_Rx_Task_Thread(void *argu);
void *KeyButton_Rx_Task_Thread(void *);
void *Host_Accept_Task_Thread(void *argu);




/* main */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* main */


int main(int argc, char *argv[])
{

    
    struct sockaddr_in Server_Addr;

    int Count = 0;
    int Guest_Num;
    int Port_Num;;

   
    cout << "Enter a Port : ";
    cin >> Port_Num;
    cout << "Enter the number of Guest : ";
    cin >> Guest_Num;

    
   bzero(&Server_Addr, sizeof(Server_Addr));
   Server_Addr.sin_family = AF_INET;
   Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
   Server_Addr.sin_port = htons(Port_Num);
    

    pSocket = new Socket(Server_Addr,Guest_Num); 
    if( pSocket->Set_Listen() == LISTEN_OK )
    {
      printf("--- Socket Listening : OK ---\n"); 

    }
    else 
    {

        printf("--- Socket Listening : Fail ---\n");
        exit(0); 

    }

    
    pHost = new Host[Guest_Num];
    

    

   

    pthread_create(&Host_Accept_Task_Thread_TID, NULL, &Host_Accept_Task_Thread, NULL);
    // pthread_create(&ETH_Rx_Task_Thread_TID, NULL, &ETH_Rx_Task_Thread, NULL);
    // pthread_create(&KeyButton_Rx_Task_Thread_TID, NULL, &KeyButton_Rx_Task_Thread, NULL);

    while (1)
    {

        cout << "main Thread While Loop" << Count++ << endl;
        sleep(1);
    }

    pthread_join(Host_Accept_Task_Thread_TID, NULL);
    // pthread_join(ETH_Rx_Task_Thread_TID, NULL);
    // pthread_join(KeyButton_Rx_Task_Thread_TID, NULL);

    return 0;
}
void *Host_Accept_Task_Thread(void *argu)
{
    
    
    
    
    int cnt = 0;
    int idx = 0; 

    int SocketFD = pSocket->Get_SocketFD();
    int Cli_Num = pSocket->Get_GuestNum();
    
     


    while (1)
    {
        
        struct sockaddr_in Guest_Addr;
        bzero(&Guest_Addr,sizeof(Guest_Addr)); 
        int len = sizeof(Guest_Addr);        
        int Value = accept(pSocket->Get_SocketFD(), (struct sockaddr *)&Guest_Addr, (socklen_t *)&len);
        pHost[idx].Get_GuestAddr() = Guest_Addr;

        
        if ( Value ==  ACCEPT_ERROR )
        {

            printf("--- Socket Accept : Fail ---\n");
        }
        else if ( Value != ACCEPT_ERROR )
        {
            
            pHost[idx].Get_HostFD() = Value; 
           
            printf("--- Socket Accept : OK ---\n");
           
           
            printf("pHost[%d].Get_HostFD() : %d\n",idx,pHost[idx].Get_HostFD());
                                         
            cout << "inet_ntoa(pETH->GetClientAddr(idx).sin_addr) : " << inet_ntoa(pHost[idx].Get_GuestAddr().sin_addr) << endl;
            idx++;
            
        }
    }
}
/*
void *KeyButton_Receive_Task_Thread(void *argu)
{

    cout << "---  KeyButton Receive Task Thread Entry Point ---" << endl;

    int idx = 0;
    char c;
    char *Buffer = new char[1024];
    memset(Buffer, 0x0, sizeof(char) * 1024);

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

            cout << "Console : " << Buffer;
            pETH->Transmit(Buffer);

            if (strcmp("quit\n", Buffer) == 0)
            {
                cout << "--- Exit Process ---" << endl;
                memset(Buffer, 0x0, sizeof(Buffer) * 1024);
                delete pETH;
                delete[] Buffer;

                exit(1);
            }
            memset(Buffer, 0x0, sizeof(Buffer) * 1024);
            idx = 0;
        }
    }

    return NULL;
}

void *Ethernet_Receive_Task_Thread(void *argu)
{

    cout << "---  ETH Receive Task Thread Entry Point ---" << endl;
    int idx = 0;
    char *Buffer = new char[1024];
    char c;

    memset(Buffer, 0x0, 1024);

    while (1)
    {

        c = pETH->Receive();
        Buffer[idx++] = c;

        if (c == '\b')
        {
            putchar(' ');
        }
        else if (c == '\n')
        {

            cout << "From Vim : " << Buffer;

            if (strcmp("quit\n", Buffer) == 0)
            {
                cout << "--- Exit Process ---" << endl;

                memset(Buffer, 0x0, sizeof(Buffer) * 1024);
                delete pETH;
                delete[] Buffer;

                exit(1);
            }

            bzero(Buffer, sizeof(Buffer));
            idx = 0;
        }
    }

    return argu;
}
*/