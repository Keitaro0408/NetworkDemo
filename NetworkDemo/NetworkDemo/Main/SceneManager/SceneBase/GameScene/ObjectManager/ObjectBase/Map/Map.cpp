/**
 * @file   Map.cpp
 * @brief  Mapクラスの実装
 * @author kotani
 */
#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include "Texture/TextureManager.h"


Map::Map()
{
	MapLoad();

	InitVertex(
		D3DXVECTOR2(0,0),
		D3DXVECTOR2(CHIP_SIZE, CHIP_SIZE),
		"Resource/map.anim",
		"Ground",
		&m_Vertex);

	SINGLETON_INSTANCE(Lib::TextureManager).Load("Resource/mapGround.jpg",&m_TextureIndex);
	m_Vertex.pVertex->SetTexture(
		SINGLETON_INSTANCE(Lib::TextureManager).GetTexture(m_TextureIndex));

}

Map::~Map()
{
	SINGLETON_INSTANCE(Lib::TextureManager).ReleaseTexture(m_TextureIndex);
	ReleaseVertex(&m_Vertex);
}

void Map::MapLoad()
{
	FILE*  fp;
	fopen_s(&fp, "Resource/map.csv", "r");

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			fscanf_s(fp, "%d,", &m_MapChip[i][j], _countof(m_MapChip));
		}
	}

	fclose(fp);
}

void Map::Update()
{
}

void Map::Draw()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (m_MapChip[y][x] == CHIP_GROUND)
			{
				D3DXVECTOR2 pos;
				pos.x = static_cast<float>((x + 1) * CHIP_SIZE - CHIP_SIZE / 2);
				pos.y = static_cast<float>((y + 1) * CHIP_SIZE - CHIP_SIZE / 2);
				m_Vertex.pVertex->Draw(&pos, m_Vertex.pUvController->GetUV());
			}
		}
	}
}
