#pragma once
#include "CEntity.h"
#include "CAnimator3D.h"


class CAnimation3D : public CEntity
{
private:
	wstring m_strName;

	int   m_iStartFrame;
	int   m_iEndFrame;
	int   m_iNextFrame;

	float m_fStartTime;
	float m_fEndTime;

	int   m_iCurFrame;
	float m_fCurTime;

	//현재 프레임 끝나는 비율
	float m_fRatio;

	//bool  m_bRepeat;
	UINT m_iAnimIdx;

	CAnimator3D* m_pOwner;
public:
	void final_tick();

	void SetKey(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetKey() { return m_strName; }

	bool IsComplete();

	void Reset();

public:
	//void SetStartFrame();
	void SetFrame(int _iStartFrame);

	int GetCurFrame() { return m_iCurFrame; }
	int GetEndFrame() { return m_iEndFrame; }
	int GetStartFrame() { return m_iStartFrame; }

	void SetAnimIdx(UINT _iIdx) { m_iAnimIdx = _iIdx; }
	UINT GetAnimIdx() { return m_iAnimIdx; }

	CLONE(CAnimation3D);
public:
	CAnimation3D();
	~CAnimation3D();

	friend class CAnimator3D;
};

