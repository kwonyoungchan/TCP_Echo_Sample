#include "TCP.h"
#include <stdio.h>
#include "Json.h"
#include <string>
void TCP::StartWSA()
{
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: ������ �ʱ�ȭ �� �� �����ϴ�.");
		return;
	}
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

	//Ŭ���̾�Ʈ ������ �޾Ƶ��̰� ���ο� ���� ����(����)
	while ((hClient = ::accept(hSocket,
		(SOCKADDR*)&clientaddr,
		&nAddrLen)) != INVALID_SOCKET)
	{
		puts("�� Ŭ���̾�Ʈ�� ����Ǿ����ϴ�."); fflush(stdout);
		//Ŭ���̾�Ʈ�κ��� ���ڿ��� ������.
		while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0)) > 0)
		{
			// ���⿡ Json �Ľ��ؼ� ���� �����Ѵ�.
			//������ ���ڿ��� �״�� ��������.
			::send(hClient, szBuffer, sizeof(szBuffer), 0);
			Json j;
			puts(j.ReadComment(szBuffer).c_str()); fflush(stdout);
			memset(szBuffer, 0, sizeof(szBuffer));
		}

		//Ŭ���̾�Ʈ�� ������ ������.
		::shutdown(hClient, SD_BOTH);
		::closesocket(hClient);
		puts("Ŭ���̾�Ʈ ������ ������ϴ�."); fflush(stdout);
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
		puts("ERROR: ������ ������ �� �����ϴ�.");
		return;
	}
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
