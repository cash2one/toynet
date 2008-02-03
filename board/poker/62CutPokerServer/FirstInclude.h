
// 생성 가능한 최대 소켓 수 지정(Default=64, Win98에선 최대 80개정도밖에 생성되지 않는다)
#ifndef FD_SETSIZE
#define FD_SETSIZE 10000
#endif

// 최대 데이터 소켓 개수
#ifndef MAX_DATA_SOCKET
#define MAX_DATA_SOCKET 3100
#endif

#include "Winsock2.h"
