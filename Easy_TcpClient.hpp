#ifndef __EasyTcpClient_hpp__
#define __EasyTcpClient_hpp__

#ifdef _WIN32       //windows platform
	#define _CRT_SECURE_NO_WARNINGS
	#define WIN32_LEAN_AND_MEAN
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include<windows.h>
	#include<WinSock2.h>
	#pragma comment(lib,"ws2_32.lib")

#else               //uninx platform
	#define SOCKET unsigned int
	#include<stdio.h>
	#include<sys/types.h>
	#include<sys/socket.h>
	#include<arpa/inet.h>
	#include<unistd.h>
	#include<string.h>
	#include<stdlib.h>
	#include<errno.h>

#endif
#include<stdio.h>
#include<iostream>
#include"TcpClient.h"
using namespace std;

class TcpClient
{
	SOCKET _sock;
public:
	TcpClient()
	{
		_sock = INVALID_SOCKET;
	}

	virtual ~TcpClient()
	{

	}
	/* init socket */
	void InitSocket()
	{
		//startup windows net enviroment
			#define SOCKET unsigned int
#ifdef _WIN32
		WORD ver = MAKEWORD(2, 2);
		int recv_len = 0;
		WSADATA dat;
		WSAStartup(ver, &dat);	//start Windows net enviroment£¬windows apis can be called before cleanup.
#endif
		if (_sock != INVALID_SOCKET)
		{
			cout << "close old connection!" << endl;
			Close();
		}
		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_sock == INVALID_SOCKET)
		{
			cout << "error:Create socket failed!" << endl;
		}
	}
	/* create connection */
	int Connect(const char *ip,short port)
	{
		if (_sock == INVALID_SOCKET)
		{
			InitSocket();
		}
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr(ip);
#endif
		int ret = connect(_sock, (sockaddr *)&_sin, sizeof(sockaddr_in));
		if (SOCKET_ERROR == ret)
		{
			cout << "error:connect server failed!" << endl;
		}
		else
		{
			cout << "connect server success!" << endl;
		}
		return ret;
	}
	/* close socket */
	void Close()
	{
		if (_sock != INVALID_SOCKET)
		{
#ifdef _WIN32
			closesocket(_sock);
			WSACleanup();
#else
			close(_sock);
#endif
			_sock = INVALID_SOCKET;
		}
	}
	
	int OnRun()
	{
		fd_set fdReads;
		FD_ZERO(&fdReads);
		FD_SET(_sock, &fdReads);

		timeval t = { 1,0 };

		int ret = select(_sock, &fdReads, 0, 0, &t);
		if (ret < 0)
		{
			cout << "errot:select task exit!" << endl;
			return false;
		}
		if (FD_ISSET(_sock,&fdReads))
		{
			FD_CLR(_sock, &fdReads);
			if (-1 == RecvData())
			{
				cout << "RecvData error, task exit!" << endl;
				return false;
			}
		}
		return true;
	}
	/* receive data */
	int RecvData()
	{
		char RecvBuf[1024] = {};
		if (IsConnecting())
		{
			int ret = (int)recv(_sock, RecvBuf, sizeof(DataHeader), 0);
			DataHeader *header = (DataHeader *)RecvBuf;
			if (ret <= 0)
			{
				cout << "error:server clsoed!" << endl;
				return -1;
			}
			recv(_sock, RecvBuf + sizeof(DataHeader), header->datalen - sizeof(DataHeader), 0);

			HandleMessage(_sock, header);
		}
		return 0;
	}
	/* handle message */
	virtual void HandleMessage(SOCKET _sock, DataHeader * header)
	{
		switch (header->cmd)
		{
		case CMD_LOGIN_RESULT:
			cout << "receive login result order " << header->datalen << " byte from server" << endl;
			break;
		case CMD_LOGOUT_RESULT:
			cout << "receive logout result order" << header->datalen << " byte from server" << endl;
			break;
		case CMD_ERROR:
			cout << "receive error order!" << endl;
			break;
		default:
			cout << "receive <" << strlen((char *)header) << "> byte from server >" <<(char *) header << endl;
		}
	}
	/* send data */
	int SendData(DataHeader * head)
	{
		if (IsConnecting() && head)
		{
			int ret = send(_sock,(const char *) head, head->datalen, 0);
			if (ret <= 0)
			{
				cout << "error:send data failed!" << endl;
				return false;
			}
			else
			{
				cout << "send " << ret << " byte data to server" << endl;
				return true;
			}
		}
		return false;
	}
	bool IsConnecting()
	{
		return _sock != INVALID_SOCKET;
	}

private:

};

#endif