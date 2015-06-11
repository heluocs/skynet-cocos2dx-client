#include "message_dispatcher.h"
#include "base_controller.h"

MessageDispatcher* MessageDispatcher::m_dispatcher = nullptr;

MessageDispatcher* MessageDispatcher::getInstance()
{
	if (m_dispatcher == nullptr)
	{
		m_dispatcher = new MessageDispatcher();
	}

	return m_dispatcher;
}

MessageDispatcher::MessageDispatcher()
{
	m_Controllers.clear();
}

void MessageDispatcher::registController(unsigned int moduleno, BaseController * controller)
{
	std::pair<std::map<unsigned int, BaseController*>::iterator, bool> result;
	result = m_Controllers.insert(std::pair<unsigned int, BaseController*>(moduleno, controller));
	if (result.second == false)
	{
		fprintf(stderr, "register module failed.");
	}
	else
	{
		printf("register module success.");
	}

}

void MessageDispatcher::dispatchMessage(const unsigned int opcode, const char * buf, const unsigned short size)
{
	unsigned int moduleno = opcode >> 16;
	unsigned int msgno = opcode & 0x0000FFFF;

	if (moduleno == 0)
	{
		return;
	}

	std::map<unsigned int, BaseController*>::iterator iter = m_Controllers.find(moduleno);
	if (iter != m_Controllers.end())
	{
		BaseController* pController = iter->second;
		if (pController != nullptr)
		{
			pController->onMsgResponse(opcode, buf, size);
		}
	}
	else
	{
		fprintf(stderr, "illegal message.");
	}
}
