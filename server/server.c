// TCPServer.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

//Windows VC++ での　TCP/IP サンプルプログラム（ここからサーバー）
//クライアントから送られてきた文字列を大文字に変換して送り返す
//サーバープログラムを実行してからクライアントプログラムを実行して下さい

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> // inet_addr()
#include <netinet/in.h> // inet_addr()
#include <netinet/tcp.h> // TCP_NODELAY
#include <arpa/inet.h> // inet_addr()
#pragma comment(lib, "ws2_32.lib")//pragma must set before define

#include "server.h"

#define PORT 9876 //クライアントプログラムとポート番号を合わせてください


int Socket_Recv_Sock(Thread_Arg *thread_arg) {
  int i;
  // ポート番号，ソケット
  int srcSocket;  // 自分
  int dstSocket;  // 相手

  // sockaddr_in 構造体
  struct sockaddr_in srcAddr;
  struct sockaddr_in dstAddr;
  int dstAddrSize = sizeof(dstAddr);
  int status;
  // 各種パラメータ
  int numrcv;
  char buffer[1024];
  char pt[2];

  printf("--- server ---\n");
  // sockaddr_in 構造体のセット
  memset(&srcAddr, 0, sizeof(srcAddr));
  srcAddr.sin_port = htons(PORT);
  srcAddr.sin_family = AF_INET;
  srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // ソケットの生成（ストリーム型）
  srcSocket = socket(AF_INET, SOCK_STREAM, 0);
  // ソケットのバインド
  bind(srcSocket, (struct sockaddr *) &srcAddr, sizeof(srcAddr));
  // 接続の許可
  listen(srcSocket, 1);

  while (1) { //ループで回すことによって何度でもクライアントからつなぐことができる
    if(thread_arg->end_flag == 1){
      break;
    }

    // 接続の受付け
    printf("接続を待っています\nクライアントプログラムを動かしてください\n");
    dstSocket = accept(srcSocket, (struct sockaddr *) &dstAddr, &dstAddrSize);
    printf("%s から接続を受けました\n", inet_ntoa(dstAddr.sin_addr));

    //  while (1) {
    //パケットの受信
    numrcv = recv(dstSocket, thread_arg->recv_data, sizeof(char) * 19, 0);
    if (numrcv == 0 || numrcv == -1) {
      status = close(dstSocket);
      //break;
    }
    printf("recv: %s, size: %lu, len:%lu\n", thread_arg->recv_data, sizeof(thread_arg->recv_data), /*strlen(recv_data)*/numrcv);
    if(thread_arg->recv_data[0] == 0x02 && thread_arg->recv_data[17] == 0x03) {
      if(thread_arg->recv_data[1] == 'P' && thread_arg->recv_data[2]  == 'T'){
	printf("=== Recive OK ===\n");
      }
    } else {
      printf("# STX or EDX ERROR\n");
      break;
    }
    if(thread_arg->recv_data[1] == 'E'){
      thread_arg->end_flag = 1;
      break;
    }
<<<<<<< HEAD

      /* printf("### char ###\n"); */
      /* for(i = 0; i < 18; i++) { */
      /* printf("i:%d, char:%c, x: %x\n", i, thread_arg->recv_data[i], thread_arg->recv_data[i]); */
      /* } */
      /* printf("###########\n"); */

    /* if(strlen(thread_arg->recv_data) < 18) { */
    /*   printf("コマンドが短すぎます\n"); */
    /* } */
=======
    /*
      printf("### char ###\n");
      for(i = 0; i < 27; i++) {
      printf("i:%d, char:%c, x: %x\n", i, recv_data[i], recv_data[i]);
      }
      printf("###########\n");
    */

    if(strlen(thread_arg->recv_data) < 18) {
      printf("コマンドが短すぎます\n");
    }
>>>>>>> hiraoka/master
    // パケットの送信
    send(dstSocket, thread_arg->recv_data, sizeof(char) * 19, 0);
    //if(strcmp(&recv_data[0], "0") == 0) break;
    //}
    //if(recv_data[1] == '0') break;
    printf("--- Socket_Recv_Soc ---\n");
    printf("recv : %s\n", thread_arg->recv_data);

    pthread_mutex_lock(&thread_arg->mutex);
    {
      // コマンド解析
<<<<<<< HEAD
      // thread_arg->command = Sock_Command_Analysis(thread_arg->recv_data);
      Sock_Command_Analysis(thread_arg->recv_data, &thread_arg->command);
=======
      thread_arg->command = Sock_Command_Analysis(thread_arg->recv_data);
>>>>>>> hiraoka/master
      thread_arg->recv_flag = 1; /* 受信完了 */
    }
    pthread_mutex_unlock(&thread_arg->mutex);

    printf("--- Sock_Command_Analysis ---\n");
    printf("recv : %s\n", thread_arg->recv_data);
  }
  close(srcSocket);
  close(dstSocket);
  return 0;
}

/*
  int main () {
  char recv_data[19];
  Socket_Recv_Sock(recv_data);
  printf("main\n");
  printf("recv: %s\n", recv_data);
  return 0;
  }
*/
