#ifndef __DEFS_H__

#include <MyLib/myLogerDef.h>

#define PORT                8888    // 端口号
#define MESSAGE_BUFFER_SIZE 512     // 数据的字节数
#define TIMEOUT             2500

using namespace MyLib::MyLoger;

void ServerOperator(void);

void serverExitHandle(int);

void ClientOperator(const char * __ip, int __port);

void clientExitHandle(int);

#endif // __DEFS_H__