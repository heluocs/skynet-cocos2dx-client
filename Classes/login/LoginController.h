#ifndef _LOGIN_CONTROLLER_H_
#define _LOGIN_CONTROLLER_H_

#include "network/base_controller.h"

class LoginController : public BaseController
{
public:
	static LoginController* getInstance();

	void onMsgResponse(const unsigned int opcode, const char* msg, const unsigned int size);

private:
	LoginController();

	static LoginController* m_controller;
};

#endif