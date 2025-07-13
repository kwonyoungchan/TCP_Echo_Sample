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

	puts("�� Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.");
	while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0)) > 0)
	{
		puts(szBuffer);

		SendMulticast(szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}
	puts("Ŭ���̾�Ʈ�� ������ ���������ϴ�.");
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
	puts("���� �����尡 ����Ǿ����ϴ�.");
	return 0;
}


void TCP::StartWSA()
{
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: ������ �ʱ�ȭ �� �� �����ϴ�.");
		return;
	}
	// Critical Section �ʱ�ȭ
	::InitializeCriticalSection(&g_cs);
}

void TCP::CreateTCPSocket()
{
	hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		puts("ERROR: ������ ������ �� �����ϴ�.");
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
		puts("ERROR: ���Ͽ� IP�ּҿ� ��Ʈ�� ���ε� �� �� �����ϴ�.");
		return;
	}
}

void TCP::ListenSocket()
{
	if (::listen(hSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		puts("ERROR: ���� ���·� ��ȯ�� �� �����ϴ�.");
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

	//Ŭ���̾�Ʈ ������ �޾Ƶ��̰� ���ο� ���� ����(����)
	while ((hClient = ::accept(hSocket,
		(SOCKADDR*)&clientaddr,
		&nAddrLen)) != INVALID_SOCKET)
	{
		if (AddUser(hClient) == FALSE)
		{
			puts("ERROR : �� �̻� Ŭ���̾�Ʈ ������ ó���� �� �����ϴ�.");
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
	puts("*** ä�� ������ �����մϴ�. ***"); 
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
		puts("ERROR: ������ ������ �� �����ϴ�.");
		return;
	}

	//ä�� �޽��� ���� ������ ����
	DWORD dwThreadID = 0;
	HANDLE hThread = ::CreateThread(NULL,	//���ȼӼ� ���
		0,					//���� �޸𸮴� �⺻ũ��(1MB)
		ThreadReceive,		//������� ������ �Լ��̸�
		(LPVOID)hSocket,	//���� �ڵ��� �Ű������� �ѱ�
		0,					//���� �÷��״� �⺻�� ���
		&dwThreadID);		//������ ������ID�� ����� �����ּ�
	::CloseHandle(hThread);

}

void TCP::SendToServer()
{
	char szBuffer[128] = { 0 };
	while (1)
	{
		//����ڷκ��� ���ڿ��� �Է� �޴´�.
		gets_s(szBuffer);
		if (strcmp(szBuffer, "EXIT") == 0)		break;

		Json j;
		std::string comment =j.WriteJson(szBuffer);
		//����ڰ� �Է��� ���ڿ��� ������ �����Ѵ�.
		::send(hSocket, comment.c_str(), comment.size(), 0);
		//�����κ��� ��� ���� ���ڿ��� ���� ���� �޽����� �����Ѵ�.
		memset(szBuffer, 0, sizeof(szBuffer));
		::recv(hSocket, szBuffer, sizeof(szBuffer), 0);
		printf("From server: %s\n", szBuffer);
	}
}

void TCP::CloseSocket()
{
	::closesocket(hSocket);
	// CriticalSection ����
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
	// Critical Section ���� ��, client Socket�� List�� �����Ѵ�.
	::EnterCriticalSection(&g_cs);
	g_listClient.push_back(hSocket);
	::LeaveCriticalSection(&g_cs);

	return TRUE;
}


