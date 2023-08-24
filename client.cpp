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
 

  private :

   int sockfd; 
   struct sockaddr_in Addr_Server; // IP(L3 Header) , Port(L4 Header)
      
    
 public :

 // Constructor
 ETH(char* ,int);  // Require Both IP Address & Port. 
 ~ETH(); 
 void Transmit(char* );
 char Receive();
 void Receive(char* ,int);  

 int GetSockFD();


  ETH* Get_This(); 


};

ETH :: ETH(char* Address ,int Port)
{
    
    char c; 
    cout << "--- Construcing Ethernet Instance ---" << endl; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    
    bzero(&Addr_Server,sizeof(Addr_Server));   
    
    Addr_Server.sin_family = AF_INET;  
    Addr_Server.sin_port = htons(Port); 
    Addr_Server.sin_addr.s_addr = inet_addr(Address); 
  
    if ( connect(sockfd, (struct sockaddr *)&Addr_Server, sizeof(Addr_Server)) == 0 )
    {

        cout << "--- Get SYN(1) + ACK(1) Flag From Server Sucessfully [ " << Address << " ] ---" << endl; 
        cout << "--- Client Enters Establishment To Server [ " << Address << " ] ---" << endl; 


    }
     
        

}

int ETH :: GetSockFD()
{


    return sockfd;
}


void ETH :: Transmit(char* Buffer)
{


     int state = write(sockfd,Buffer,strlen(Buffer)); 
     if( state == -1 )
     {

        cout << "--- Fail To Transmit Ethernet Data to Server ---" << endl; 

     }


}

char ETH :: Receive()
{
    
     char c;
     int state = read(sockfd,&c,1); 
     if( state == -1 )
     {

        cout << "--- Fail To Receive Ethernet Data From Server ---" << endl; 

     }


}

void ETH :: Receive(char* Buffer, int size)
{

    int state = read(sockfd,Buffer,size);
    if(state == -1)
    {

        cout <<"--- Fail To Receive Ethernet Data From Server ---" << endl; 

    }



}
ETH :: ~ETH()
{

     cout << "--- Destructing Object : " << this << " ---" << endl;  
     close(sockfd);

}

ETH* ETH :: Get_This()
{

return this; 

}


void *KeyButton_Rx_Task_Thread(void *);
void *ETH_Rx_Task_Thread(void *argu);
ETH *pETH; 



int main(int argc, char* argv[])
{



cout << "Hello World...!!!" <<endl; 
cout << "clinet...!!!" <<endl; 


    char* Address = new char[1024];
    int Port;


    pthread_t ETH_Rx_Task_Thread_TID;
    pthread_t KeyButton_Rx_Task_Thread_TID;

    cout << "Server Address : ";
    cin >> Address; 

    cout <<"Server Port : ";
    cin >> Port; 
   

    pETH = new ETH( Address,Port ); 

    // pETH->Transmit("Hello I'm Clinet ...!!!\n");

    cout << "pETH : " << (int *)pETH << endl;
    cout << "pETH->Get_This() : " << (int *)pETH->Get_This() << endl;

    pthread_create(&ETH_Rx_Task_Thread_TID, NULL, &ETH_Rx_Task_Thread, NULL);
    pthread_create(&KeyButton_Rx_Task_Thread_TID, NULL, &KeyButton_Rx_Task_Thread, NULL);

    while(1)
    {



    }

    pthread_join(ETH_Rx_Task_Thread_TID, NULL);
    pthread_join(KeyButton_Rx_Task_Thread_TID, NULL);



    return 0; 
}



void *KeyButton_Rx_Task_Thread(void *argu)
{


    cout << "---  KeyButton Rx Task Thread Entry Point ---" << endl;
    char Buffer[1024];
    int sockfd = pETH->GetSockFD(); 
    int state; 
    
    
    bzero(Buffer,sizeof(char)*1024);

    while(1)
    {

        cout << "Message For Host : ";
        cin >> Buffer;

        state = write(sockfd,Buffer,1024); 
        if( state != -1 )
        {

            bzero(Buffer,1024); 
            cout << "write ok" << endl; 
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
