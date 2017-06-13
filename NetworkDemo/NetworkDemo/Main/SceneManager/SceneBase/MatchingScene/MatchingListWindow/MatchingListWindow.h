/**
 * @file   MatchingListWindow.h
 * @brief  MatchingListWindowクラスのヘッダファイル
 * @author kotani
 */
#ifndef MATCHINGLISTWINDOW_H
#define MATCHINGLISTWINDOW_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <thread>
#include <winsock2.h>

class MatchingListWindow
{
public:
	/**
	 * コンストラクタ
 	 */
	MatchingListWindow();
	~MatchingListWindow();
	
	/**
	 * メイン制御
	 */
	void MainLoop();

private:
	struct PLAYER_DATA
	{
		int32_t	Id;
		bool IsLoadMap;
		bool IsJoin;
	};

	PLAYER_DATA  m_PlayerData;

	WSADATA		 m_WsaData;
	SOCKET		 m_Socket;
	sockaddr_in  m_ServerAdd;
	bool		 m_GameIsEnd;
	std::thread* m_pThread;
	timeval		 m_TimeOut;
	fd_set		 m_Fds, m_ReadFds;

};


#endif
