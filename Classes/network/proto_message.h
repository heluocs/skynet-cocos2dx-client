#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//低16位消息号  高16位模块号

enum ProtoMessage
{
	//Login
	MSG_PLAYER_LOGIN_C2S		= 0x00010001,	//玩家登陆请求
	MSG_PLAYER_LOGIN_S2C		= 0x00010002,	//玩家登陆响应
};

#endif