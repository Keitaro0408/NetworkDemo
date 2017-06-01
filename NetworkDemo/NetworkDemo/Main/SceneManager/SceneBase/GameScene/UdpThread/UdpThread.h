/**
 * @file   UdpThread.h
 * @brief  UdpThreadクラスのヘッダファイル
 * @author kotani
 */
#ifndef UDPTHREAD_H
#define UDPTHREAD_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <thread>
#include <winsock2.h>
#include <vector>
#include <DxInput\KeyBoard\KeyDevice.h>

class UdpThread
{
public:
	enum KEY_COMMAND
	{
		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,
		KEY_MAX
	};

	struct PlayerData
	{
		int32_t	Id;
		float	PosX, PosY;
		float	RectX, RectY;
	};

	struct RecvData
	{
		PlayerData PlayerData;
	};

	struct SendData
	{
		int32_t		  PlayerId;
		Lib::KEYSTATE KeyCommand[KEY_MAX];
	};

	/**
	 * コンストラクタ
	 */
	UdpThread(LPCTSTR _ip, int _port);
	
	/**
	 * デストラクタ
	 */
	~UdpThread();

	/**
	 * メイン制御
	 */
	void MainLoop();

	inline RecvData GetRecvData()
	{
		return m_RecvData;
	}

	/**
	 * ゲーム終了
	 */
	inline void GameEnd()
	{
		m_GameIsEnd = true;
	};

private:
	/**
	 * 送信
	 */
	void Send();

	/**
	 * 受信
	 */
	void Recv();
	
	WSADATA		 m_WsaData;
	SOCKET		 m_Socket;
	sockaddr_in  m_ServerAdd;
	bool		 m_GameIsEnd;
	std::thread* m_pThread;
	timeval		 m_TimeOut;
	fd_set		 m_Fds, m_ReadFds;

	SendData	 m_SendData;
	RecvData	 m_RecvData;

};


#endif
