

#include"Easy_TcpClient.hpp"
#include"TcpClient.h"

#include<iostream>
using namespace std;
//#pragma comment(lib,"ws2_32.lib")	//Windowsƽ̨��������ӿ⣨�˴����ڹ��������������м��룬����ſ�ע�ͣ�

int main()
{
	MessageData LoginData(CMD_LOGIN, "hello3");

	TcpClient client;
	client.InitSocket();
	client.Connect("127.0.0.1", 8080);
	client.SendData(&LoginData);
	while (client.IsConnecting())
	{
		client.OnRun();
	}
	client.Close();
	return 0;
}