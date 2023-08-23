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

using namespace std;




class Ethernet
{

private:
    

      struct sockaddr_in* ptr_Addr_Guest;
      int* ptr_HostFD; 

    bool Bind_Flag = false; 
    bool Listen_Flag = false; 
    int Binding_State;
    int Listening_State = -1;
 
    int SocketFD;

    int Port_Num;  
    int Guest_Num;
    int Est_Num = 0; 
    struct sockaddr_in Addr_Host; 
   
  

public:



    /* --- Constructor --- */
    Ethernet(const int& _Port_Num, const int& _Cli_Num); 
    Ethernet(const sockaddr_in& s); 
    
    /* --- Destructor --- */
    ~Ethernet();

    // This Pointer 
    Ethernet* Get_This(); 
    Ethernet& Get_Instace(); 
    

    /* ---Getter --- */  
    struct sockaddr_in& Get_AddrHost(); 
    struct sockaddr_in Get_AddrGuest(int Idx); 

    
    int Get_PortNum(); 
    int Get_GuestNum();
    int& Get_Listening_State();
    int& Get_Binding_State(); 
    int Get_SocketFD();
    int Get_HostFD(int Idx); 
    int Get_EstNum();

    bool Get_BindFlag(); 
    bool Get_ListenFlag(); 
    


    /* --- Setter --- */ 


   struct sockaddr_in& Set_AddrGuest(int idx); 
   struct sockaddr_in& Set_AddrGuest(const sockaddr_in& s, int idx); 
   
    int& Set_EstNum();
    int Set_Listen(); 

    int& Set_HostFD(int Idx);

    void Set_HostFD(int _HostFD, int Idx); 

    void Set_EstNum(const int& Number); 
    
   
    void Set_PortNum(int Value);
    void Set_BindFlag(bool Flag); 
    


    
    void SetClient_Number(int Value);
    void SetListening_State(int Value); 
    

    
    /* --- Generic Method */
    void Transmit(char * Buffer);
    char Receive();
    void Receive(char *Buffer, int Size);

 




};



/* --- Constructor --- */
Ethernet :: Ethernet(const int& _Port_Num, const int& _Guest_Num)
{
    
    cout << "--- Creation Ethernet Instance ---" << endl;
    cout << "--- Guest Number : " << _Guest_Num << " --- "<< endl; 
    cout << "--- Port Number : " << _Port_Num << " --- " << endl;
   
   
    
    Guest_Num = _Guest_Num;
    Port_Num = _Port_Num; 
         
    ptr_HostFD = new int[_Guest_Num];
    ptr_Addr_Guest = new struct sockaddr_in[_Guest_Num]; 
   
    bzero(ptr_HostFD, sizeof(int)*_Guest_Num);
    bzero(ptr_Addr_Guest,sizeof(struct sockaddr_in)*_Guest_Num);
    bzero(&Addr_Host, sizeof(Addr_Host));
  

    Addr_Host.sin_family = AF_INET;
    Addr_Host.sin_addr.s_addr = htonl(INADDR_ANY);
    Addr_Host.sin_port = htons(Port_Num);

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD == SOCKET_CREATION_ERROR )
    {
        cout << "--- Socket Creation : Error ---" << endl;
        exit(0);
    }
    else 
    {

        cout << "--- Socket Creation : OK ---" << endl;
        cout << "--- Socket File Descriptor : " << SocketFD << " ---" << endl; 
        int state = bind(SocketFD, (struct sockaddr *)&Addr_Host, sizeof(Addr_Host));
        if (state == BIND_OK)
        {
            int state; 
            int Temp_SocketFD = SocketFD;
            cout << "--- Socket Binding : OK ---" << endl;
            Bind_Flag = true;
            Set_BindFlag(true);  

            

            state = listen(Temp_SocketFD,Guest_Num); 
            if( state == LISTEN_OK)
            {

                cout << "--- Socket Listening : OK ---" << endl; 
                Listen_Flag = true;

            }
            else if ( state == LISTEN_ERROR )
            {

                 cout << "--- Socket Listening : ERROR ---" << endl; 
                 Listen_Flag = false; 

            }
            
            
           
        }
        else if (state != BIND_OK)
        {
            cout << "--- Socket Binding : Error ---" << endl;
            Bind_Flag = false; 
            Set_BindFlag(false); 
        }

    }
}


/* --- Generic Method --- */
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

