#pragma once
#include "CComponent.h"

class CMotionBlur : public CComponent
{
private:
	float m_fPosCheckTime;
	float m_fCurCheckTime;

	//일정시간 쉐이더를 호출할 위치
	Vec3 m_vCheckPos;
	//내가 오브젝트의 크기만큼 mesh를 늘림
	CGameObject* m_pMotionBlur;
private:
	void create_mesh();

public:
	CMotionBlur();
	~CMotionBlur();

	void Initialize();

	CLONE(CMotionBlur);

	virtual void finaltick()override;


	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);


};

