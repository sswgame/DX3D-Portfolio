#include "pch.h"
#include "CResMgr.h"


void CResMgr::init()
{
	InitSound();

	CreateEngineMesh();
	CreateEngineTexture();
	CreateEngineShader();
	CreateEngineMaterial();

	CreateEngineComputeShader();
}

void CResMgr::update()
{
	CSound::g_pFMOD->update();
}


void CResMgr::CreateEngineMesh()
{
	CMesh* pMesh = nullptr;

	vector<Vtx>  vecVtx;
	vector<UINT> vecIdx;

	Vtx v;

	// ==========
	// Point Mesh	
	// ==========
	v.vPos   = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV    = Vec2(0.f, 0.f);
	vecVtx.push_back(v);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"PointMesh", pMesh, true);
	vecVtx.clear();
	vecIdx.clear();


	// ========
	// RectMesh
	// 0 --- 1
	// |  \  |
	// 3 --- 2
	// ========
	v.vPos      = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor    = Vec4(1.f, 0.2f, 0.2f, 1.f);
	v.vUV       = Vec2(0.f, 0.f);
	v.vTangent  = Vec3(1.f, 0.f, 0.f);
	v.vNormal   = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);

	vecVtx.push_back(v);

	v.vPos      = Vec3(0.5f, 0.5f, 0.f);
	v.vColor    = Vec4(0.2f, 1.f, 0.2f, 1.f);
	v.vUV       = Vec2(1.f, 0.f);
	v.vTangent  = Vec3(1.f, 0.f, 0.f);
	v.vNormal   = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos      = Vec3(0.5f, -0.5f, 0.f);
	v.vColor    = Vec4(0.2f, 0.2f, 1.f, 1.f);
	v.vUV       = Vec2(1.f, 1.f);
	v.vTangent  = Vec3(1.f, 0.f, 0.f);
	v.vNormal   = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos      = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor    = Vec4(1.f, 0.2f, 0.2f, 1.f);
	v.vUV       = Vec2(0.f, 1.f);
	v.vTangent  = Vec3(1.f, 0.f, 0.f);
	v.vNormal   = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh, true);
	vecIdx.clear();

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh_LineStrip", pMesh, true);
	vecVtx.clear();
	vecIdx.clear();

	// ==========
	// CircleMesh
	// ==========
	v.vPos   = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV    = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT  iSliceCount = 40;
	float fRadius     = 0.5f;
	float fAngleStep  = XM_2PI / float(iSliceCount);

	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos   = Vec3(fRadius * cosf(fAngleStep * (float)i), fRadius * sinf(fAngleStep * (float)i), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV    = Vec2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
		vecVtx.push_back(v);
	}


	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh, true);
	vecIdx.clear();

	// CicleMesh_LineStrip
	for (UINT i = 1; i <= iSliceCount + 1; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_LineStrip", pMesh, true);
	vecVtx.clear();
	vecIdx.clear();


	// =========
	// Cube Mesh
	// =========
	Vtx arrCube[24] = {};
	// 윗면
	arrCube[0].vPos    = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor  = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV     = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos    = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor  = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV     = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos    = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor  = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV     = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos    = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor  = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV     = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// 아랫 면	
	arrCube[4].vPos    = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV     = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos    = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV     = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos    = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV     = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos    = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV     = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// 왼쪽 면
	arrCube[8].vPos    = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV     = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos    = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV     = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos    = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV     = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos    = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV     = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// 오른쪽 면
	arrCube[12].vPos    = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor  = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV     = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos    = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor  = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV     = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos    = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor  = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV     = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos    = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor  = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV     = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// 뒷 면
	arrCube[16].vPos    = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor  = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV     = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos    = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor  = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV     = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos    = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor  = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV     = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos    = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor  = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV     = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// 앞 면
	arrCube[20].vPos    = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor  = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV     = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos    = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor  = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV     = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos    = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor  = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV     = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos    = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor  = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV     = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh;
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CubeMesh", pMesh, true);
	vecIdx.clear();


	// ===========
	// Sphere Mesh
	// ===========
	fRadius = 0.5f;

	// Top
	v.vPos    = Vec3(0.f, fRadius, 0.f);
	v.vUV     = Vec2(0.5f, 0.f);
	v.vColor  = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent  = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, 1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	iSliceCount      = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
			              , fRadius * cosf(i * fStackAngle)
			              , fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV     = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor  = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos    = Vec3(0.f, -fRadius, 0.f);
	v.vUV     = Vec2(0.5f, 1.f);
	v.vColor  = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent  = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i) + (j) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j) + 1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i) + (j) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"SphereMesh", pMesh, true);

	vecVtx.clear();
	vecIdx.clear();

	// Cylinder

	// Frustum


	// ================
	// PentaHedron Mesh - For view Frustum 
	// ================
	// Normal 값 재정의 해야함 
	// ================
	/*
			  1 -- 0
			  |	   |
		4(0,0)|	   |
			  |    |
			  2	-- 3

	 */
	Vtx arrPenta2[16] = {};

	// 앞 면	
	arrPenta2[0].vPos    = Vec3(0.5f, 0.5f, 1.f);
	arrPenta2[0].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[0].vUV     = Vec2(0.f, 0.f);
	arrPenta2[0].vNormal = Vec3(0.f, 0.f, 1.f);

	arrPenta2[1].vPos    = Vec3(-0.5f, 0.5f, 1.f);
	arrPenta2[1].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[1].vUV     = Vec2(0.f, 0.f);
	arrPenta2[1].vNormal = Vec3(0.f, 0.f, 1.f);

	arrPenta2[2].vPos    = Vec3(-0.5f, -0.5f, 1.f);
	arrPenta2[2].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[2].vUV     = Vec2(0.f, 0.f);
	arrPenta2[2].vNormal = Vec3(0.f, 0.f, 1.f);

	arrPenta2[3].vPos    = Vec3(0.5f, -0.5f, 1.f);
	arrPenta2[3].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[3].vUV     = Vec2(0.f, 0.f);
	arrPenta2[3].vNormal = Vec3(0.f, 0.f, 1.f);

	// 윗 면
	arrPenta2[4].vPos    = Vec3(0.f, 0.f, 0.f);
	arrPenta2[4].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrPenta2[4].vUV     = Vec2(0.f, 0.f);
	arrPenta2[4].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrPenta2[5].vPos    = Vec3(-0.5f, 0.5f, 1.f);
	arrPenta2[5].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[5].vUV     = Vec2(0.f, 0.f);
	arrPenta2[5].vNormal = Vec3(0.f, 0.f, 1.f);

	arrPenta2[6].vPos    = Vec3(0.5f, 0.5f, 1.f);
	arrPenta2[6].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[6].vUV     = Vec2(0.f, 0.f);
	arrPenta2[6].vNormal = Vec3(0.f, 0.f, 1.f);

	// 아랫 면
	arrPenta2[7].vPos    = Vec3(0.f, 0.f, 0.f);
	arrPenta2[7].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrPenta2[7].vUV     = Vec2(0.f, 0.f);
	arrPenta2[7].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrPenta2[8].vPos    = Vec3(0.5f, 0.5f, 1.f);
	arrPenta2[8].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[8].vUV     = Vec2(0.f, 0.f);
	arrPenta2[8].vNormal = Vec3(0.f, 0.f, 1.f);

	arrPenta2[9].vPos    = Vec3(0.5f, -0.5f, 1.f);
	arrPenta2[9].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[9].vUV     = Vec2(0.f, 0.f);
	arrPenta2[9].vNormal = Vec3(0.f, 0.f, 1.f);


	// 왼쪽 면
	arrPenta2[10].vPos    = Vec3(0.f, 0.f, 0.f);
	arrPenta2[10].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrPenta2[10].vUV     = Vec2(0.f, 0.f);
	arrPenta2[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrPenta2[11].vPos    = Vec3(-0.5f, -0.5f, 1.f);
	arrPenta2[11].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[11].vUV     = Vec2(0.f, 0.f);
	arrPenta2[11].vNormal = Vec3(0.f, 0.f, 1.f);

	arrPenta2[12].vPos    = Vec3(-0.5f, 0.5f, 1.f);
	arrPenta2[12].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[12].vUV     = Vec2(0.f, 0.f);
	arrPenta2[12].vNormal = Vec3(0.f, 0.f, 1.f);

	// 오른쪽 면
	arrPenta2[13].vPos    = Vec3(0.f, 0.f, 0.f);
	arrPenta2[13].vColor  = Vec4(0.f, 1.f, 0.f, 1.f);
	arrPenta2[13].vUV     = Vec2(0.f, 0.f);
	arrPenta2[13].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrPenta2[14].vPos    = Vec3(0.5f, 0.5f, 1.f);
	arrPenta2[14].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[14].vUV     = Vec2(0.f, 0.f);
	arrPenta2[14].vNormal = Vec3(0.f, 0.f, 1.f);

	arrPenta2[15].vPos    = Vec3(0.5f, -0.5f, 1.f);
	arrPenta2[15].vColor  = Vec4(1.f, 0.f, 0.f, 1.f);
	arrPenta2[15].vUV     = Vec2(0.f, 0.f);
	arrPenta2[15].vNormal = Vec3(0.f, 0.f, 1.f);

	// 인덱스
	for (int i = 0; i < 3; i += 1)
	{
		vecIdx.push_back(4);
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i);
	}
	vecIdx.push_back(4);
	vecIdx.push_back(0);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(3);

	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	pMesh = new CMesh;
	pMesh->Create(arrPenta2, 16, vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"PentahedronMesh", pMesh, true);
	vecIdx.clear();
}

