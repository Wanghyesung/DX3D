#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CMeshData : public CRes
{
private:
	Ptr<CMesh>		m_pMesh;
	vector<Ptr<CMaterial>> m_vecMtrl;

public:
	static vector<Ptr<CMeshData>> LoadFromFBX(const wstring& _strFilePath);

	virtual int Save(const wstring& _strFilePath) override;
	virtual int Load(const wstring& _strFilePath) override;


	CGameObject* Instantiate();

	CLONE_DISABLE(CMeshData)
public:
	CMeshData(bool _bEngine = false);
	virtual ~CMeshData();
};

