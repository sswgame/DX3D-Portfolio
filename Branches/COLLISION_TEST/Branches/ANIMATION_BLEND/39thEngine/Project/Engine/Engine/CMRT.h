#pragma once
#include "CEntity.h"

class CMRT : public CEntity
{
private:
	Ptr<CTexture> m_arrRT[8];
	int           m_iRTCount;

	Ptr<CTexture> m_pDSTex;
	Vec4          m_arrClearColor[8];

	D3D11_VIEWPORT m_tViewPort;

public:
	void Create(int _iRTCount, Ptr<CTexture>* _ppTex, Ptr<CTexture> _pDSTex);

	const D3D11_VIEWPORT& GetViewPort() { return m_tViewPort; }


	void SetClearColor(int _iCount, Vec4* _pColor)
	{
		for (int i = 0; i < _iCount; ++i)
		{
			m_arrClearColor[i] = _pColor[i];
		}
	}

	void OMSet();
	void Clear();
	int  GetTargetCount() const { return m_iRTCount; }


public:
	CLONE_DISABLE(CMRT);

public:
	CMRT();
	~CMRT();
};
