#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#define DATA_MAX	128

#define SOCKE_ERROR (-1)
#include<string.h>
enum MessageCmd
{
	CMD_LOGIN = 1,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};

class DataHeader
{
public:
	int datalen;
	int cmd;
};
class MessageData: public DataHeader
{
public:	
	MessageData(int cmd,const char *buf)
	{
		datalen = sizeof(data);
		this->cmd = cmd;
		memset(data, 0, sizeof(data));
		strcpy(data ,buf);
	}
	char data[DATA_MAX];
};
enum TcpStatus
{
	TCP_OK = 0,
	TCP_ERR 
};

#endif