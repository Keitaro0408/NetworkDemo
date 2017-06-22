/**
 * @file   ObjectManager.h
 * @brief  ObjectManager�N���X�̃w�b�_�t�@�C��
 * @author kotani
 */
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <vector>

class ObjectBase;
class Player;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Update();
	void Draw();

private:
	std::vector<ObjectBase*> m_pObjectBase;
	std::vector<Player*>     m_pPlayer;
	int						 m_PlayerTextureIndex;
};


#endif