void CResMgr::CreateEngineTexture()
{
	Ptr<CTexture> pNoise01    = Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png", true);
	Ptr<CTexture> pNoise02    = Load<CTexture>(L"texturenoise\\noise_02.png", L"texture\\noise\\noise_02.png", true);
	Ptr<CTexture> pNoiseCloud = Load<CTexture>(L"texture\\noise\\noise_cloud.jpg", L"texture\\noise\\noise_cloud.jpg",
	                                           true);

	pNoise01->UpdateData(PIPELINE_STAGE::ALL, 70);
	pNoise01->UpdateData_CS(70, true);

	pNoise02->UpdateData(PIPELINE_STAGE::ALL, 71);
	pNoise02->UpdateData_CS(71, true);

	pNoiseCloud->UpdateData(PIPELINE_STAGE::ALL, 72);
	pNoiseCloud->UpdateData_CS(72, true);

	g_global.vNoiseResolution = Vec2(pNoise01->Width(), pNoise01->Height());
}

void CResMgr::CreateEngineShader()
{
	MakeInputLayoutInfo();

	CGraphicsShader* pShader = nullptr;

	// Std2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->AddScalarParamInfo(L"Mask Limit", SCALAR_PARAM::FLOAT_0);
	pShader->AddTexParamInfo(L"OutputTex 0", TEX_PARAM::TEX_0);
	pShader->AddTexParamInfo(L"OutputTex 1", TEX_PARAM::TEX_1);
	pShader->AddTexParamInfo(L"OutputTex 2", TEX_PARAM::TEX_2);

	pShader->AddScalarParamInfo(L"TestParam", SCALAR_PARAM::INT_0);


	AddRes<CGraphicsShader>(L"Std2DShader", pShader, true);


	// Std2DAlphaBlend Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2DAlpha");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DAlpha");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->AddTexParamInfo(L"OutputTex", TEX_PARAM::TEX_0);

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader, true);


	// Std3D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->AddScalarParamInfo(L"Mask Limit", SCALAR_PARAM::FLOAT_0);
	pShader->AddTexParamInfo(L"OutputTex", TEX_PARAM::TEX_0);
	pShader->AddTexParamInfo(L"NormapMap", TEX_PARAM::TEX_1);

	AddRes<CGraphicsShader>(L"Std3DShader", pShader, true);

	// Std3D Wire Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Std3DWire");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Std3DWire");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_FORWARD);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	AddRes<CGraphicsShader>(L"Std3DWireShader", pShader, true);

	// Std3D Deferred Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->AddTexParamInfo(L"OutputTex", TEX_PARAM::TEX_0);
	pShader->AddTexParamInfo(L"NormapMap", TEX_PARAM::TEX_1);
	pShader->AddTexParamInfo(L"SpecularMap", TEX_PARAM::TEX_2);
	pShader->AddTexParamInfo(L"EmbientMap", TEX_PARAM::TEX_3);

	AddRes<CGraphicsShader>(L"Std3D_DeferredShader", pShader, true);

	// Trail Shader 
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\trail.fx", "VS_Trail");
	pShader->CreatePixelShader(L"shader\\trail.fx", "PS_Trail");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	AddRes<CGraphicsShader>(L"TrailShader", pShader, true);

	// PaperBurn Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\paperburn.fx", "VS_PaperBurn");
	pShader->CreatePixelShader(L"shader\\paperburn.fx", "PS_PaperBurn");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->AddScalarParamInfo(L"Burn Strength", SCALAR_PARAM::FLOAT_0);
	pShader->AddTexParamInfo(L"OutputTex", TEX_PARAM::TEX_0);

	AddRes<CGraphicsShader>(L"PaperBurnShader", pShader, true);


	// TileMap Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->AddTexParamInfo(L"TileMapAtlas", TEX_PARAM::TEX_0);

	AddRes<CGraphicsShader>(L"TileMapShader", pShader, true);


	// Collider2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Collider2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Collider2D");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// pShader->AddScalarParamInfo(L"IsCollision", SCALAR_PARAM::INT_0);

	AddRes<CGraphicsShader>(L"Collider2DShader", pShader, true);

	// Collider3D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Collider2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Collider2D");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::NO_TEST);

	pShader->AddScalarParamInfo(L"IsCollision", SCALAR_PARAM::INT_0);

	AddRes<CGraphicsShader>(L"Collider3DShader", pShader, true);

	// SkyBox Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_SkyBox");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_SkyBox");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_FORWARD);
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->AddTexParamInfo(L"Output Texture", TEX_PARAM::TEX_0);
	pShader->AddTexParamInfo(L"Cube Texture", TEX_PARAM::TEX_CUBE_0);

	AddRes<CGraphicsShader>(L"SkyBoxShader", pShader, true);

	// Particle Render Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"Shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"Shader\\particlerender.fx", "PS_ParticleRender");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader, true);

	// PostProcess Shader
	pShader = new CGraphicsShader;

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_PostProcess");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_PostProcess");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	AddRes<CGraphicsShader>(L"PostProcessShader", pShader, true);

	// Tessellation Test Shader
	pShader = new CGraphicsShader;

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_FORWARD);
	pShader->CreateVertexShader(L"Shader\\tessellation.fx", "VS_Tess");
	pShader->CreateHullShader(L"Shader\\tessellation.fx", "HS_Tess");
	pShader->CreateDomainShader(L"Shader\\tessellation.fx", "DS_Tess");
	pShader->CreatePixelShader(L"Shader\\tessellation.fx", "PS_Tess");

	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	AddRes<CGraphicsShader>(L"TessShader", pShader, true);
}

