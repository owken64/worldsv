#include "Database.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

MYSQL *conn     = NULL;
MYSQL_RES *resp = NULL;


int ConnectDB(){
  MYSQL_ROW row;
  char sql_str[255];
  char *sql_serv  = "localhost";
  char *user      = "root";
  char *passwd    = "";
  char *db_name   = "db_test";

  memset( &sql_str[0] , 0x00 , sizeof(sql_str) );

  // mysql接続
  conn = mysql_init(NULL);
  if( !mysql_real_connect(conn,sql_serv,user,passwd,db_name,0,NULL,0) ){
    // error
    return DATABASE_CONNECT_FAILURE;
  }
  
  return DATABASE_CONNECT_SUCCESS;
}

void CloseDB(){
 // 後片づけ
  mysql_free_result(resp);
  mysql_close(conn);
}
  