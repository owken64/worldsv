#pragma once

#define DATABASE_CONNECT_SUCCESS 0
#define DATABASE_CONNECT_FAILURE -1

int ConnectDB();
void CloseDB();

