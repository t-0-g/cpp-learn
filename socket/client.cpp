#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
 
using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in addr;
    int send_msg_num = 100000;
    int msg_size = atoi(argv[1]);


   	std::chrono::system_clock::time_point start, end;

	  start = std::chrono::system_clock::now();

    // ソケット生成
    if( (sockfd = socket( AF_INET, SOCK_STREAM, 0) ) < 0 ) {
        perror( "socket" );
    }
 
    // 送信先アドレス・ポート番号設定
    addr.sin_family = AF_INET;
    addr.sin_port = htons( 1234 );
    addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
 

    // サーバ接続
    connect( sockfd, (struct sockaddr *)&addr, sizeof( struct sockaddr_in ) );

 
    // データ送信
    char send_str[msg_size];
    char receive_str[msg_size];
    for ( int i = 0; i < send_msg_num; i++ ){
        sprintf( send_str, "%d", i );
        if( send( sockfd, send_str, msg_size, 0 ) < 0 ) {
            perror( "send" );
        } else {
            recv( sockfd, receive_str, msg_size, 0 );
        }
    }
    end = std::chrono::system_clock::now();
	  double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
	  cout << "time: " << elapsed / 1000 << " ms" << endl;
 
    // ソケットクローズ
    close( sockfd );
 
    return 0;
}