void CResMgr::CreateEngineMaterial()
{
	wstring strContent = CPathMgr::GetInst()->GetContentPath();

	CMaterial* pMtrl = nullptr;

	// Std2DMtrl 생성
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"material\\Std2DMtrl.mtrl", pMtrl);

	// Std2DAlphaBlend
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes<CMaterial>(L"material\\Std2DAlphaBlendMtrl.mtrl", pMtrl);

	// Std3D Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3DShader"));
	AddRes<CMaterial>(L"material\\Std3DMtrl.mtrl", pMtrl);

	// Std3D Wire
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3DWireShader"));
	AddRes<CMaterial>(L"material\\Std3DWireShader.mtrl", pMtrl);

	// Collider3D
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Collider3DShader"));
	AddRes<CMaterial>(L"material\\Collider3D.mtrl", pMtrl);
	Vec4 color = {1.f, 0.f, 1.f, 1.f};
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &color);


	// Std3D Deferred Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3D_DeferredShader"));
	AddRes<CMaterial>(L"material\\Std3D_DeferredMtrl.mtrl", pMtrl);


	// SkyBox Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"SkyBoxShader"));
	AddRes<CMaterial>(L"material\\SkyBoxMtrl.mtrl", pMtrl);

	// Trail Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TrailShader"));
	AddRes<CMaterial>(L"material\\TrailMtrl.mtrl", pMtrl);


	// PaperBurnMtrl	
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PaperBurnShader"));
	AddRes<CMaterial>(L"material\\PaperBurnMtrl.mtrl", pMtrl);


	// TileMapMtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"material\\TileMapMtrl.mtrl", pMtrl);

	// Collider2DMtrl 
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Collider2DShader"));
	AddRes<CMaterial>(L"material\\Collider2DMtrl.mtrl", pMtrl);

	// Particle Render Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl", pMtrl);

	// PostProcess Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PostProcessShader"));
	//pMtrl->SetTexParam(TEX_PARAM::TEX_0, FindRes<CTexture>(L"PostProcessTex"));
	AddRes<CMaterial>(L"material\\PostProcessMtrl.mtrl", pMtrl);

	// Tessellation Test Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TessShader"));
	AddRes<CMaterial>(L"material\\TessMtrl.mtrl", pMtrl);
}

