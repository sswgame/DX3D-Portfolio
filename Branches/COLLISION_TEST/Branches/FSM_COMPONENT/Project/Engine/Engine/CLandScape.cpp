#include "pch.h"
#include "CLandScape.h"

#include "CTransform.h"

CLandScape::CLandScape()
	:
	CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
  , m_iXFaceCount(0)
  , m_iZFaceCount(0) {}

CLandScape::~CLandScape() {}

void CLandScape::finalupdate() {}

void CLandScape::render()
{
	Transform()->UpdateData();
	GetSharedMaterial()->UpdateData();
	GetMesh()->render();
}
