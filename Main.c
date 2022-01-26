#include <unistd.h>
#include <stdio.h>

#include "Database.h"
#include "Network.h"
#include "Bool.h"
#include "Parameter.h"

void mainLoop(void){

	// 初期処理
	ConnectDB();
	NetworkInit();
	
	// ループ
	while (TRUE){
	  
	  // メッセージを受け取ったら
	  NetworkReceive();
	  // DBへクエリを送信する
	  
	  // 返す必要があれば
	  
	  // 結果をサーバへ返す
	  
	  // 出力すべきログがあれば
	  if(flgSendLogsv){
	    // ログをログサーバへ送信する
      }
	}

	// 終了処理	
	NetworkClose();
	CloseDB();
	
}

int main(void){
    //ここで子プロセスを生成し親は終了
    if(daemon(0, 0) == 0) {
        mainLoop();
    } else {
        printf("error\n");
    }
    return 0;
}