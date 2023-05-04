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

#define PORT 8080
#define MAX 1024
#define TxBufferSize 10
#define RxBufferSize 1024

int init_sock_client(char *address, int port)
{

  int sockfd;
  struct sockaddr_in addr_server;

  bzero(&addr_server, sizeof(addr_server));
  addr_server.sin_family = AF_INET;
  addr_server.sin_port = htons(port);
  addr_server.sin_addr.s_addr = inet_addr(address);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1)
  {
    printf("소켓 생성에 실패하였습니다.\n");
    printf("프로그램을 종료합니다.\n");
    exit(0);
  }
  else
    printf("소켓 생성에 성공하였습니다.\n");

  if (connect(sockfd, (struct sockaddr *)&addr_server, sizeof(addr_server)) == 0)
  {

    printf("서버에 연결을 성공하였습니다.\n");
    return sockfd;
  }
  else
  {
    printf("서버에 연결을 실패하였습니다.\n");
    printf("프로그램을 종료합니다.\n");
    exit(0);
  }
}
int enable_chat_client_module(int sockfd)
{

  char bufferTx[TxBufferSize];
  char bufferRx[RxBufferSize];

  memset(bufferTx, 0, sizeof(bufferTx));
  memset(bufferRx, 0, sizeof(bufferRx));

  int var = 0;

  printf("서버와 채팅이 활성화 하였습니다.\n");
  getchar();
  while (1)
  {

    printf("To Server : ");
    memset(bufferTx, 0, sizeof(bufferTx));
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




    write(sockfd, bufferTx, sizeof(bufferTx));

    if (strncmp("quit", bufferTx, 4) == 0)
      break;

    memset(bufferRx, 0, sizeof(bufferRx));

    read(sockfd, bufferRx, sizeof(bufferRx));
    printf("From Server : %s\n", bufferRx);
    if (strncmp("quit", bufferRx, 4) == 0) return -1;    

  
  };


 

}
  int do_chat_client(char *address, int port)
  {

    int sockfd;
    sockfd = init_sock_client(address, port);
    if (enable_chat_client_module(sockfd) == -1)
    {

      close(sockfd);
      return -1;
    }
  }
  int main(int argc, char **argv[])
  {

    printf("클라이언트 프로그램입니다 ( 재작자 : 오선철 ).\n");
    printf("Ubuntu-18.04환경에서 재작하였습니다.\n");
    printf("쓰레드를 이용하여 동시 채팅 기능 구현 예정입니다...\n");

    int port;
    char address[MAX];
    int var;

    // 0.0.0.0 : 3+4 = 7
    // 192.168.100.100 = 3 + 3*4 = 3*5 = 15.

    printf("Port : ");
    scanf("%d", &port);
    printf("Address : ");
    scanf("%s", address);

    // getchar();

    if (do_chat_client(address, port) == -1)
      printf("프로그램 종료되었습니다.\n");

    return 0;
  }
