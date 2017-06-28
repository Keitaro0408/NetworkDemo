/**
 * @file   NetworkDataManager.cpp
 * @brief  NetworkDataManagerクラスの実装
 * @author kotani
 */
#include "NetworkDataManager.h"
#include <stdio.h>
#include <stdlib.h>


NetworkDataManager::NetworkDataManager()
{
	FILE* fp;
	fopen_s(&fp,"ip.txt","r");
	fscanf_s(fp, "%s", &m_Ip, _countof(m_Ip));
	fclose(fp);
}


NetworkDataManager::~NetworkDataManager()
{
}
