#ifndef __DEFS_H__

#include <MyLib/myLogerDef.h>

// #define IPV4_STRING_LENGTH  17   // IPv4 字符串长度（12 个数字字符 + 4 个 '.' + 1 空字符）

#define PORT                8888    // 端口号
#define MESSAGE_BUFFER_SIZE 4096    // 数据缓冲区的大小    （单位：字节）
#define TIMEOUT             1000    // 等待检查套接字的时间（单位：毫秒）

using namespace MyLib::MyLoger;

// typedef struct IPaddress {};

/**
 * @brief 服务器端操作 
*/
void ServerOperator(void);

/**
 * @brief 服务器端收到退出信号时的操作，由 signal() 函数调用。
*/
void serverExitHandle(int);

/**
 * @brief 客户端操作。
 * 
 * @param __ip          服务器端的 IP 地址
 * @param __port        通信端口号
*/
void ClientOperator(const char * __ip, int __port);

/**
 * @brief 客户端收到退出信号时的操作，由 signal() 函数调用。
*/
void clientExitHandle(int);

#endif // __DEFS_H__