#pragma once
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <sys/select.h>
#include "Config.h"
#include "Bool.h"

// as Server
extern int sockForGamesv;
extern int sockForMsgsv;
extern int sockForLoginsv;

extern struct sockaddr_in sockAddrForGamesv;
extern struct sockaddr_in sockAddrForMsgsv;
extern struct sockaddr_in sockAddrForLoginsv;


// for Client
extern int sockGamesv;
extern int sockMsgsv;
extern int sockLoginsv;

extern struct sockaddr_in sockAddrGamesv;
extern struct sockaddr_in sockAddrMsgsv;
extern struct sockaddr_in sockAddrLoginsv;

// as Client
extern int sockLogsv;
extern struct sockaddr_in sockAddrLogsv;

// for select function
extern fd_set readfds;


// Message
extern char messageGamesv[MSG_LENGTH];
extern char messageMsgsv[MSG_LENGTH];
extern char messageLoginsv[MSG_LENGTH];
extern char messageLogsv[MSG_LENGTH];
extern BOOL flgRecvGamesv;
extern BOOL flgRecvMsgsv;
extern BOOL flgRecvLoginsv;
extern BOOL flgSendLogsv;

