#pragma once
#include "CComponent.h"

class CMotionBlur : public CComponent
{
private:
	float m_fPosCheckTime;
	float m_fCurCheckTime;

	//�����ð� ���̴��� ȣ���� ��ġ
	Vec3 m_vCheckPos;
	//���� ������Ʈ�� ũ�⸸ŭ mesh�� �ø�
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

