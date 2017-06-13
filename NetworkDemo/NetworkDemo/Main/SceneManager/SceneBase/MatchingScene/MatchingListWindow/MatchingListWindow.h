/**
 * @file   MatchingListWindow.h
 * @brief  MatchingListWindow�N���X�̃w�b�_�t�@�C��
 * @author kotani
 */
#ifndef MATCHINGLISTWINDOW_H
#define MATCHINGLISTWINDOW_H
#include <string>
#include <thread>
#include <winsock2.h>

class MatchingListWindow
{
public:
	/**
	 * �R���X�g���N�^
 	 */
	MatchingListWindow();
	~MatchingListWindow();
	
	/**
	 * ���C������
	 */
	void MainLoop();

private:
	std::string  m_playerData;
	WSADATA		 m_WsaData;
	SOCKET		 m_Socket;
	sockaddr_in  m_ServerAdd;
	bool		 m_GameIsEnd;
	std::thread* m_pThread;
	timeval		 m_TimeOut;
	fd_set		 m_Fds, m_ReadFds;

};


#endif
