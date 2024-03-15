#include "pch.h"
#include "CAnimation3D.h"

#include "CTimeMgr.h"
#include "CAnimator3D.h"



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

	if (m_iCurFrame <= m_iEndFrame)
	{
		m_iCurFrame = m_iEndFrame;

		m_iNextFrame = m_iEndFrame;
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

	//��ü �ð� 48��
	//�ʸ��� 30������ emode30
	// 2 ������ 
	//��ü ������ 1432
	//�� 134

	//30������ ����
	int iFrameCount = 30;
	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_fCurTime * (double)iFrameCount;
	m_iCurFrame = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iCurFrame >= m_iEndFrame)
	{
		m_iCurFrame = m_iEndFrame;

		m_iNextFrame = m_iEndFrame;	// ���̸� ������ ��ġ��
	}
	else
	{
		m_iNextFrame = m_iCurFrame + 1;
		// �����Ӱ��� �ð��� ���� ������ �����ش�.
	}

	m_fRatio = (float)(dFrameIdx - (double)m_iCurFrame);
}


bool CAnimation3D::IsComplete()
{
	if (m_iCurFrame >= m_iEndFrame)
		return true;

	return false;
}

void CAnimation3D::Reset()
{
	m_iCurFrame = m_iStartFrame;
	m_fCurTime = m_fStartTime;
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

CAnimation3D::CAnimation3D() :
	m_iStartFrame(0),
	m_iEndFrame(0),
	m_fStartTime(0),
	m_fEndTime(0),
	m_iCurFrame(0),
	m_fCurTime(0),
	m_iNextFrame(0),
	m_fRatio(0.f)
	//m_bRepeat(false)
{

}

CAnimation3D::~CAnimation3D()
{

}
