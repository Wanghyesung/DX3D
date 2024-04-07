#pragma once
#include "CComponent.h"

class CNavMeshPlane : public CComponent
{
private:
	

public:
	virtual void finaltick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	
	CLONE(CNavMeshPlane);

public:
	CNavMeshPlane();
	virtual ~CNavMeshPlane();
};

