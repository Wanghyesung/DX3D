#include "pch.h"
#include "CAnimation3D.h"

#include "CTimeMgr.h"
#include "CAnimator3D.h"

void CAnimation3D::final_tick()
{
	//if (m_fCurTime >= m_fEndTime && m_bRepeat)
	//{
	//	Reset();
	//}

	m_fCurTime += DT;
	//const vector<tMTAnimClip>* vecClip = m_pOwner->GetAnimClip();

	//전체 시간 48초
	//초마다 30프레임 emode30
	// 2 프레임 
	//전체 프레임 1432
	//뼈 134

	//30프레임 고정
	int iFrameCount = 30;
	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_fCurTime * (double)iFrameCount;
	m_iCurFrame = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iCurFrame >= m_iEndFrame)
	{
		m_iCurFrame = m_iEndFrame;

		m_iNextFrame = m_iEndFrame;	// 끝이면 마지막 위치로
	}
	else
	{
		m_iNextFrame = m_iCurFrame + 1;
		// 프레임간의 시간에 따른 비율을 구해준다.
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
