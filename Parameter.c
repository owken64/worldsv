#include "Parameter.h"
#include "Config.h"
#include "Bool.h"
#include <arpa/inet.h>
#include <sys/select.h>

// as Server
int sockForGamesv;
int sockForMsgsv;
int sockForLoginsv;

struct sockaddr_in sockAddrForGamesv;
struct sockaddr_in sockAddrForMsgsv;
struct sockaddr_in sockAddrForLoginsv;


// for Client
int sockGamesv;
int sockMsgsv;
int sockLoginsv;

struct sockaddr_in sockAddrGamesv;
struct sockaddr_in sockAddrMsgsv;
struct sockaddr_in sockAddrLoginsv;

// as Client
int sockLogsv;
struct sockaddr_in sockAddrLogsv;

// for select function
fd_set readfds;

// Message
char messageGamesv[MSG_LENGTH];
char messageMsgsv[MSG_LENGTH];
char messageLoginsv[MSG_LENGTH];
char messageLogsv[MSG_LENGTH];
BOOL flgRecvGamesv;
BOOL flgRecvMsgsv;
BOOL flgRecvLoginsv;
BOOL flgSendLogsv;

