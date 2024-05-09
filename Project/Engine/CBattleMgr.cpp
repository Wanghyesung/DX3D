#include "pch.h"
#include "CBattleMgr.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"

void CBattleMgr::ShowFont(int _iDamage, Vec2 _vPos)
{
	wstring strDamage = std::to_wstring(_iDamage);
	
	CFontMgr::GetInst()->DrawFont(strDamage.c_str(), _vPos.x, _vPos.y, 16, FONT_RGBA(127, 127, 127, 255));
}

void CBattleMgr::AddFont(float _fTime, int _iDamage, Vec2 _vPos)
{
	tDamageFont tFont = {};
	tFont.fTime = _fTime;
	tFont.fCurTime = 0.f;
	tFont.iDamage = _iDamage;

	//위치 윈도우 좌표계로 바꿔야
	tFont.vPos = _vPos;

	m_vecFont.push_back(tFont);
}

void CBattleMgr::tick()
{
	vector< tDamageFont>::iterator iter = m_vecFont.begin();

	for (iter; iter != m_vecFont.end(); )
	{
		if (iter->fCurTime < iter->fTime)
		{
			ShowFont(iter->iDamage, iter->vPos);
		}
		else
		{
			iter = m_vecFont.erase(iter);
			continue;
		}

		++iter;
	}
}

