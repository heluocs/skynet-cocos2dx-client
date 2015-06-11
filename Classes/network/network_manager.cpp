#include "network_manager.h"
#include "message_dispatcher.h"
#include "socket_manager.h"
#include "message/module.pb.h"
#include "../login/LoginController.h"

NetworkManager* NetworkManager::m_instance = nullptr;

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{

}

NetworkManager* NetworkManager::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new NetworkManager();
	}

	return m_instance;
}

void NetworkManager::init()
{
	const char* ip = "182.92.184.3";
	const int port = 8888;

	SocketManager::getInstance()->connect(ip, port);
	SocketManager::getInstance()->startThread();

	MessageDispatcher::getInstance()->registController(ModuleID::MODULE_LOGIN, LoginController::getInstance());
}