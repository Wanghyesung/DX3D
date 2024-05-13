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
	
	virtual void MouseOn();//UI위에 마우스 올라왔을 떄 
	virtual void MouseLbtnDown();//UI안에서 눌렸을 떄
	virtual void MouseLbtnUp();//UI안에서 땠을 때
	virtual void MouseLbtnClicked();//UI안에서 누르고 떘을 떄

	void MoveUI(CEngineUI* _pUI);
	void MoveToParent(Vector2 _vDiff);

	bool IsLBntDown() { return m_bLbntDown; }
	bool IsMosueOn() { return m_bMouseOn; }

	//private -> public
	void MouseOnCheck();

	const vector<CEngineUI*> GetChildUI() { return m_vecChildUI; }

	void UpdateCameraPos();
private:
	//자식 UI 업데이트
	//void ChildUpdate();
	//void ChildRender();
	//void ChildLateupdate();

	//void MoveToParent();
	Ptr<class CMaterial> create_uimatrial();

private:
	//내 자식 UI
	vector<CEngineUI*> m_vecChildUI;
	//내 부모 UI
	CEngineUI* m_pParentUI;

	bool m_bMouseOn;
	bool m_bLbntDown;

	friend class CEngineUIMgr;
};





