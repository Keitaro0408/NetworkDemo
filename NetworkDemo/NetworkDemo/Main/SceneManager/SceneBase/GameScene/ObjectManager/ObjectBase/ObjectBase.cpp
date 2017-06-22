#include "ObjectBase.h"
#include "Dx11/DX11Manager.h"
#include "Window/Window.h"
#include "Helper/Helper.h"


void ObjectBase::InitVertex(const D3DXVECTOR2& _pos,
	const D3DXVECTOR2& _rect,
	LPCTSTR _animFile,
	LPCTSTR _animName,
	Vertex* _vertex)
{
	_vertex->Pos = _pos;
	_vertex->Rect = _rect;
	_vertex->pUvController = new Lib::AnimUvController();
	_vertex->pUvController->LoadAnimation(_animFile, _animName);
	_vertex->pVertex = new Lib::Vertex2D(
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDevice(),
		SINGLETON_INSTANCE(Lib::DX11Manager).GetDeviceContext(),
		SINGLETON_INSTANCE(Lib::Window).GetWindowSize());
	_vertex->pVertex->Init(&_vertex->Rect, _vertex->pUvController->GetUV());
}

void ObjectBase::ReleaseVertex(Vertex* _vertex)
{
	if (_vertex->pVertex != NULL)
	{
		_vertex->pVertex->Release();
		Lib::SafeDelete(_vertex->pVertex);
	}
	Lib::SafeDelete(_vertex->pVertex);

	Lib::SafeDelete(_vertex->pUvController);
}
