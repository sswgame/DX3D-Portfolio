#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer(CB_TYPE _eType)
	: m_Desc{}
	, m_eCBType(_eType) {}

CConstBuffer::~CConstBuffer() {}

void CConstBuffer::UpdateData()
{
	CONTEXT->VSSetConstantBuffers(static_cast<UINT>(m_eCBType), 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers(static_cast<UINT>(m_eCBType), 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers(static_cast<UINT>(m_eCBType), 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers(static_cast<UINT>(m_eCBType), 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers(static_cast<UINT>(m_eCBType), 1, m_CB.GetAddressOf());
}

void CConstBuffer::UpdateData_CS()
{
	CONTEXT->CSSetConstantBuffers(static_cast<UINT>(m_eCBType), 1, m_CB.GetAddressOf());
}

int CConstBuffer::Create(UINT _iBufferSize)
{
	m_Desc.ByteWidth = _iBufferSize;

	// ���� ���� ���Ŀ���, ������ ������ ���� �� �� �ִ� �ɼ�
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage          = D3D11_USAGE_DYNAMIC;

	// ������ �����ϴ� ������ ���� ���� �˸�
	m_Desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.MiscFlags           = 0;
	m_Desc.StructureByteStride = 0;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		assert(nullptr);
		return E_FAIL;
	}

	return S_OK;
}

void CConstBuffer::SetData(const void* _pData, UINT _iSize)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pData, _iSize);
	CONTEXT->Unmap(m_CB.Get(), 0);
}
