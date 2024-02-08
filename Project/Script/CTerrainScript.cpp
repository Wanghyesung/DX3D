#include "pch.h"
#include "CTerrainScript.h"

CTerrainScript::CTerrainScript()
	: CScript((UINT)SCRIPT_TYPE::TERRAINSCRIPT)
{
}

CTerrainScript::~CTerrainScript()
{
}

void CTerrainScript::begin()
{
}

void CTerrainScript::tick()
{
}

void CTerrainScript::BeginOverlap(CCollider3D* _Other)
{
}

void CTerrainScript::OnOverlap(CCollider3D* _Other)
{
}

void CTerrainScript::EndOverlap(CCollider3D* _Other)
{
}

void CTerrainScript::Initialize(const wstring& _strFbxName)
{
	CGameObject* pTerrain = GetOwner();
	pTerrain->SetName(_strFbxName);

	vector<Ptr<CMeshData>> vecMeshData = {};
	CGameObject* pObj = nullptr;

	//Artorias4.fbx
	vector<Ptr<CMeshData>> pVecMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\" + _strFbxName + L".fbx");

	for (int i = 0; i < pVecMeshData.size(); ++i)
	{
		wstring strNum = std::to_wstring(i);
		

		Ptr<CMeshData> pMeshData = pVecMeshData[i];
		if (pMeshData != nullptr)
		{
			pObj = pMeshData->Instantiate();
			pObj->SetName(_strFbxName + strNum);
			pTerrain->AddChild(pObj);
		}
	}

	//Transform()->SetRelativeRot(-XM_PI / 2.f, 0.f, 0.f);
	//Transform()->SetRelativeScale(2.f, 2.f, 2.f);
}

void CTerrainScript::SaveToLevelFile(FILE* _File)
{
}

void CTerrainScript::LoadFromLevelFile(FILE* _FILE)
{

}



