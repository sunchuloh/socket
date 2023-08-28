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
    int Queue_Size;  
    int Bind_State; 
    int SocketFD; 
    int Session_Number = 0; 


    

public:
    
    /* --- Constructor --- */ 
 
    Socket(const struct sockaddr_in& s, const int& n);

    /* --- Destructor --- */
    ~Socket();

    /* --- Setter --- */

    int& Set_SessionNum();
    

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
Socket :: Socket(const struct sockaddr_in& s,const int& n)
{

    
    Queue_Size = n; 
    bzero(&Server_Addr,sizeof(Server_Addr));
    
    Server_Addr.sin_family = s.sin_family;
    Server_Addr.sin_addr.s_addr = s.sin_addr.s_addr;
    Server_Addr.sin_port = s.sin_port;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
 
    if ( SocketFD == -1 )  
    {
        printf("--- Socket Creation : Fail ---\n");
        perror("Error : "); 
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
        else if ( Value != BIND_OK )
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

printf("--- Destructing Socket Instance [%d] : OK ---\n",this);


}

/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Setter */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

int& Socket :: Set_SessionNum()
{

return Session_Number; 

}


int Socket :: Set_Listen()
{

int state; 
if( Bind_State == BIND_OK )
state = listen(SocketFD,Queue_Size);
return state;

}



/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* Getter */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

int Socket :: Get_SocketFD()
{

    return SocketFD;
}
int Socket :: Get_QueueSize()
{

return Queue_Size; 

}

int Socket :: Get_SessionNum()
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

  
    int Client_Number = 0; 

    int HostFD; 

public:

    /* --- Default Constructor ---*/
    Host() {}

    /* --- Destructor --- */ 
    ~Host();

        

    /* --- Getter ---  */
    
    Host *Get_ThisPointer(); 
    Host &Get_Instance();     

   
    struct sockaddr_in Get_ClientAddr();

    int& Get_HostFD();   
    int Get_ClientNum();
    int Get_Listening_Flag();
    int Get_Binding_Flag();
    int Get_Established_File_Descriptor();


    /* --- Setter ---  */

 
    void Set_ClientAddr(struct sockaddr_in& s);
    struct sockaddr_in& Set_ClientAddr(); 
    void Set_ClientAddr(const sockaddr_in& ); 
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
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Default Constructor  --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

Host :: Host()
{

bzero(&Client_Addr,sizeof(Client_Addr)); 


}







/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Setter --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
void Host :: Set_ClientAddr(const struct sockaddr_in& s)
{

  Client_Addr = s; 


}

struct sockaddr_in& Host :: Set_ClientAddr()
{


return Client_Addr; 


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

int Host ::Get_ClientNum()
{

    return Client_Number;
}


/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Generic Method --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/
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
/* ------------------------------------------------------------------------------------------------------------------------------------*/
/* --- Destructor --- */
/* ------------------------------------------------------------------------------------------------------------------------------------*/

Host ::~Host()
{

    cout << "Destructing [ " << this << " ] : OK" << endl;
    close(HostFD);
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
pthread_t Server_Rx_TID;
pthread_t Key_Rx_TID; 


Socket *pSocket;
Host *pHost;


void *Server_Ax_Thread(void *argu);
void *Key_Rx_Thread(void *);
void *Server_Rx_Thread(void *argu);




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

   
    printf("Enter Port Number : "); scanf("%d",&Port_Num);
    printf("Ennter Client Queue Size : "); scanf("%d",&Queue_Size);
    

    
   bzero(&Server_Addr, sizeof(Server_Addr));
   Server_Addr.sin_family = AF_INET;
   Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
   Server_Addr.sin_port = htons(Port_Num);
   
    pSocket = new Socket(Server_Addr,Queue_Size);  
    
    if( pSocket->Set_Listen() == LISTEN_OK )
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
    
    

   

    if( pthread_create(&Server_Ax_TID, NULL, Server_Ax_Thread, NULL) != 0) perror("Error : "); 
   
    

    while (1)
    {

        cout << "main Thread While Loop" << Count++ << endl;
        sleep(1);
    }

    

    return 0;
}
void *Server_Ax_Thread(void *argu)
{
    
    
    int SocketFD = pSocket->Get_SocketFD();
    
    
    int cnt = 0;
    int idx = 0; 

    
    
    
     


    while (1)
    {
        
        struct sockaddr_in Client_Addr;
        bzero(&Client_Addr,sizeof(Client_Addr)); 
        int len = sizeof(Client_Addr);        
        int FD = accept(pSocket->Get_SocketFD(), (struct sockaddr *)&Client_Addr, (socklen_t *)&len);
        pHost[idx].Set_ClientAddr() = Client_Addr; 
        
        if ( FD ==  ACCEPT_ERROR )
        {

            printf("--- Socket Accept : Fail ---\n");
            perror("Error : ");
        }
        else if ( FD != ACCEPT_ERROR )
        {
            pHost[idx].Set_Session(true); 
            pHost[idx].Set_SessionFD() = FD; 
            pSocket.Set_SessionNum()++;
          
           
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