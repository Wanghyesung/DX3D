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

	//���� ������ ������ ����
	float m_fRatio;

	//bool  m_bRepeat;
	UINT m_iAnimIdx;

	//�ִϸ��̼� �ݴ�� ����
	bool m_bReverse;

	CAnimator3D* m_pOwner;

	vector <FrameEvent> m_vecEvent;
	int					m_iCurEventCheck; 
	//������ ������ ���� �����ӿ� �̺�Ʈ�� �� �ɸ� �� �ֱ� ������
	//���� ������ üũ�ߴ��� �˻��صΰ� ���ƴٸ� ���� ������ �˻�
private:
	void reverse_update();
	void update();

	void check_event();
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

	void AddAnimEvent(const FrameEvent& _tEvent);

	CLONE(CAnimation3D);
public:
	CAnimation3D();
	~CAnimation3D();

	friend class CAnimator3D;
};

