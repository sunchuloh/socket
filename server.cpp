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

class Ethernet
{

private:
    
    int Binding_State;
    int Listening_State = -1;
    int Port; 
    int SocketFD;
    int* pConnectedFD; 
    int Client_Number;
    struct sockaddr_in Server_Addr; 
    struct sockaddr_in Client_Addr;
    struct sockaddr_in* pClient_Addr;

public:



    // 생성자
    Ethernet(int Port, int Number); 
    
    // 소멸자
    ~Ethernet();

    // This Pointer 
    Ethernet* GetThis(); 
    

    // Getter 
    struct sockaddr_in& GetServer_Addr(); // 구조체 반환
    struct sockaddr_in& GetClient_Addr(); // 구조체 반환
    struct sockaddr_in& GetClient_Addr(int Value); 

    
    int GetPort(); 
    int GetClient_Number();
    int GetListening_State();
    int GetBinding_State(); 
    int GetSocketFD();
    int GetConnectedFD(int Value); 
    


    // Setter

    void SetPort(int Value);
    void SetClient_Number(int Value);
    void SetListening_State(int Value); 
    void SetConnectedFD(int Value,int Idx);


    
    // 메서드
    void Transmit(char * Buffer);
    char Receive();
    void Receive(char *Buffer, int Size);

 


};

Ethernet :: Ethernet(int Port, int Value)
{

    int state;
    
    SetPort(Port);
    SetClient_Number(Value);
    

    pConnectedFD = new int[Value];
    pClient_Addr = new struct sockaddr_in[Value]; 
   
    bzero(pClient_Addr,Value);
    bzero(pConnectedFD, Value);
    bzero(&Server_Addr, sizeof(Server_Addr));
    bzero(&Client_Addr, sizeof(Client_Addr));

    Server_Addr.sin_family = AF_INET;
    Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Server_Addr.sin_port = htons(Port);

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD == -1)
    {
        cout << "Socket Creation : Error" << endl;
        exit(0);
    }
    else 
    {

        cout << "Socket Creation : OK" << endl;
        cout << "Socket File Descriptor : " << SocketFD << endl; 
        state = bind(SocketFD, (struct sockaddr *)&Server_Addr, sizeof(Server_Addr));
        if (state == 0)
        {
            cout << "Socket Binding : OK" << endl;
            Binding_State = state; 
            
           
        }
        else if (state != 0)
        {
            cout << "Socket Binding : Error" << endl;
            Binding_State = state; 
        }
    }
}

void Ethernet :: Transmit(char* Buffer) 
{


     int state = write(SocketFD,Buffer,sizeof(Buffer)); 
     if( state == -1 )  
     cout << "TCP Transmit : Error" <<endl; 
     
       

     

}

char  Ethernet :: Receive()
{
    
     char c;
     int state = read(SocketFD,&c,1); 
     if( state == -1 ) 
     cout << "TCP Receive : Error" << endl; 
     
      

     
}

void Ethernet :: Receive(char* Buffer, int size)
{

    int state = read(SocketFD,Buffer,size);
    if(state == -1) 
    cout << "TCP Receive : Error" << endl; 
    
      
   

}
Ethernet :: ~Ethernet()
{

     cout << "Destructing [ " << this << " ] : OK" << endl;
     close(SocketFD); 

}




// Setter 
void Ethernet :: SetPort(int Value)
{

this->Port = Value; 


}
void Ethernet :: SetClient_Number(int Value)
{


this->Client_Number = Value; 


}

void Ethernet :: SetListening_State(int Value)
{




this->Listening_State = Value; 


}

void Ethernet :: SetConnectedFD(int Value, int Idx)
{


// *((*this).(p_ConnectedFD+Idx))= Value; 

this->pConnectedFD[Idx] = Value; 



}




// Getter
Ethernet* Ethernet :: GetThis()
{

return this; 

}


int Ethernet :: GetBinding_State()
{



return this->Binding_State; 


}
int Ethernet :: GetListening_State()
{



return this->Listening_State;

}

int Ethernet :: GetClient_Number()
{


return this->Client_Number;

}



int Ethernet :: GetPort()
{


return this->Port;

}

int Ethernet :: GetSocketFD()
{


return this->SocketFD;


}

int Ethernet :: GetConnectedFD(int Idx)
{


this->pConnectedFD[Idx]; 


}




struct sockaddr_in& Ethernet ::GetServer_Addr() 
{
    /* struct sockaddr_in 구조체 Pointer Return */
    return Server_Addr;
}
struct sockaddr_in& Ethernet :: GetClient_Addr() 
{

    /* struct sockaddr_in 구조체 Pointer Return */
    return Client_Addr;
}