/* --- Destructor --- */
Ethernet :: ~Ethernet()
{

     cout << "Destructing [ " << this << " ] : OK" << endl;
     close(SocketFD); 

}




/* --- Setter --- */

struct sockaddr_in& Ethernet ::  Set_AddrGuest(int idx)
{


return ptr_Addr_Guest[idx]; 


}
struct sockaddr_in& Ethernet :: Set_AddrGuest(const sockaddr_in& s, int idx)
{


ptr_Addr_Guest[idx] = s; 


}
   
int& Ethernet ::  Set_EstNum()
{

return Est_Num;


}


void Ethernet :: Set_BindFlag(bool Flag)
{

Bind_Flag = Flag; 
  

}


int Ethernet :: Set_Listen()
{

int state; 

if( Listen_Flag == false)
{


state = listen(SocketFD,Guest_Num); 

return state;

}
else 
{

return LISTEN_OK; 

}



}

int& Ethernet :: Set_HostFD(int Idx)
{

    return ptr_HostFD[Idx]; 

}

void Ethernet :: Set_HostFD(int _HostFD,int Idx)
{


ptr_HostFD[Idx] = _HostFD; 


}


void Ethernet :: Set_EstNum(const int& Number)
{

Est_Num = Number;

}




void Ethernet :: SetListening_State(int Value)
{




this->Listening_State = Value; 


}





/* --- Getter --- */

struct sockaddr_in& Ethernet :: Get_AddrHost() 
{
   
    return Addr_Host;
}


struct sockaddr_in Ethernet :: Get_AddrGuest(int Idx)
{


return ptr_Addr_Guest[Idx]; 


}

bool Ethernet :: Get_ListenFlag()
{

return Listen_Flag; 


}

int Ethernet :: Get_HostFD(int Idx)
{



return ptr_HostFD[Idx]; 


}

int Ethernet::Get_EstNum()
{

return Est_Num; 

}

Ethernet* Ethernet :: Get_This()
{

return this; 

}




int Ethernet :: Get_GuestNum()
{


return Guest_Num;


}




int Ethernet :: Get_PortNum()
{


return Port_Num;

}

int Ethernet :: Get_SocketFD()
{


return SocketFD; 


}


bool Ethernet :: Get_BindFlag()
{


return Bind_Flag; 

}







Ethernet *ptr_ether; 


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

  ptr_ether = new Ethernet(Port_Num,Guest_Num); 

   printf("ptr_ether->Get_GuestNum() : %d\n",ptr_ether->Get_GuestNum());
   printf("ptr_ether->Get_PortNum() : %d\n",  ptr_ether->Get_PortNum()); 
   
  
    

   
    
    
   

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


/*
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

*/ 



void *ETH_Accept_Task_Thread(void *argu)
{
    struct sockaddr_in Addr_Guest;
    int HostFD;  
    int len = sizeof(Addr_Guest); 
    int cnt = 0; 
    int idx = 0; 
    int socketFD = ptr_ether->Get_SocketFD(); 
    int Guest_Number = ptr_ether->Get_GuestNum(); 
    int Est_Num = ptr_ether->Get_EstNum(); 
    bool Bind_Flag = ptr_ether->Get_BindFlag(); 
    char *Address; 
  
           

    while (1)
    {
         
        cout << "--- Socket Accepting in I/O Mode ---" << endl; 
        bzero(&Addr_Guest,sizeof(Addr_Guest)); 
        HostFD = accept(socketFD, (struct sockaddr *)&Addr_Guest, (socklen_t *)&len); // Enter I/O Mode

        if ( HostFD == SOCKET_ACCEPT_ERROR )
        {

            cout << "Socket Accepting : Error" << endl; 
        }
        else
        {

            cout << "--- Socket Accepting : OK ---" << endl;
            ptr_ether->Set_HostFD(idx) =  HostFD; 
            ptr_ether->Set_AddrGuest(idx) = Addr_Guest;  
            ptr_ether->Set_EstNum()++; 
         //  Address = inet_ntoa(ptr_ether->Get_AddrGuest(idx).sin_addr); 
            Address = inet_ntoa(Addr_Guest.sin_addr);

            printf("Guest[%d] Address : %s\n",idx,Address); 
            printf("Established Guest Number : %d\n",ptr_ether->Get_EstNum());
            idx++; 
                                
                 
           
        }
   
    }


}
