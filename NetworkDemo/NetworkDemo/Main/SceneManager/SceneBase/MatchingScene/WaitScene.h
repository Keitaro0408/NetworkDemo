#ifndef MATCHINGSCENE_H
#define MATCHINGSCENE_H
#include "../SceneBase.h"
#include <string>
#include <thread>
#include <winsock2.h>


class WaitScene : public SceneBase
{
public:
	WaitScene();
	~WaitScene();

	/**
	 * ÉÅÉCÉìêßå‰
	 */
	void MainLoop();

	SceneID Update() override;
	void Draw() override;

private:
	struct SendData
	{
		bool IsOk;
		bool IsJoin;
	};

	WSADATA						  m_WsaData;
	SOCKET						  m_Socket;
	sockaddr_in					  m_ServerAdd;
	bool						  m_GameIsEnd;
	std::thread*				  m_pThread;
	timeval						  m_TimeOut;
	fd_set						  m_Fds, m_ReadFds;


};


#endif
