#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"


CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)		
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
	//Vec3 vPos = GetMesh()->GetVtxSysMem()->vPos;
	//Matrix matScale = Transform()->GetWorldScaleMat();
	//Vec3 vScale = XMVector4Transform(vPos, matScale);
	//int a = 10;
}

void CMeshRender::render()
{	
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D 업데이트
	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (UINT i = 0; i < GetMtrlCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh 알리기
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	// 렌더
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			// 사용할 재질 업데이트
			GetMaterial(i)->UpdateData();

			// 사용할 메쉬 업데이트 및 렌더링
			GetMesh()->render(i);
		}
	}

	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
	//다음 프레임 그림자 렌더링에서 지우기
}

void CMeshRender::render(UINT _iSubset)
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(_iSubset))
		return;

	if (_iSubset > 0)
	{
		int a = 10;
	}

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}


	if (Animator3D())
	{
		Animator3D()->UpdateData();
		GetMaterial(_iSubset)->SetAnim3D(true); //Animation Mesh 알리기
		GetMaterial(_iSubset)->SetBoneCount(Animator3D()->GetBoneCount());
	}

	GetMaterial(_iSubset)->UpdateData();

	GetMesh()->render(_iSubset);

	// Animation 관련 정보 제거
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}
