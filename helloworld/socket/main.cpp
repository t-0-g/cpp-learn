#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include <unistd.h>

#define QUEUELIMIT 5
#define MSGSIZE 32
#define MAX_MSGSIZE 1024
#define BUFFSIZE (MSGSIZE+1)

using std::thread;
using std::cout;
using std::endl;

typedef struct _worker_args {
  int sockfd;
  int id;
} worker_args;

void worker(void *arg){
  worker_args* wargs = (worker_args *)arg;
  int fd = wargs->sockfd;
  char buf[BUFFSIZE];
  while(true){
    recv(fd, buf, sizeof(buf), MSG_WAITALL);
//    send(fd, buf, sizeof(buf), MSG_WAITALL);
  }
}


void server(){
  cout << "S:server start" << endl;

  int servSock;
  int clientSock;
  struct sockaddr_in servSockAddr;
  struct sockaddr_in clientSockAddr;

  unsigned short servPort = 51111;
  unsigned int clitLen;

  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("socket() failed." );
    exit(EXIT_FAILURE);
  }

  memset(&servSockAddr, 0, sizeof(servSockAddr));
  servSockAddr.sin_family = AF_INET;
  servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servSockAddr.sin_port = htons(servPort);

  if (bind(servSock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr)) < 0){
    perror("bind() failed.");
    exit(EXIT_FAILURE);
  }

  if (listen(servSock, QUEUELIMIT) < 0) {
    perror("listen() failed.");
    exit(EXIT_FAILURE);
  }

  while (true) {
    clitLen = sizeof(clientSockAddr);
    if ((clientSock = accept(servSock, (struct sockaddr *) &clientSockAddr, &clitLen)) < 0 ) {
      perror("accept() failed.");
      exit(EXIT_FAILURE);
    }

    printf("S:connected from %s.\n", inet_ntoa(clientSockAddr.sin_addr));

    worker_args *wargs = (worker_args *)malloc(sizeof(worker_args));

    wargs->id = 10;
    wargs->sockfd = clientSock;
    thread workerThread(worker, (void*)wargs);
    workerThread.detach();
  }    
}

void client(int i){
  cout << "C"<< i <<":client start" << endl;
  int sock;
  struct sockaddr_in servSockAddr;
  unsigned short servPort = 51111;
  memset(&servSockAddr, 0, sizeof(servSockAddr));
  servSockAddr.sin_family = AF_INET;
  if (inet_aton("127.0.0.1", &servSockAddr.sin_addr) == 0) {
    fprintf(stderr, "invalid IP Address.\n");
    exit(EXIT_FAILURE);
  }
  servSockAddr.sin_port = htons(servPort);
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("socket() failed.");
    exit(EXIT_FAILURE);
  }
  if(connect(sock, (struct sockaddr*) &servSockAddr, sizeof(servSockAddr)) < 0){
    perror("connect() failed.");
    exit(EXIT_FAILURE);
  }
  printf("C:connect to %s\n", inet_ntoa(servSockAddr.sin_addr));
  while(true) {
    char buf[BUFFSIZE];
    sprintf(buf, "%d",i);
    send(sock, buf, sizeof(buf), 0);
    recv(sock, buf, sizeof(buf), MSG_WAITALL);
    printf("C:From server: %s\n", buf);
    sleep(2);
  }
}

int main(){
  thread serverThread(server);
  sleep(3);
  for (int i=0; i<5; i++){
    thread clientThread(client, i);
    clientThread.detach();
    sleep(1);
  }
  serverThread.join();
}

