#include "Network.h"

#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()
#include "Config.h"
#include "Bool.h"
#include "Parameter.h"

#define QUEUELIMIT 5

int ConnectGamesv();
int ConnectMsgsv();
int ConnectLoginsv();
int ConnectLogsv();
void CloseGamesv();
void CloseMsgsv();
void CloseLoginsv();
void CloseLogsv();

int NetworkInit(){
	// ファイルディスクリプタの初期化
	FD_ZERO(&readfds);

	if (ConnectGamesv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	
	if (ConnectMsgsv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	
	if (ConnectLoginsv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	if (ConnectLogsv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}

	return NETWORK_CONNECT_SUCCESS;
}

void NetworkClose(){
	CloseGamesv();
	CloseMsgsv();
	CloseLoginsv();
	CloseLogsv();
}

int NetworkReceive(){
  fd_set fds;
  unsigned int clitLen; // client internet socket address length
  // 初期化
  // 読み込み用fd_setの初期化
  // selectが毎回内容を上書きしてしまうので、毎回初期化します
  memcpy(&fds, &readfds, sizeof(fd_set));

  flgRecvGamesv = FALSE;
  flgRecvMsgsv = FALSE;
  flgRecvLoginsv = FALSE;

  // fdsに設定されたソケットが読み込み可能になるまで待ちます
  select(0, &fds, NULL, NULL, NULL);

  // サーバのソケットが受信したならば
  if(FD_ISSET(sockForGamesv, &fds)){
    // Accept
    clitLen = sizeof(sockAddrGamesv);
    if ((sockGamesv = accept(sockForGamesv, (struct sockaddr *) &sockAddrGamesv, &clitLen)) < 0) {
      perror("accept() failed.");
      return NETWORK_RECEIVE_FAILURE;
    }
    FD_SET(sockGamesv, &readfds);
  }

  if(FD_ISSET(sockForMsgsv, &fds)){
    // Accept
    clitLen = sizeof(sockAddrMsgsv);
    if ((sockMsgsv = accept(sockForMsgsv, (struct sockaddr *) &sockAddrMsgsv, &clitLen)) < 0) {
      perror("accept() failed.");
      return NETWORK_RECEIVE_FAILURE;
    }

    FD_SET(sockMsgsv, &readfds);
  }

  if(FD_ISSET(sockForLoginsv, &fds)){
    // Accept
    clitLen = sizeof(sockAddrLoginsv);
    if ((sockLoginsv = accept(sockForLoginsv, (struct sockaddr *) &sockAddrLoginsv, &clitLen)) < 0) {
      perror("accept() failed.");
      return NETWORK_RECEIVE_FAILURE;
    }
    
    FD_SET(sockLoginsv, &readfds);
  }

  if (FD_ISSET(sockGamesv, &fds)){
    // パケット受信
    if(recv(sockGamesv, messageGamesv, sizeof(messageGamesv), 0) < 0) {
      flgRecvGamesv = FALSE;
      perror("recv");
      return NETWORK_RECEIVE_FAILURE;
    }
    flgRecvGamesv = TRUE;
  }
  
  if (FD_ISSET(sockMsgsv, &fds)) {
    // パケット受信
    if(recv(sockMsgsv, messageMsgsv, sizeof(messageMsgsv), 0) < 0) {
      flgRecvMsgsv = FALSE;
      perror("recv");
      return NETWORK_RECEIVE_FAILURE;
    }
    flgRecvMsgsv = TRUE;
  }
  
  if (FD_ISSET(sockLoginsv, &fds)) {
    // パケット受信
    if(recv(sockLoginsv, messageLoginsv, sizeof(messageLoginsv), 0) < 0) {
      flgRecvLoginsv = FALSE;
      perror("recv");
      return NETWORK_RECEIVE_FAILURE;
    }
    flgRecvLoginsv = TRUE;
  }
  
  // Gamesv, Msgsv, Loginsvのいずれか2つ以上からのデータを同時に受信すると、あとから受信したデータだけが残る(バグ)

  return NETWORK_RECEIVE_SUCCESS;
}

int ConnectGamesv(){
	memset(&sockAddrForGamesv, 0, sizeof(sockAddrForGamesv));
    sockAddrForGamesv.sin_family      = AF_INET;
    sockAddrForGamesv.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddrForGamesv.sin_port        = htons(PORT_GAMESV);

    if (bind(sockForGamesv, (struct sockaddr *) &sockAddrForGamesv, sizeof(sockAddrForGamesv) ) < 0 ) {
        perror("bind() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    if (listen(sockForGamesv, QUEUELIMIT) < 0) {
        perror("listen() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

	// fd setへの追加
	FD_SET(sockForGamesv, &readfds);

	return NETWORK_CONNECT_SUCCESS;
}

int ConnectMsgsv(){
	memset(&sockAddrForMsgsv, 0, sizeof(sockAddrForMsgsv));
    sockAddrForMsgsv.sin_family      = AF_INET;
    sockAddrForMsgsv.sin_addr.s_addr = inet_addr(IP_MSGSV);
    sockAddrForMsgsv.sin_port        = htons(PORT_MSGSV);

    if (bind(sockForMsgsv, (struct sockaddr *) &sockAddrForMsgsv, sizeof(sockAddrForMsgsv) ) < 0 ) {
        perror("bind() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    if (listen(sockForMsgsv, QUEUELIMIT) < 0) {
        perror("listen() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    // fd setへの追加
    FD_SET(sockForMsgsv, &readfds);

	return NETWORK_CONNECT_SUCCESS;
}

int ConnectLoginsv(){
	memset(&sockAddrForLoginsv, 0, sizeof(sockAddrForLoginsv));
    sockAddrForLoginsv.sin_family      = AF_INET;
    sockAddrForLoginsv.sin_addr.s_addr = inet_addr(IP_LOGINSV);
    sockAddrForLoginsv.sin_port        = htons(PORT_LOGINSV);

    if (bind(sockForLoginsv, (struct sockaddr *) &sockAddrForLoginsv, sizeof(sockAddrForLoginsv) ) < 0 ) {
        perror("bind() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    if (listen(sockForLoginsv, QUEUELIMIT) < 0) {
        perror("listen() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

	// fd setへの追加
	FD_SET(sockForLoginsv, &readfds);
	
	return NETWORK_CONNECT_SUCCESS;
}

int ConnectLogsv(){

	// IPv4 TCP のソケットを作成する
    if((sockLogsv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NETWORK_CONNECT_FAILURE;
    }
 
    // 送信先アドレスとポート番号を設定する
    sockAddrLogsv.sin_family = AF_INET;
    sockAddrLogsv.sin_port = htons(PORT_LOGSV);
    sockAddrLogsv.sin_addr.s_addr = inet_addr(IP_LOGSV);
 
    // サーバ接続（TCP の場合は、接続を確立する必要がある）
    connect(sockLogsv, (struct sockaddr *)&sockAddrLogsv, sizeof(struct sockaddr_in));

	return NETWORK_CONNECT_SUCCESS;
}

void CloseGamesv(){
    FD_CLR(sockGamesv, &readfds);
	close(sockGamesv);
	
	FD_CLR(sockForGamesv, &readfds);
	close(sockForGamesv);
}

void CloseMsgsv(){
	FD_CLR(sockMsgsv, &readfds);
	close(sockMsgsv);
	
	FD_CLR(sockForMsgsv, &readfds);
	close(sockForMsgsv);
}

void CloseLoginsv(){
	FD_CLR(sockLoginsv, &readfds);
	close(sockLoginsv);
	
	FD_CLR(sockForLoginsv, &readfds);
	close(sockForLoginsv);
}

void CloseLogsv(){
	close(sockLogsv);
}
