/**
 * @file   UdpThread.cpp
 * @brief  UdpThreadクラスの実装
 * @author kotani
 */
#include "UdpThread.h"
#include "DxInput\KeyBoard\KeyDevice.h"
#include <mutex>

namespace
{
	std::mutex g_Mutex;
};

UdpThread::UdpThread(LPCTSTR _ip, int _port) :
m_GameIsEnd(false),
m_IsUpdate(false)
{
	if (WSAStartup(MAKEWORD(2, 0), &m_WsaData) != 0)
	{
		OutputDebugString("WSAStartupに失敗しました。");
	}  

	m_Socket = socket(AF_INET,SOCK_DGRAM,0);
	if (m_Socket == INVALID_SOCKET)
	{
		WSACleanup();
		OutputDebugString("ソケットの生成に失敗しました。");
	}

	FD_SET(m_Socket, &m_ReadFds);

	m_TimeOut.tv_sec = 2;
	m_TimeOut.tv_usec = 0;

	m_ServerAdd.sin_family = AF_INET;
	m_ServerAdd.sin_port = htons(_port);
	m_ServerAdd.sin_addr.s_addr = inet_addr(_ip);

	m_pThread = new std::thread(&UdpThread::MainLoop,this);
	memset(&m_RecvData, 0, sizeof(m_RecvData));
	memset(&m_SendData, 0, sizeof(m_SendData));
}


UdpThread::~UdpThread()
{
	m_GameIsEnd = true;
	m_pThread->join();
	delete m_pThread;
	m_pThread = NULL;

	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
	WSACleanup();
}


//----------------------------------------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------------------------------------

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

			m_SendData.KeyCommand[KEY_UP]	 = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_W];
			m_SendData.KeyCommand[KEY_LEFT]  = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_A];
			m_SendData.KeyCommand[KEY_RIGHT] = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_D];
			m_SendData.KeyCommand[KEY_DOWN]  = SINGLETON_INSTANCE(Lib::KeyDevice).GetKeyState()[DIK_S];

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
	memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));
	m_SendData.PlayerId = 1;
	sendto(m_Socket, reinterpret_cast<char*>(&m_SendData), sizeof(SendData), 0, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));
}

void UdpThread::Recv()
{
	memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));

	int len = (int)sizeof(sockaddr_in);
	int selectResult = select(m_Socket + 1, &m_Fds, NULL, NULL, &m_TimeOut);

	if (FD_ISSET(m_Socket, &m_Fds)) 
	{
		/* sock1からデータを受信して表示します */
		recvfrom(m_Socket, reinterpret_cast<char*>(&m_RecvData), sizeof(RecvData), 0, (sockaddr*)&m_ServerAdd, &len);
		m_IsUpdate = true;
	}
}
