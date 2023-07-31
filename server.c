#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <wchar.h>
#include <locale.h>

#define MAX 1024
#define PORT 8080
#define TxBufferSize 10
#define RxBufferSize 1024

// servaddr.sin_family = AF_INET;
// servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
// servaddr.sin_port = htons(PORT);

// create socket
// init socket address and port data
// bind ( assign ip and port to socket )
// listen
// connect

// int sockfd = socket(int domain, int type,int protocol) ---> return sockfd
// -1 , 0 = bind ( int sockfd, (struct sockaddr*)&sockaddr_in_var, sizeof(sockaddr_in_var));
// -1 , 0 = listen(int sockfd, int backlog) ---> -1 is fail
// int len_clientaddr = sizeof(clientaddr)
// int confd = accpet(int sockfd , (struct sockaddr*)&client_addr, &len_clientaddr));
//

int init_sock_server(int port) // IP , Address , Clinet Num/
{

   int sockfd;
   struct sockaddr_in addr_server;

   bzero(&addr_server, sizeof(addr_server));
   addr_server.sin_family = AF_INET;
   addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
   addr_server.sin_port = htons(port);

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd == -1)
   {
      printf("소켓 생성에 실패하였습니다.\n");
      exit(0);
   }
   else
      printf("소켓 생성에 성공하였습니다.\n");

   if (bind(sockfd, (struct sockaddr *)&addr_server, sizeof(addr_server)) == 0)
   {

      printf("소켓 바인딩에 성공하였습니다.\n");
      return sockfd;
   }
   else
   {
      printf("소켓 바인딩에 실패하였습니다.\n");
      exit(0);
   }
}
int do_wait(int sockfd, int nClient) // client address.
{

   int len;
   int connfd;
   struct sockaddr_in addr_client;
   bzero(&addr_client, sizeof(addr_client));
   if (listen(sockfd, nClient) == -1)
   {
      printf("클라이언트 리스닝에 실패하였습니다.\n");
      exit(0);
   }
   else
   {

      printf("클라이언트 리스닝에 성공하였습니다.\n");
   }

   len = sizeof(addr_client);
   connfd = accept(sockfd, (struct sockaddr *)&addr_client, &len);

   if (connfd == -1)
   {

      printf("클라이언트와 연결에 실패하였습니다.\n");
      exit(0);
   }
   else
   {

      printf("클라이언트와 연결에 성공하였습니다.\n");
      return connfd;
   }
}

int enable_chat_module_server(int connfd) // server address , port , the number of client.
{

   char buffer[MAX];
   char bufferTx[TxBufferSize];
   char bufferRx[RxBufferSize];

   memset(bufferRx, 0, sizeof(bufferRx));
   memset(bufferTx, 0, sizeof(bufferTx));

   int var = 0;

   printf("클라이언트와 채팅을 시작합니다.\n");
   getchar();

   while (1)
   {

      memset(bufferRx, 0, sizeof(bufferRx));

      read(connfd, bufferRx, sizeof(bufferRx));
      printf("From Client : %s\n", bufferRx);
      if (strncmp("quit", bufferRx, 4) == 0)
         break;

      memset(bufferTx, 0, sizeof(bufferTx));
      printf("To Client : ");
      scanf("%s",bufferTx);
      /*
      var = 0;
      
      while (((bufferTx[var++] = getchar()) != '\n'))
      {

         if (var > TxBufferSize)
         {

            printf("TxBuffer OverFlow Error...");
            return -1;
         }
      }
      */
      write(connfd, bufferTx, sizeof(bufferTx));
      if (strncmp("quit", bufferTx, 4) == 0) return -1;        
      
   }; 

  
}

int do_chat_server(int port, int nclient)
{

   int sockfd;
   int connfd;

   sockfd = init_sock_server(port);
   connfd = do_wait(sockfd, nclient);

   if (enable_chat_module_server(connfd) == -1)
   {

      close(sockfd);
      return -1;
   }
}

int main(int argc, char *argv[])
{

   printf("서버 프로그램입니다 ( 재작자 : 오선철 ).\n");
   printf("Ubuntu-18.04환경에서 재작하였습니다.\n");
   printf("쓰레드를 이용하여 동시 채팅 기능 구현 예정입니다...\n");

   int port, nclient;

   printf("Port : ");
   scanf("%d", &port);
   printf("The number of client : ");
   scanf("%d", &nclient);

   if (do_chat_server(port, nclient) == -1)
      printf("프로그램 종료되었습니다.\n");

   return 0;
}
