/**
 * @file   UdpThread.cpp
 * @brief  UdpThreadクラスの実装
 * @author kotani
 */
#include "UdpThread.h"
#include "DxInput\KeyBoard\KeyDevice.h"
#include <mutex>
#include "../../../NetworkDataManager/NetworkDataManager.h"

namespace
{
	std::mutex g_Mutex;
};

UdpThread::UdpThread() :
m_GameIsEnd(false),
m_IsUpdate(false)
{
}


UdpThread::~UdpThread()
{
	m_GameIsEnd = true;
	m_pThread->join();
	delete m_pThread;
	m_pThread = NULL;

	delete[] m_pPlayerData;

	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
	WSACleanup();
}


//----------------------------------------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------------------------------------

void UdpThread::Init(LPCTSTR _ip, int _port)
{
	m_TimeOut.tv_sec = 2;
	m_TimeOut.tv_usec = 0;

	m_ServerAdd.sin_family = AF_INET;
	m_ServerAdd.sin_port = htons(SINGLETON_INSTANCE(NetworkDataManager).GetPort());
	m_ServerAdd.sin_addr.s_addr = INADDR_ANY;

	if (WSAStartup(MAKEWORD(2, 0), &m_WsaData) != 0)
	{
		OutputDebugString("WSAStartupに失敗しました。");
	}

	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		WSACleanup();
		OutputDebugString("ソケットの生成に失敗しました。");
	}

	int r = bind(m_Socket, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));
	if (r == -1)
	{
		OutputDebugString("bindに失敗\n。");
	}

	m_ServerAdd.sin_addr.s_addr = inet_addr(_ip);
	m_ServerAdd.sin_port = htons(PORT);

	FD_SET(m_Socket, &m_ReadFds);

	int playerNum = SINGLETON_INSTANCE(NetworkDataManager).GetPlayerNum();
	m_pPlayerData = new PlayerData[playerNum];
	for (int i = 0; i < playerNum; i++)
	{
		m_pPlayerData[i].Id = 0;
		m_pPlayerData[i].PosX = 0.f;
		m_pPlayerData[i].PosY = 0.f;
		m_pPlayerData[i].IsRight = true;
		for (int j = 0; j < 3; j++)
		{
			m_pPlayerData[i].bulletData[j].IsEnable = false;
			m_pPlayerData[i].bulletData[j].IsRight= false;
			m_pPlayerData[i].bulletData[j].PosX= 0.f;
			m_pPlayerData[i].bulletData[j].PosY = 0.f;
		}
	}

	m_pThread = new std::thread(&UdpThread::MainLoop, this);
	memset(&m_SendData, 0, sizeof(m_SendData));

}

void UdpThread::MainLoop()
{

	std::thread RecvThread(&UdpThread::Recv, this);
	std::thread SendThread(&UdpThread::Send, this);
	RecvThread.join();
	SendThread.join();
}


//----------------------------------------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------------------------------------

void UdpThread::Send()
{
	timeBeginPeriod(1);

	DWORD SyncOld = timeGetTime();	//	システム時間を取得
	DWORD SyncNow;
	
	while (!m_GameIsEnd)
	{
		Sleep(1);
		SyncNow = timeGetTime();
		if (SyncNow - SyncOld >= 1000 / 60) //	1秒間に60回この中に入る
		{
			m_SendData.KeyCommand[KEY_UP] = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_UPARROW];
			m_SendData.KeyCommand[KEY_LEFT] = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_LEFTARROW];
			m_SendData.KeyCommand[KEY_RIGHT] = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_RIGHTARROW];
			m_SendData.KeyCommand[KEY_FIRE] = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_Z];
			if (m_SendData.KeyCommand[KEY_UP] != Lib::KEY_OFF ||
				m_SendData.KeyCommand[KEY_LEFT] != Lib::KEY_OFF ||
				m_SendData.KeyCommand[KEY_RIGHT] != Lib::KEY_OFF ||
				m_SendData.KeyCommand[KEY_FIRE] != Lib::KEY_OFF)
			{
				m_SendData.PlayerId = SINGLETON_INSTANCE(NetworkDataManager).GetId();
				sendto(m_Socket, reinterpret_cast<char*>(&m_SendData), sizeof(SendData), 0, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));
			}
			SyncOld = SyncNow;
		}
	}
}

void UdpThread::Recv()
{
	int len = (int)sizeof(sockaddr_in);
	int playerNum = SINGLETON_INSTANCE(NetworkDataManager).GetPlayerNum();
	while (!m_GameIsEnd)
	{
		memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));

		int selectResult = select(m_Socket + 1, &m_Fds, NULL, NULL, &m_TimeOut);

		if (FD_ISSET(m_Socket, &m_Fds)) 
		{
			/* sock1からデータを受信して表示します */
			recvfrom(m_Socket, reinterpret_cast<char*>(m_pPlayerData), sizeof(PlayerData)*playerNum, 0, (sockaddr*)&m_ServerAdd, &len);
			m_IsUpdate = true;
		}
	}
}
