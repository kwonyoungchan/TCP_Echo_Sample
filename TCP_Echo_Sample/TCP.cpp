#include "TCP.h"
#include <stdio.h>
#include "Json.h"
#include <string>


CRITICAL_SECTION  g_cs;
std::list<SOCKET> g_listClient;

void SendMulticast(char* pszParam)
{
	int nLength = strlen(pszParam);
	std::list<SOCKET>::iterator it;

	::EnterCriticalSection(&g_cs);
	for (it = g_listClient.begin(); it != g_listClient.end(); ++it)
	{
		::send(*it, pszParam, sizeof(char) * (nLength + 1), 0);
	}
	::LeaveCriticalSection(&g_cs);
}

DWORD MultiTreadFunction(LPVOID pParam)
{
	char szBuffer[128] = { 0 };
	int nReceive = 0;
	SOCKET hClient = (SOCKET)pParam;

	puts("새 클라이언트가 연결되었습니다.");
	while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0)) > 0)
	{
		puts(szBuffer);

		SendMulticast(szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}
	puts("클라이언트와 연결이 끊어졌습니다.");
	::EnterCriticalSection(&g_cs);
	g_listClient.remove(hClient);
	::LeaveCriticalSection(&g_cs);

	::closesocket(hClient);
	return 0;
}

DWORD WINAPI ThreadReceive(LPVOID pParam)
{
	SOCKET hSocket = (SOCKET)pParam;
	char szBuffer[128] = { 0 };
	while (::recv(hSocket, szBuffer, sizeof(szBuffer), 0) > 0)
	{
		printf("-> % s\n", szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}
	puts("수신 쓰레드가 종료되었습니다.");
	return 0;
}


void TCP::StartWSA()
{
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: 윈속을 초기화 할 수 없습니다.");
		return;
	}
	// Critical Section 초기화
	::InitializeCriticalSection(&g_cs);
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
	DWORD dwThreadID = 0;
	HANDLE hThread;

	//클라이언트 연결을 받아들이고 새로운 소켓 생성(개방)
	while ((hClient = ::accept(hSocket,
		(SOCKADDR*)&clientaddr,
		&nAddrLen)) != INVALID_SOCKET)
	{
		if (AddUser(hClient) == FALSE)
		{
			puts("ERROR : 더 이상 클라이언트 연결을 처리할 수 없습니다.");
			break;
		}	

		hThread = ::CreateThread(
			NULL,
			0,
			MultiTreadFunction,
			(LPVOID)hClient,
			0,
			&dwThreadID);
		::CloseHandle(hThread);
		
		
	}
	puts("*** 채팅 서버를 종료합니다. ***"); 
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

	//채팅 메시지 수신 스레드 생성
	DWORD dwThreadID = 0;
	HANDLE hThread = ::CreateThread(NULL,	//보안속성 상속
		0,					//스택 메모리는 기본크기(1MB)
		ThreadReceive,		//스래드로 실행할 함수이름
		(LPVOID)hSocket,	//소켓 핸들을 매개변수로 넘김
		0,					//생성 플래그는 기본값 사용
		&dwThreadID);		//생성된 스레드ID가 저장될 변수주소
	::CloseHandle(hThread);

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
	// CriticalSection 종료
	::DeleteCriticalSection(&g_cs);
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

BOOL TCP::AddUser(SOCKET hSocket)
{
	// Critical Section 접근 후, client Socket을 List에 저장한다.
	::EnterCriticalSection(&g_cs);
	g_listClient.push_back(hSocket);
	::LeaveCriticalSection(&g_cs);

	return TRUE;
}


