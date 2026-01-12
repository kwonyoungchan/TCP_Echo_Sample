#include <stdio.h>
#include "TCP.h"
#include "UDP.h"
int main()
{
	TCP tdp;
	char ip[10] = "127.0.0.1";

	UDP udp;
	udp.Init();
	printf("골라!!! 1 : Server, 2 : Client\n");

	char buf[10];
	gets_s(buf);
	unsigned int input = 0;
	input = atoi(buf);
	switch (input)
	{
	case 1:
		udp.setSockAddrforServer(7777);
		udp.ReciveLoop();
		break;
	case 2:
		udp.setSockAddrforClient(ip,7777);
		udp.SendLoop();
		break;
	}
	//if (tcp.ReadIpAddress(ip) == 0) printf("잘못된 IP 주소 입력. 프로그램을 종료합니다.");
	//if(tcp.ReadPort(25000) == 0)  printf("잘못된 port 입력. 프로그램을 종료합니다.");;
	//// 1. TCP 종류를 선택하게 한다.
	//printf("실행 할 TCP 를 선택하시오. 1: Server , 2 : Client\n");

	//char buf[10];
	//unsigned int input = 0;
	//gets_s(buf);
	//input = atoi(buf);
	//switch (input)
	//{
	//case 1:	// TCP Server 로직
	//	tcp.StartWSA();
	//	tcp.CreateTCPSocket();
	//	tcp.BindPort();
	//	tcp.ListenSocket();
	//	tcp.AccepetAndCommunication();
	//	tcp.EndWSA();
	//	break;
	//case 2:	// TCP Client 로직
	//	tcp.StartWSA();
	//	tcp.CreateTCPSocket();
	//	tcp.ConnectToServer();
	//	tcp.SendToServer();
	//	tcp.CloseSocket();
	//	tcp.EndWSA();
	//	break;
	//default:
	//	printf("잘못된 입력\n");
	//}

	return TRUE;
}