struct sockaddr_in& Ethernet :: GetClient_Addr(int idx)
{


return this->pClient_Addr[idx];


}




Ethernet *p_ethernet;


void *ETH_Rx_Task_Thread(void *argu);
void *KeyButton_Rx_Task_Thread(void *);
void *ETH_Accept_Task_Thread(void *argu);


int main(int argc, char *argv[])
{
    int Count = 0; 
    int Value;
    int Port; 

    pthread_t ETH_Accept_Task_Thread_TID;
    pthread_t ETH_Rx_Task_Thread_TID;
    pthread_t KeyButton_Rx_Task_Thread_TID;

    cout << "Enter a Port : ";
    cin >> Port;
    cout << "Enter the number of client : ";
    cin >> Value; 

    p_ethernet = new Ethernet(Port,Value); 
    

    // Port 
    cout << "p_ethernet->GetPort() : " << p_ethernet->GetPort() << endl; 
    // The Number of Client
    cout << "p_ethernet->GetClient_Number() : " << p_ethernet->GetClient_Number() << endl; 

    pthread_create(&ETH_Accept_Task_Thread_TID, NULL, &ETH_Accept_Task_Thread, NULL);  
    // pthread_create(&ETH_Rx_Task_Thread_TID, NULL, &ETH_Rx_Task_Thread, NULL);
    // pthread_create(&KeyButton_Rx_Task_Thread_TID, NULL, &KeyButton_Rx_Task_Thread, NULL);

    
    while (1)
    {

        cout << "main Thread While Loop" << Count++ <<endl;
        sleep(1); 


    }

    pthread_join(ETH_Accept_Task_Thread_TID, NULL);
   // pthread_join(ETH_Rx_Task_Thread_TID, NULL);
   // pthread_join(KeyButton_Rx_Task_Thread_TID, NULL);


    return 0;
}


void *KeyButton_Rx_Task_Thread(void *argu)
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
             p_ethernet->Transmit(Buffer);

            if (strcmp("quit\n", Buffer) == 0)
            {
                cout << "--- Exit Process ---" << endl;
                memset(Buffer, 0x0, sizeof(Buffer) * 1024);
                delete p_ethernet; 
                delete[] Buffer;

                exit(1);
            }
            memset(Buffer, 0x0, sizeof(Buffer) * 1024);
            idx = 0;
        }
    }

    return NULL;
}

void *ETH_Rx_Task_Thread(void *argu)
{

    cout << "---  ETH Receive Task Thread Entry Point ---" << endl;
    int idx = 0;
    char *Buffer = new char[1024];
    char c;

    memset(Buffer, 0x0, 1024);

    while (1)
    {

        c = p_ethernet->Receive();
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
                delete p_ethernet; 
                delete[] Buffer;

                exit(1);
            }

            bzero(Buffer, sizeof(Buffer));
            idx = 0;
        }
    }

    return argu;
}




void *ETH_Accept_Task_Thread(void *argu)
{

    int cnt = 0; 
    int idx = 0; 
    int _socketFD = p_ethernet->GetSocketFD(); 
    int _client_Number = p_ethernet->GetClient_Number(); 
    int len = sizeof(p_ethernet->GetClient_Addr()); 
    


    
     if( p_ethernet->GetBinding_State() == 0 )
     {

        // OK Binding.

        int state = listen(_socketFD, _client_Number);
        if( state == -1 )
        {   
            
            // error 
            cout << "Socket Listineing : Error" << endl; 
            p_ethernet->SetListening_State(state); 
            


        }
        else if ( state == 0 )
        {

           // ok 

           cout <<"Socket Listening : OK" << endl; 
           p_ethernet->SetListening_State(state); 


        }
       

     }
  

  

    while (1)
    {
        

        cout << "Socket Accepting in I/O Mode" << endl; 
        int _ConnectedFD = accept(_socketFD, (struct sockaddr *)&(p_ethernet->GetClient_Addr(idx)), (socklen_t *)&len); // Enter I/O Mode

        if ( _ConnectedFD == -1)
        {

            cout << "Socket Accepting : Error" << endl; 
        }
        else
        {

            cout << "Socket Accepting : OK" << endl;
            cout << "p_ethernet->GetConnectedFD(" << idx << ") : " << p_ethernet->GetConnectedFD(idx) << endl; 

            cout << "inet_ntoa(p_ethernet->GetClient_Addr(idx).sin_addr) : " << inet_ntoa(p_ethernet->GetClient_Addr(idx).sin_addr) << endl; 
            
            p_ethernet->SetConnectedFD(_ConnectedFD,idx++);
           
            

            
           
        }
   
    }


}
