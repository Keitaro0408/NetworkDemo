/**
 * @file   UdpThread.h
 * @brief  UdpThreadクラスのヘッダファイル
 * @author kotani
 */
#ifndef UDPTHREAD_H
#define UDPTHREAD_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <thread>
#include <vector>
#include <DxInput\KeyBoard\KeyDevice.h>
#include "Singleton.h"

class UdpThread
{
	friend Lib::Singleton<UdpThread>;
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
		bool	IsRight;
	};


	struct SendData
	{
		int32_t		  PlayerId;
		Lib::KEYSTATE KeyCommand[KEY_MAX];
	};


	void Init(LPCTSTR _ip, int _port);

	/**
	 * メイン制御
	 */
	void MainLoop();

	inline PlayerData* GetRecvData()
	{
		return m_pPlayerData;
	}

	/**
	 * ゲーム終了
	 */
	inline void GameEnd()
	{
		m_GameIsEnd = true;
	};

	inline bool GetIsUpdate()
	{
		return m_IsUpdate;
	}

	inline void SetIsUpdate(bool _isUpdate)
	{
		m_IsUpdate = _isUpdate;
	}

private:
	/**
	* コンストラクタ
	*/
	UdpThread();

	/**
	* デストラクタ
	*/
	~UdpThread();

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
	bool		 m_IsUpdate; // 座標が更新されたか？
	std::thread* m_pThread;
	timeval		 m_TimeOut;
	fd_set		 m_Fds, m_ReadFds;

	SendData	 m_SendData;
	PlayerData*  m_pPlayerData;

};


#endif
