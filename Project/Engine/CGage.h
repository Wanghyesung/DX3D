#pragma once
#include "CEngineUI.h"

class CGage : public CEngineUI
{
private:
	float m_fMaxRatio;
	float m_fCurRatio;
public:
	virtual void Initialize(const wstring& _strTexName, const Vec3& _vScale,
		const wstring& _strName)override;
	void UpdateGage(float _fMaxGage, float _fCurGage);

public:
	virtual void MouseOn()override {};
	virtual void MouseLbtnDown()override {};
	virtual void MouseLbtnUp()override {};
	virtual void MouseLbtnClicked()override {};
public:
	CGage();
	virtual ~CGage();
};

