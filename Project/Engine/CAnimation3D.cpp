#include "pch.h"
#include "CAnimation3D.h"

#include "CTimeMgr.h"
#include "CAnimator3D.h"

CAnimation3D::CAnimation3D() :
	m_iStartFrame(0),
	m_iEndFrame(0),
	m_fStartTime(0),
	m_fEndTime(0),
	m_iCurFrame(0),
	m_fCurTime(0),
	m_iNextFrame(0),
	m_fRatio(0.f),
	m_vecEvent{},
	m_iCurEventCheck(0)
	//m_bRepeat(false)
{

}

CAnimation3D::~CAnimation3D()
{

}

void CAnimation3D::final_tick()
{
	if (m_bReverse)
		reverse_update();
	else
		update();
}

void CAnimation3D::reverse_update()
{
	m_fCurTime -= m_pOwner->GetAnimDT();
	
	//30������ ����
	int iFrameCount = 30;
	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_fCurTime * (double)iFrameCount;
	m_iCurFrame = (int)(dFrameIdx);

	//������ ���������� �ʱ�ȭ 1183  1175
	if (m_iCurFrame <= m_iStartFrame)
	{
		m_iCurFrame = m_iStartFrame;

		m_iNextFrame = m_iStartFrame;
	}
	else
	{
		m_iNextFrame = m_iCurFrame - 1;
	}

	m_fRatio = (float)(dFrameIdx - (double)m_iCurFrame);
}

void CAnimation3D::update()
{
	//if (m_fCurTime >= m_fEndTime && m_bRepeat)
	//{
	//	Reset();
	//}

	m_fCurTime += m_pOwner->GetAnimDT();
	//const vector<tMTAnimClip>* vecClip = m_pOwner->GetAnimClip();

	check_event();

	//30������ ����
	const int iFrameCount = 30;
	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_fCurTime * (double)iFrameCount;
	m_iCurFrame = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iCurFrame >= m_iEndFrame)
	{
		m_iCurFrame = m_iEndFrame;

		m_iNextFrame = m_iEndFrame;	// ���̸� ������ ��ġ��

		dFrameIdx = m_fEndTime * (double)iFrameCount;
	}
	else
	{
		m_iNextFrame = m_iCurFrame + 1;
		// �����Ӱ��� �ð��� ���� ������ �����ش�.
	}

	m_fRatio = (float)(dFrameIdx - (double)m_iCurFrame);
}

void CAnimation3D::check_event()
{
	//������ ������
	if (m_iCurEventCheck == m_iEndFrame)
		return;

	//������ �����Ӹ�ŭ �̺�Ʈ ȣ�� ���� �����ӱ���
	for (m_iCurEventCheck; m_iCurEventCheck <= m_iCurFrame; ++m_iCurEventCheck)
	{
		m_vecEvent[m_iCurEventCheck].tEvent();
	}
}


bool CAnimation3D::IsComplete()
{
	if (m_bReverse)
	{
		if (m_iCurFrame <= m_iStartFrame)
			return true;

		return false;
	}
	else
	{
		if (m_iCurFrame >= m_iEndFrame)
			return true;

		return false;
	}
}

void CAnimation3D::Reset()
{
	if (m_bReverse)
	{
		m_fCurTime = m_fEndTime;
		m_iCurFrame = m_iEndFrame;
	}
	else
	{
		m_fCurTime = m_fStartTime;
		m_iCurFrame = m_iStartFrame;
	}
		
	m_iCurEventCheck = m_iCurFrame;
}

void CAnimation3D::SetFrame(int _iStartFrame)
{
	m_iCurFrame = _iStartFrame; 
	m_fCurTime = float(_iStartFrame) / float(30);

	if (m_iCurFrame >= m_iEndFrame)
	{
		m_iCurFrame = m_iEndFrame;
		m_fCurTime = m_fEndTime;
	}
	else if (m_iCurFrame <= m_iStartFrame)
	{
		m_iCurFrame = m_iStartFrame;
		m_fCurTime = m_fStartTime;
	}
}


void CAnimation3D::AddAnimEvent(const FrameEvent& _tEvent)
{
	m_vecEvent[_tEvent.iStartFrame] = _tEvent;
}

