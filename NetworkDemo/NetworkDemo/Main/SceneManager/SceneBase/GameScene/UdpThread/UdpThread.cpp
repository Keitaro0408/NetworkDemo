﻿/**
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

	m_TimeOut.tv_sec = 2;
	m_TimeOut.tv_usec = 0;

	m_ServerAdd.sin_family = AF_INET;
	m_ServerAdd.sin_port = htons(_port);
	m_ServerAdd.sin_addr.s_addr = inet_addr(_ip);

	m_pThread = new std::thread(&UdpThread::MainLoop, this);
	memset(&m_SendData, 0, sizeof(m_SendData));

}

void UdpThread::MainLoop()
{
	DWORD SyncOld = timeGetTime();	//	システム時間を取得
	DWORD SyncNow;
	timeBeginPeriod(1);

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

			Send();
			Recv();
			SyncOld = SyncNow;
		}
	}
	timeEndPeriod(1);
}


//----------------------------------------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------------------------------------

void UdpThread::Send()
{
	m_SendData.PlayerId = SINGLETON_INSTANCE(NetworkDataManager).GetId();
	sendto(m_Socket, reinterpret_cast<char*>(&m_SendData), sizeof(SendData), 0, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));
}

void UdpThread::Recv()
{
	memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));

	int len = (int)sizeof(sockaddr_in);
	int selectResult = select(m_Socket + 1, &m_Fds, NULL, NULL, &m_TimeOut);
	int playerNum = SINGLETON_INSTANCE(NetworkDataManager).GetPlayerNum();

	if (FD_ISSET(m_Socket, &m_Fds)) 
	{
		/* sock1からデータを受信して表示します */
		recvfrom(m_Socket, reinterpret_cast<char*>(m_pPlayerData), sizeof(PlayerData)*playerNum, 0, (sockaddr*)&m_ServerAdd, &len);
		m_IsUpdate = true;
	}
}
