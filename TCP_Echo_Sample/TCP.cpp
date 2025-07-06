#include "TCP.h"
#include <stdio.h>
#include "Json.h"
#include <string>
void TCP::StartWSA()
{
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: 윈속을 초기화 할 수 없습니다.");
		return;
	}
}

void TCP::CreateTCPSocket()
{
	hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		puts("ERROR: 소켓을 생성할 수 없습니다.");
		return;
	}
}

void TCP::EndWSA()
{
	::WSACleanup();
}

void TCP::BindPort()
{
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(_port);
	const char* ip = _ipAddress;
	svraddr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (::bind(hSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		puts("ERROR: 소켓에 IP주소와 포트를 바인드 할 수 없습니다.");
		return;
	}
}

void TCP::ListenSocket()
{
	if (::listen(hSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		puts("ERROR: 리슨 상태로 전환할 수 없습니다.");
		return;
	}
	puts("TCP Server Listening...");
}

void TCP::AccepetAndCommunication()
{
	SOCKADDR_IN clientaddr = { 0 };
	int nAddrLen = sizeof(clientaddr);
	SOCKET hClient = 0;
	char szBuffer[128] = { 0 };
	int nReceive = 0;

	//클라이언트 연결을 받아들이고 새로운 소켓 생성(개방)
	while ((hClient = ::accept(hSocket,
		(SOCKADDR*)&clientaddr,
		&nAddrLen)) != INVALID_SOCKET)
	{
		puts("새 클라이언트가 연결되었습니다."); fflush(stdout);
		//클라이언트로부터 문자열을 수신함.
		while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0)) > 0)
		{
			// 여기에 Json 파싱해서 값을 측정한다.
			//수신한 문자열을 그대로 반향전송.
			::send(hClient, szBuffer, sizeof(szBuffer), 0);
			Json j;
			puts(j.ReadComment(szBuffer).c_str()); fflush(stdout);
			memset(szBuffer, 0, sizeof(szBuffer));
		}

		//클라이언트가 연결을 종료함.
		::shutdown(hClient, SD_BOTH);
		::closesocket(hClient);
		puts("클라이언트 연결이 끊겼습니다."); fflush(stdout);
	}
	::closesocket(hSocket);
}

void TCP::ConnectToServer()
{
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(_port);
	const char* ip = _ipAddress;
	svraddr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (::connect(hSocket,
		(SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		puts("ERROR: 서버에 연결할 수 없습니다.");
		return;
	}
}

void TCP::SendToServer()
{
	char szBuffer[128] = { 0 };
	while (1)
	{
		//사용자로부터 문자열을 입력 받는다.
		gets_s(szBuffer);
		if (strcmp(szBuffer, "EXIT") == 0)		break;

		Json j;
		std::string comment =j.WriteJson(szBuffer);
		//사용자가 입력한 문자열을 서버에 전송한다.
		::send(hSocket, comment.c_str(), comment.size(), 0);
		//서버로부터 방금 보낸 문자열에 대한 에코 메시지를 수신한다.
		memset(szBuffer, 0, sizeof(szBuffer));
		::recv(hSocket, szBuffer, sizeof(szBuffer), 0);
		printf("From server: %s\n", szBuffer);
	}
}

void TCP::CloseSocket()
{
	::closesocket(hSocket);
}

INT TCP::ReadIpAddress(char* ipAddress)
{
	if (ipAddress != nullptr)
	{
		_ipAddress = ipAddress;
		return 1;
	}
	return 0;
}

INT TCP::ReadPort(int port)
{
	_port = port;
	return 1;
}

INT TCP::ReadPort(char* port)
{

	if (port != nullptr)
	{
		const char* tmp = port;
		_port = atoi(tmp);
		return 1;
	}
	return 0;
}
