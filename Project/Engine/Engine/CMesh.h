#pragma once
#include "CRes.h"
#include "CFBXLoader.h"

class CStructuredBuffer;

struct tIndexInfo
{
	ComPtr<ID3D11Buffer> pIB;
	D3D11_BUFFER_DESC    tIBDesc;
	UINT                 iIdxCount;
	void*                pIdxSysMem;
};

class CMesh : public CRes
{
private:
	ComPtr<ID3D11Buffer> m_VB;
	D3D11_BUFFER_DESC    m_tVBDesc;
	UINT                 m_iVtxCount;
	void*                m_pVtxSys;

	// �ϳ��� ���ؽ����ۿ� �������� �ε������۰� ����
	vector<tIndexInfo> m_vecIdxInfo;

	// Animation3D ����
	vector<tMTAnimClip> m_vecAnimClip;
	vector<tMTBone>     m_vecBones;

	CStructuredBuffer* m_pBoneFrameData; // ��ü �� ������ ����(ũ��, �̵�, ȸ��) (������ ������ŭ)
	CStructuredBuffer* m_pBoneOffset;    // �� ���� offset ���(�� ���� ��ġ�� �ǵ����� ���) (1�� ¥��)

public:
	void render(UINT _iSubset);
	void render_particle(UINT _iCount);

	void UpdateData(UINT _iSubset);
	void UpdateVertexBuffer_Dynamic();

public:
	int           Create(const void* _pVtxSys, UINT _iVtxCount, const void* _pIdxSys, UINT _iIdxCount);
	static CMesh* CreateFromContainer(CFBXLoader& _loader, int ContainerIdx);;
	int           CreateBuffer_Dynamic(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount);

public:
	int                        GetVtxCount() const { return m_iVtxCount; }
	Vtx*                       GetVtxSysMem() const { return static_cast<Vtx*>(m_pVtxSys); }
	UINT                       GetSubsetCount() const { return static_cast<UINT>(m_vecIdxInfo.size()); }
	const tIndexInfo&          GetIndexInfo(int index) const { return m_vecIdxInfo[index]; }
	const vector<tMTBone>*     GetBones() const { return &m_vecBones; }
	UINT                       GetBoneCount() const { return static_cast<UINT>(m_vecBones.size()); }
	const vector<tMTAnimClip>* GetAnimClip() const { return &m_vecAnimClip; }
	bool                       IsAnimMesh() const { return !m_vecAnimClip.empty(); }

	CStructuredBuffer* GetBoneFrameDataBuffer() const { return m_pBoneFrameData; } // ��ü �� ������ ����
	CStructuredBuffer* GetBoneOffsetBuffer() const { return m_pBoneOffset; }       // �� ���� offset ���	

public:
	int Save(const wstring& _strFilePath) override;
	int Load(const wstring& _strFilePath) override;

public:
	CMesh();
	virtual ~CMesh();
};
