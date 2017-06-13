/**
 * @file   MatchingListWindow.cpp
 * @brief  MatchingListWindowクラスの実装
 * @author kotani
 */
#include "MatchingListWindow.h"


MatchingListWindow::MatchingListWindow() :
m_GameIsEnd(false)
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

	m_TimeOut.tv_sec = 2;
	m_TimeOut.tv_usec = 0;

	m_ServerAdd.sin_family = AF_INET;
	m_ServerAdd.sin_port = htons(50000);
	m_ServerAdd.sin_addr.s_addr = inet_addr("49.250.217.198");

	m_pThread = new std::thread(&MatchingListWindow::MainLoop, this);
}

MatchingListWindow::~MatchingListWindow()
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

void MatchingListWindow::MainLoop()
{
	while (!m_GameIsEnd)
	{
		memcpy(&m_Fds, &m_ReadFds, sizeof(fd_set));
		sendto(m_Socket, reinterpret_cast<char*>(&m_SendData), sizeof(SendData), 0, (struct sockaddr *)&m_ServerAdd, sizeof(m_ServerAdd));

	}
}