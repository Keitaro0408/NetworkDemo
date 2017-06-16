#ifndef MATCHINGSCENE_H
#define MATCHINGSCENE_H
#include "../SceneBase.h"
#include <thread>
#include <string>


class WaitScene : public SceneBase
{
public:
	WaitScene();
	~WaitScene();

	/**
	 * ÉÅÉCÉìêßå‰
	 */
	void WaitThread();

	SceneID Update() override;
	void Draw() override;

private:
	struct SendData
	{
		bool IsOk;
		bool IsMapLoad;
	};

	struct RecvData
	{
		int32_t Id;
		bool IsStart;
	};

	WSADATA						  m_WsaData;
	SOCKET						  m_Socket;
	sockaddr_in					  m_ServerAdd;
	bool						  m_IsWaitEnd;
	std::thread*				  m_pWaitThread;
	timeval						  m_TimeOut;
	fd_set						  m_Fds, m_ReadFds;
	SendData					  m_SendData;
	RecvData					  m_RecvData;

};


#endif
