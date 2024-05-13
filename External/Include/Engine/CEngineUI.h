#pragma once
#pragma once
#include "CGameObject.h"

class CEngineUI : public CGameObject
{
public:
	CEngineUI();
	CEngineUI(const CEngineUI& _pOrigin);
	virtual ~CEngineUI();

	virtual void Initialize(const wstring& _strTexName, const Vec3& _vScale, 
		const wstring& _strName);
	virtual void finaltick()override;
	
	virtual void MouseOn();//UI���� ���콺 �ö���� �� 
	virtual void MouseLbtnDown();//UI�ȿ��� ������ ��
	virtual void MouseLbtnUp();//UI�ȿ��� ���� ��
	virtual void MouseLbtnClicked();//UI�ȿ��� ������ ���� ��

	void MoveUI(CEngineUI* _pUI);
	void MoveToParent(Vector2 _vDiff);

	bool IsLBntDown() { return m_bLbntDown; }
	bool IsMosueOn() { return m_bMouseOn; }

	//private -> public
	void MouseOnCheck();

	const vector<CEngineUI*> GetChildUI() { return m_vecChildUI; }

	void UpdateCameraPos();
private:
	//�ڽ� UI ������Ʈ
	//void ChildUpdate();
	//void ChildRender();
	//void ChildLateupdate();

	//void MoveToParent();
	Ptr<class CMaterial> create_uimatrial();

private:
	//�� �ڽ� UI
	vector<CEngineUI*> m_vecChildUI;
	//�� �θ� UI
	CEngineUI* m_pParentUI;

	bool m_bMouseOn;
	bool m_bLbntDown;

	friend class CEngineUIMgr;
};





