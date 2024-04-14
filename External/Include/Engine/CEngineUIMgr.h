#pragma once
#include "CSingleton.h"
#include "CEngineUI.h"

class CEngineUIMgr : public CSingleton<CEngineUIMgr>
{
	SINGLE(CEngineUIMgr);

	void tick();
	void SetFoucseUI(CEngineUI* _pUI);
	void MoveFrontChildUI(CEngineUI* pUI);

	CEngineUI* GetFoucseUI();
	CEngineUI* GetTargetUI(CEngineUI* _pParentUI);
	void ReleaseChildUI();

	CEngineUI* m_pFoucseUI;
};

