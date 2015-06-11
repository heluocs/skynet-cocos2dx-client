#include "LoginController.h"
#include "network/proto_message.h"
#include "network/message/login.pb.h"

LoginController* LoginController::m_controller = nullptr;

LoginController* LoginController::getInstance()
{
	if (m_controller == nullptr)
	{
		m_controller = new LoginController();
	}

	return m_controller;
}

LoginController::LoginController()
{

}

void LoginController::onMsgResponse(const unsigned int opcode, const char* msg, const unsigned int size)
{
	switch (opcode)
	{
	case LoginOpcode::ID_S2C_PLAYER_LOGIN_RESPONSE:
	{
		CMsgPlayerLoginResponse response;
		response.ParseFromArray(msg, size);
		printf("-----entityId: %s -------", response.entityid());
		break;
	}
	default:
		break;
	}
}