#include "CTestShader.h"
#include "CParticleUpdateShader.h"
#include "CAnimation3DShader.h"

void CResMgr::CreateEngineComputeShader()
{
	CComputeShader* pCS = nullptr;

	// TestShader
	pCS = new CTestShader;
	pCS->CreateComputeShader(L"shader\\testcs.fx", "CS_Test");
	AddRes<CComputeShader>(L"TestCS", pCS, true);

	// Particle Update Shader
	pCS = new CParticleUpdateShader;
	pCS->CreateComputeShader(L"shader\\particle.fx", "CS_Particle");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pCS, true);

	// Animation3D Update Shader	
	pCS = new CAnimation3DShader;
	pCS->CreateComputeShader(L"shader\\animation3d.fx", "CS_Animation3D");
	AddRes<CComputeShader>(L"Animation3DUpdateShader", pCS, true);
}


void CResMgr::MakeInputLayoutInfo()
{
	// 정점 입력 구조 (InputLayout 생성)	
	UINT                     iOffset    = 0;
	D3D11_INPUT_ELEMENT_DESC tInputDesc = {};

	tInputDesc.SemanticName         = "POSITION";	// Semantic 이름
	tInputDesc.SemanticIndex        = 0;			// 중복 이름인 경우 인덱스로 구분
	tInputDesc.InputSlot            = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 12;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName         = "COLOR";
	tInputDesc.SemanticIndex        = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 16;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName         = "TEXCOORD";
	tInputDesc.SemanticIndex        = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 8;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName         = "TANGENT";
	tInputDesc.SemanticIndex        = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 12;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName         = "NORMAL";
	tInputDesc.SemanticIndex        = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 12;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName         = "BINORMAL";
	tInputDesc.SemanticIndex        = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 12;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName         = "BLENDWEIGHT";
	tInputDesc.SemanticIndex        = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 16;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName         = "BLENDINDICES";
	tInputDesc.SemanticIndex        = 0;
	tInputDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tInputDesc.AlignedByteOffset    = iOffset;
	iOffset += 16;

	CGraphicsShader::AddInputLayout(tInputDesc);
}


void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}
