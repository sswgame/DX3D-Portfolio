#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
private:
	Vec3 m_vRelativePos;
	Vec3 m_vRelativeScale;
	Vec3 m_vRelativeRot;     // 축별 회전량

	Vec3 m_arrRelativeDir[static_cast<UINT>(DIR_TYPE::END)]; // 상대 방향
	Vec3 m_arrWorldDir[static_cast<UINT>(DIR_TYPE::END)];    // 최종 월드상에서의 방향

	Matrix m_matRelativeRot;
	Matrix m_matWorld;     // 위치변환 정보 행렬
	Matrix m_matWorldInv;  // 월드 역행렬

	bool m_bIgnoreParentScale;
	bool m_bIgnoreParent;

public:
	void SetRelativePos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
	void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }

	void SetRelativeScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
	void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }

	void SetRelativeRotation(const Vec3 _vRot) { m_vRelativeRot = _vRot; }
	void SetRelativeRotation(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); }

	const Vec3& GetRelativePos() { return m_vRelativePos; }
	const Vec3& GetRelativeScale() { return m_vRelativeScale; }
	const Vec3& GetRelativeRotation() { return m_vRelativeRot; };

	const Vec3& GetRelativeDir(DIR_TYPE _eType) { return m_arrRelativeDir[static_cast<UINT>(_eType)]; }
	const Vec3& GetRelativeRightDir() { return m_arrRelativeDir[static_cast<UINT>(DIR_TYPE::RIGHT)]; }
	const Vec3& GetRelativeUpDir() { return m_arrRelativeDir[static_cast<UINT>(DIR_TYPE::UP)]; }
	const Vec3& GetRelativeFrontDir() { return m_arrRelativeDir[static_cast<UINT>(DIR_TYPE::FRONT)]; }

	const Vec3& GetWorldDir(DIR_TYPE _eType) { return m_arrWorldDir[static_cast<UINT>(_eType)]; }
	const Vec3& GetWorldRightDir() { return m_arrWorldDir[static_cast<UINT>(DIR_TYPE::RIGHT)]; }
	const Vec3& GetWorldUpDir() { return m_arrWorldDir[static_cast<UINT>(DIR_TYPE::UP)]; }
	const Vec3& GetWorldFrontDir() { return m_arrWorldDir[static_cast<UINT>(DIR_TYPE::FRONT)]; }

	Vec3   GetWorldPos() { return m_matWorld.Translation(); }
	Vec3   GetWorldScale();
	Matrix GetWorldRotation();
	bool   GetIgnorantParentScale() { return m_bIgnoreParentScale; }
	bool   GetIgnoreParent() { return m_bIgnoreParent; }

	const Matrix& GetWorldMat() { return m_matWorld; }
	const Matrix& GetWorldInvMat() { return m_matWorldInv; }

	void SetIgnoreParentScale(bool _bSet) { m_bIgnoreParentScale = _bSet; }
	void SetIgnoreParent(bool _bSet) { m_bIgnoreParent = _bSet; }

	void UpdateData() override;
	void finalupdate() override;
	void finalupdate_module() override;

	void active() override;
	void deactive() override;

public:
	void SaveToScene(FILE* _pFile) override;
	void LoadFromScene(FILE* _pFile) override;

	CLONE(CTransform)

public:
	void Serialize(YAML::Emitter& emitter) override;
	void Deserialize(const YAML::Node& node) override;

public:
	CTransform();
	virtual ~CTransform();
};
