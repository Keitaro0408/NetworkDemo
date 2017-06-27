#ifndef MATCHINGSCENE_H
#define MATCHINGSCENE_H
#include "../SceneBase.h"
#include <thread>
#include <string>
#include <vector>


class WaitScene : public SceneBase
{
public:
	WaitScene();
	~WaitScene();

	/**
	 * メイン制御
	 */
	void WaitThread();

	SceneID Update() override;
	void Draw() override;

private:
	struct SendData
	{
		unsigned char Id;
		bool IsOk;
		bool IsMapLoad;
	};

	struct RecvData
	{
		int32_t PlayerNum; //!< プレイヤーの数
		unsigned char Id;
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
