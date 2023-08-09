#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int ehh_fd;

void* KeyButton_Rx_Task_Thread(void* ); 
void *ETH_Rx_Task_Thread(void *);
void *ETH_Tx_Task_Thread(void *);





class Ethernet
{

private:
public:
    //
   //  Ethernet();
};

using namespace std;

int main(int argc, char *argv[])
{


     cout << "---It's a Entry Point of main Thread---" << endl;
  

    
    pthread_t ETH_Tx_Task_Thread_TID;
    pthread_t ETH_Rx_Task_Thread_TID;
    pthread_t KeyButton_Rx_Task_Thread_TID; 


    void* p_ETH_Tx_Task_Thread_RetValue;
    void* p_ETH_Rx_Task_Thread_RetValue;
    void* p_KeyButton_Rx_Task_Thread_RetValue;


    pthread_create(&ETH_Tx_Task_Thread_TID, NULL, &ETH_Tx_Task_Thread, NULL);
    pthread_create(&ETH_Rx_Task_Thread_TID, NULL, &ETH_Rx_Task_Thread, NULL);
    pthread_create(&KeyButton_Rx_Task_Thread_TID, NULL, &KeyButton_Rx_Task_Thread, NULL);



    pthread_join(ETH_Tx_Task_Thread_TID,&p_ETH_Tx_Task_Thread_RetValue);
    pthread_join(ETH_Rx_Task_Thread_TID,&p_ETH_Rx_Task_Thread_RetValue);
    pthread_join(KeyButton_Rx_Task_Thread_TID,&p_KeyButton_Rx_Task_Thread_RetValue);


    cout << (char*)p_ETH_Tx_Task_Thread_RetValue << "Should, Must Be No Thing" << endl; 
    cout << (char*)p_ETH_Rx_Task_Thread_RetValue << "Should, Must Be No Thing" << endl;
    cout << (char*)p_KeyButton_Rx_Task_Thread_RetValue << "Should, Must Be No Thing" << endl; 



   

    return 0;
}

void* KeyButton_Rx_Task_Thread(void* argu)
{


    cout << "---It's a Entry Point of KeyButton Receive Task Thread---" << endl;

    char Buffer[1024];
    memset(Buffer,0x0,sizeof(Buffer)); 

    while(1)
    {


        cout << "\nUnder Infinite Loop of KeyButton_Rx_Task Thread..." << endl;
        scanf("%s",Buffer); 
        cout << "Console = " << Buffer << endl; 
        

       

        sleep(1); 




    }


    return NULL; 



    
}

void *ETH_Rx_Task_Thread(void *argu)
{



    cout << "---It's a Entry Point of Ethernet Receive Task Thread---" << endl;

    while(1)
    {


        cout << "\nUnder Infinite Loop of ETH_Rx_Task Thread..." << endl;
        sleep(1); 




    }


    return NULL; 



}

void *ETH_Tx_Task_Thread(void *argu)
{


    cout << "---It's a Entry Point of Ethernet Transmit Task Thread---" << endl;


    while(1)
    {


          cout << "\nUnder Infinite Loop of ETH_Tx_Task Thread..." << endl;
          sleep(1); 



    }



    return NULL; 


}
