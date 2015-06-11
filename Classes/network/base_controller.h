#ifndef _BASE_CONTROLLER_H_
#define _BASE_CONTROLLER_H_

class BaseController
{
protected:
	BaseController();
	~BaseController();

public:
	virtual void onMsgResponse(const unsigned int opcode, const char* msg, const unsigned int size) = 0;
};

#endif