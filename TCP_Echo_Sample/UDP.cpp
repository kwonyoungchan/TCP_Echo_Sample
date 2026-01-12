#pragma warning(disable :4996)
#include "UDP.h"
#include <stdio.h>
#include <stdio.h>
#define BUF_SIZE 1024

using namespace std;

UDP::~UDP()
{
	hSocket = NULL;
}

INT UDP::Init()
{
	int socketChecker = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (socketChecker != 0)
	{
		puts("winsock 시작 안함");
		return 0;
	}
	hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	return 1;
}

INT UDP::setSockAddrforServer(int _port)
{
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	svraddr.sin_port = htons(_port);
	if (::bind(hSocket, getSOCKADDR(), sizeof(getSockAddress())) == SOCKET_ERROR)
	{
		puts("바인딩 안됨");
		return 0;
	}
	return 1;
}

INT UDP::setSockAddrforClient(char* _ipAddress, int _port)
{
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.S_un.S_addr = inet_addr(_ipAddress);
	svraddr.sin_port = htons(_port);
	return 1;
}

SOCKADDR_IN UDP::getSockAddress()
{
	return svraddr;
}

SOCKADDR* UDP::getSOCKADDR()
{
	SOCKADDR_IN SocketAddressTemp = getSockAddress();
	return (SOCKADDR*)&SocketAddressTemp;
}

INT UDP::Receive(char* buffer)
{
	int size = sizeof(svraddr);
	if (recvfrom(hSocket, buffer, BUF_SIZE, 0, (SOCKADDR*)&svraddr, &size))
	{
		printf("수신 : %s\n", buffer);
		return 1;
	}
	else
	{
		printf("수신 오류 %s\n", WSAGetLastError());
		return 0;
	}
}

void UDP::ReciveLoop()
{
	char* buf = new char[1024];
	int size = sizeof(svraddr);
	puts("패킷 수신중 ...\n");
	while (Receive(buf))
	{
		
	}
	printf("수신 완료\n");
}

INT UDP::Send(const char* buffer)
{
	if (strlen(buffer) == 0)
	{
		buffer = "none";
	}
	return sendto(hSocket, buffer, BUF_SIZE, 0, (SOCKADDR*)&svraddr, sizeof(getSockAddress()));
}

void UDP::SendLoop()
{
	int result = 1;

	while (result)
	{
		puts("전송할 데이터를 입렵하시오.");
		char buf[100];
		gets_s(buf);
		result = Send(buf);
	}
}
