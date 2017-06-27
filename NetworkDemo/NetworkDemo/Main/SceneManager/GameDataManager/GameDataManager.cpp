/**
 * @file   GameDataManager.cpp
 * @brief  GameDataManagerクラスの実装
 * @author kotani
 */
#include "GameDataManager.h"
#include <stdio.h>
#include <stdlib.h>


GameDataManager::GameDataManager()
{
	FILE* fp;
	fopen_s(&fp,"ip.txt","r");
	fscanf_s(fp, "%s", &m_Ip, _countof(m_Ip));
	fclose(fp);
}


GameDataManager::~GameDataManager()
{
}
