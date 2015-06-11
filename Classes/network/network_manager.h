#ifndef _NETWORK_MANAGER_H_
#define _NETWORK_MANAGER_H_

class NetworkManager
{
public:
	static NetworkManager* getInstance();
	~NetworkManager();

	void init();

private:
	NetworkManager();
	static NetworkManager* m_instance;
};

#endif