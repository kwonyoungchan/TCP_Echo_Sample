#include <stdio.h>
#include "TCP.h"


int main()
{
	TCP tcp;
	char ip[10] = "127.0.0.1";
	if (tcp.ReadIpAddress(ip) == 0) printf("�߸��� IP �ּ� �Է�. ���α׷��� �����մϴ�.");
	if(tcp.ReadPort(25000) == 0)  printf("�߸��� port �Է�. ���α׷��� �����մϴ�.");;
	// 1. TCP ������ �����ϰ� �Ѵ�.
	printf("���� �� TCP �� �����Ͻÿ�. 1: Server , 2 : Client\n");

	char buf[10];
	unsigned int input = 0;
	gets_s(buf);
	input = atoi(buf);
	switch (input)
	{
	case 1:	// TCP Server ����
		tcp.StartWSA();
		tcp.CreateTCPSocket();
		tcp.BindPort();
		tcp.ListenSocket();
		tcp.AccepetAndCommunication();
		tcp.EndWSA();
		break;
	case 2:	// TCP Client ����
		tcp.StartWSA();
		tcp.CreateTCPSocket();
		tcp.ConnectToServer();
		tcp.SendToServer();
		tcp.CloseSocket();
		tcp.EndWSA();
		break;
	default:
		printf("�߸��� �Է�\n");
	}

	return TRUE;
}