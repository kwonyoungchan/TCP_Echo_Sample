// TCP.h : TCP Server, Client 예제 소스코드 
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS	// 최신 C++ 표준을 사용하면 C4996 오류가 발생하는데 해당 코드 입력하면 해결됨 
#include <winsock2.h>
#pragma comment(lib, "ws2_32")



class TCP
{
public:

	TCP() {}
	~TCP() {} 

	// TCP 소켓 공통 사항
	void StartWSA();
	void CreateTCPSocket();			// 1. 소켓 생성
	void EndWSA();

	// TCP 서버 함수 
	void BindPort();				// 2. 소켓 포트 바인드 
	void ListenSocket();			// 3. 소켓 포트 접속 대기
	void AccepetAndCommunication();

	// TCP 클라이언트 함수 
	void ConnectToServer();			// 2. 소켓 포트 연결
	void SendToServer();			// 3. 서버에게 소켓 전송
	void CloseSocket();				// 4. 소켓 포트 닫음(TCP에서 반드시 클라이언트가 소켓 연결을 종료시켜야 한다.
		
public:
	// IP Adress, 포트 번호 입력 받고, 정의
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

