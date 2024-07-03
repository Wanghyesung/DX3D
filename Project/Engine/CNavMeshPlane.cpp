#include "pch.h"
#include "CNavMeshPlane.h"
#include "CTransform.h"
#include "CKeyMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"
CNavMeshPlane::CNavMeshPlane():
	CComponent(COMPONENT_TYPE::NAVMESHPLANE),
	m_bActiveRay(true)
{
}

CNavMeshPlane::~CNavMeshPlane()
{

}


void CNavMeshPlane::finaltick()
{
	Vec4 vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	const Matrix& m_tMat = GetOwner()->Transform()->GetWorldMat();

	DrawDebugCube(m_tMat,vColor,0.0f,false);

	if (!m_bActiveRay)
		return;

	if (KEY_TAP(KEY::LBTN))
		if (RayCasting())
			int a = 10;
}

bool CNavMeshPlane::RayCasting()
{
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return false;

	const tRay& ray = pMainCam->GetRay();

	//������ ������
	//R(t) = pOrigin + t*vDir
	//R(t) = p0 - t * vDir    R(t) : �������� ��� ��
	// pOrigin : ������ ���� ��, t : p0�κ��� ������ �Ÿ�  vDir : ������ ����  


	//����� ������
	//N dot (p0 - p1) = 0
	// N : ����� ������ ����, p0 : ����� ������ �� �� , p1 : ������ ������ �� �� p0 - p1 : ������� ����(����)

	// S : ������ ����� ���� (���⼭ S�� ���� ������ �����̱� ������ ����� ������ p1���� ����, 
	// ������ ������ R(t)�� ���� ������ p1 = R(t)�� �Ǵ� ������ S�� ��

	//N dot (p0 - R(t)) = 0
	//N dot p0 - N dot R(t) = 0
	//N dot p0 - N dot (pOrigin + t*vDir) = 0
	//N dot p0 - N dot pOrigin - t *(N dot vDir) = 0
	//t*(N dot vDir) = N dot p0 - N dot pOrigin
	// t = N dot p0 - N dot pOrigin / N dot p0


	Vec3 vEdge[3] = {};
	vEdge [0] = m_vecWorldVertices[0] - m_vecWorldVertices[1];
	vEdge [1] = m_vecWorldVertices[2] - m_vecWorldVertices[1];
	vEdge [2] = m_vecWorldVertices[2] - m_vecWorldVertices[0];
	//����� �븻����
	Vec3 vNormal = vEdge->Cross(vEdge[1]);
	//N dot pOrigin / N dot p0
	//float f = vNormal.Dot(m_vecWorldVertices[0]) - vNormal.Dot(ray.vStart);

	//������ ����� ���� , ��鿡 ���� �� �� ������ �������� �ʴ´�.
	float f = vNormal.Dot(ray.vDir);
	if (f == 0.f)
		return false;

	Vec3 vLen = ray.vStart - m_vecWorldVertices[0];
	float fResult = -vNormal.Dot(vLen);
	float fT = fResult / f;

	Vec3 vPoint = ray.vStart + fT * ray.vDir;

	//�������� �´��� Ȯ��
	// ���� ������ �簢�� ���ο� �ִ��� Ȯ��

	// 0 ----3
	// |     |
	// 1-----2
	// 
	// �踮 ��Ʈ�� ��ǥ ���
	double uu, uv, vv, wu, wv, inverseD;

	//�ﰢ�� �� ���� ����ġ
	uu = vEdge[0].Dot(vEdge[0]);
	uv = vEdge[0].Dot(vEdge[1]);
	vv = vEdge[1].Dot(vEdge[1]);

	//�ﰢ�� �� ���� ����ġ
	Vec3 w = vPoint - m_vecWorldVertices[1];
	wu = w.Dot(vEdge[0]);
	wv = w.Dot(vEdge[1]);

	//���� ��Ʈ�� ���� ���� 
	//D�� �ﰢ���� �踮 ��Ʈ�� ��ǥ�� ���� �� ���Ǵ� ��
	inverseD = uv * uv - uu * vv;
	inverseD = 1.0f / inverseD;

	//����ġ  ������ 0~1 �϶��� ������ ��ȯ

	//u =        D
	//    (uv*wv - vv*wu)

	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.0f || u > 1.0f) 
		return false;

	//v =         D
	//     (uv*wu - uu*wv)

	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.0f || (u + v) > 1.0f)
		return false;

	return true;
}

float CNavMeshPlane::GetArea(const Vec3& _vP1, const Vec3& _vP2)
{
	float fArea =
		(_vP1.x*_vP2.y + _vP1.y*_vP2.z + _vP1.z*_vP2.x) -
		(_vP1.y*_vP2.x + _vP1.z*_vP2.y + _vP1.x*_vP2.z);

	return fArea / 2.f;

}

void CNavMeshPlane::SaveToLevelFile(FILE* _File)
{
}

void CNavMeshPlane::LoadFromLevelFile(FILE* _File)
{
}


