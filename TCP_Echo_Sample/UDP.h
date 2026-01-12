#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS	// 최신 C++ 표준을 사용하면 C4996 오류가 발생하는데 해당 코드 입력하면 해결됨 
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

class UDP
{
public:
	UDP() {}
	~UDP();

	INT Init();

	INT setSockAddrforServer(int _port);
	INT setSockAddrforClient(char* _ipAddress, int _port);
	SOCKADDR_IN getSockAddress();
	SOCKADDR* getSOCKADDR();
public:
	INT Receive(char* buffer);
	void ReciveLoop();
	INT Send(const char* buffer);
	void SendLoop();
private:
	WSADATA wsa = { 0 };
	SOCKET hSocket = NULL;
	SOCKADDR_IN	svraddr = { 0 };

private:
	char* _ipAddress = { 0 };
};

