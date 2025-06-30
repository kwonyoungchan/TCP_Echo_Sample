// TCP.h : TCP Server, Client ���� �ҽ��ڵ� 
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS	// �ֽ� C++ ǥ���� ����ϸ� C4996 ������ �߻��ϴµ� �ش� �ڵ� �Է��ϸ� �ذ�� 
#include <winsock2.h>
#pragma comment(lib, "ws2_32")



class TCP
{
public:

	TCP() {}
	~TCP() {} 

	// TCP ���� ���� ����
	void StartWSA();
	void CreateTCPSocket();			// 1. ���� ����
	void EndWSA();

	// TCP ���� �Լ� 
	void BindPort();				// 2. ���� ��Ʈ ���ε� 
	void ListenSocket();			// 3. ���� ��Ʈ ���� ���
	void AccepetAndCommunication();

	// TCP Ŭ���̾�Ʈ �Լ� 
	void ConnectToServer();			// 2. ���� ��Ʈ ����
	void SendToServer();			// 3. �������� ���� ����
	void CloseSocket();				// 4. ���� ��Ʈ ����(TCP���� �ݵ�� Ŭ���̾�Ʈ�� ���� ������ ������Ѿ� �Ѵ�.
		
public:
	// IP Adress, ��Ʈ ��ȣ �Է� �ް�, ����
	INT ReadIpAddress(char* ipAddress);
	INT ReadPort(int port);
	INT ReadPort(char* port);
private:
	WSADATA wsa = { 0 };
	SOCKET hSocket = NULL;
	SOCKADDR_IN	svraddr = { 0 };

private:
	char* _ipAddress = { 0 };
	unsigned int _port = 25000;

};

