#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"
#include "CAnimation3D.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_mapAnim{}
	, m_pCurAnimation(nullptr)
	, m_iCurClip(0)
	, m_dCurTime(0.)
	, m_iFrameCount(30)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_bStop(true)
	, m_bRepeat(true)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_iCurAnim(0)
	, m_AnimCount(0)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_mapAnim(_origin.m_mapAnim)
	, m_pCurAnimation(nullptr)
	, m_iCurClip(_origin.m_iCurClip)
	, m_dCurTime(_origin.m_dCurTime)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(_origin.m_iFrameIdx)
	, m_iNextFrameIdx(_origin.m_iNextFrameIdx)
	, m_fRatio(_origin.m_fRatio)
	, m_bStop(false)
	, m_bRepeat(true)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;

	map<wstring, CAnimation3D*>::iterator iter =
		m_mapAnim.begin();

	for (; iter != m_mapAnim.end(); ++iter)
	{
		delete iter->second;
		iter->second = nullptr;
	}
}


void CAnimator3D::finaltick()
{
	if (nullptr == m_pCurAnimation)
		return;

	if (m_bStop)
		return;

	if (m_pCurAnimation->IsComplete() && m_bRepeat)
	{
		Events* pEvents =
			FindEvents(m_pCurAnimation->GetKey());

		if (pEvents)
			pEvents->tCompleteEvent();

		m_pCurAnimation->Reset();
	}

	m_pCurAnimation->final_tick();
	m_iCurAnim = m_pCurAnimation->m_iAnimIdx;

	m_iFrameIdx = m_pCurAnimation->m_iCurFrame;
	m_iNextFrameIdx = m_pCurAnimation->m_iNextFrame;
	m_fRatio = m_pCurAnimation->m_fRatio;

	m_bFinalMatUpdate = false;

	//m_dCurTime = 0.f;
	//// 현재 재생중인 Clip 의 시간을 진행한다.
	//m_vecClipUpdateTime[m_iCurClip] += DT;
	//
	//if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
	//{
	//	m_vecClipUpdateTime[m_iCurClip] = 0.f;
	//}
	//
	//m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	//전체 시간 48초
	//초마다 30프레임 emode30
	//전체 프레임 1432
	//뼈 134

	// 현재 프레임 인덱스 구하기
	//double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	//m_iFrameIdx = (int)(dFrameIdx);
	//
	//// 다음 프레임 인덱스
	//if (m_iFrameIdx >= m_pVecClip->at(0).iFrameLength - 1)
	//	m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	//else
	//	m_iNextFrameIdx = m_iFrameIdx + 1;
	//
	//// 프레임간의 시간에 따른 비율을 구해준다.
	//m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);
	//
	//// 컴퓨트 쉐이더 연산여부
	//m_bFinalMatUpdate = false;
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());

	// 테스트 코드
	/*static float fTime = 0.f;
	fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;*/
}

void CAnimator3D::CreateAnimationF(const wstring& _strAnimName, int _iStartFrame, int _iLastFrame)
{
	CAnimation3D* pAnim = FindAnimation(_strAnimName);
	if (pAnim != nullptr)
		return;

	pAnim = new CAnimation3D;
	Events* tEvent = {};
	m_mapAnimEvent.insert(make_pair(_strAnimName, tEvent));
	pAnim->SetName(_strAnimName);

	//pAnim->m_bRepeat = _bRepeat;

	pAnim->m_iStartFrame = _iStartFrame;
	pAnim->m_iCurFrame = _iStartFrame;
	pAnim->m_iEndFrame = _iLastFrame;

	pAnim->m_fStartTime = float(_iStartFrame) / float(m_iFrameCount);
	pAnim->m_fCurTime = float(_iStartFrame) / float(m_iFrameCount);
	pAnim->m_fEndTime = float(_iLastFrame) / float(m_iFrameCount);
	pAnim->m_pOwner = this;

	pAnim->m_iAnimIdx = ++m_AnimCount;

	m_mapAnim.insert(make_pair(_strAnimName, pAnim));

	m_pCurAnimation = pAnim;
}

void CAnimator3D::CreateAnimationT(const wstring& _strAnimName, float _fStartTime, float _fLastTime)
{
	CAnimation3D* pAnim = FindAnimation(_strAnimName);
	if (pAnim != nullptr)
		return;

	pAnim = new CAnimation3D;

	//pAnim->m_bRepeat = _bRepeat;

	pAnim->m_fStartTime = _fStartTime;
	pAnim->m_fCurTime = _fStartTime;
	pAnim->m_fEndTime = _fLastTime;

	pAnim->m_iStartFrame = (int)(_fStartTime * m_iFrameCount);
	pAnim->m_iCurFrame = (int)(_fStartTime * m_iFrameCount);
	pAnim->m_iEndFrame = (int)(_fLastTime * m_iFrameCount);
	pAnim->m_pOwner = this;

	pAnim->m_iAnimIdx = ++m_AnimCount;

	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

void CAnimator3D::SetCurFrame(int _iCurFame)
{
	m_pCurAnimation->SetFrame(_iCurFame);
}

CAnimation3D* CAnimator3D::FindAnimation(const wstring& _strName)
{
	map<wstring, CAnimation3D*>::iterator iter =
		m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator3D::SetCurAnimation(const wstring& _strName)
{
	CAnimation3D* pAnim = FindAnimation(_strName);
	if (pAnim == nullptr)
		return;

	m_pCurAnimation = pAnim;
}


void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		m_iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(m_iBoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();


		//m_vecFinalBoneMat.resize(m_iBoneCount);
		//m_pBoneFinalMatBuffer->GetData(m_vecFinalBoneMat.data());
		

		m_bFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}


void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, true, nullptr);
	}

}

void CAnimator3D::Play(const std::wstring& _strName, bool _bRepeat)
{
	CAnimation3D* pPrevAnim = m_pCurAnimation;

	Events* pEvents;
	if (pPrevAnim != nullptr)
	{
		pEvents = FindEvents(pPrevAnim->GetKey());
		if (pEvents)
			pEvents->tEndEvent();
	}

	CAnimation3D* pAnim = FindAnimation(_strName);
	if (pAnim)
	{
		m_pCurAnimation = pAnim;
		//m_iCurAnim = pAnim->GetAnimIdx;
	}

	pEvents = FindEvents(m_pCurAnimation->GetKey());
	if (pEvents)
		pEvents->tStartEvent();


	//m_pCurAnimation->m_bRepeat = _bRepeat;
	m_bRepeat = _bRepeat;
	m_pCurAnimation->Reset();
}

Events* CAnimator3D::FindEvents(const std::wstring& _strName)
{
	std::map<std::wstring, Events*>::iterator iter =
		m_mapAnimEvent.find(_strName);

	if (iter == m_mapAnimEvent.end())
		return nullptr;

	return iter->second;
}


void CAnimator3D::Reset()
{
	m_pCurAnimation->Reset();
}

void CAnimator3D::SetCurIdx(int _iIdx)
{
	if (_iIdx > m_mapAnim.size())
		_iIdx = m_mapAnim.size() - 1;

	map<wstring, CAnimation3D*>::iterator iter =
		m_mapAnim.begin();

	for (iter; iter != m_mapAnim.end(); ++iter)
	{
		if (iter->second->GetAnimIdx() == _iIdx)
		{
			Play(iter->second->GetName(), true);
		}
	}
}

std::function<void()>& CAnimator3D::StartEvent(const std::wstring _strKey)
{
	Events* pEvents = FindEvents(_strKey);

	return pEvents->tStartEvent.m_Event;

}
std::function<void()>& CAnimator3D::CompleteEvent(const std::wstring _strKey)
{
	Events* pEvents = FindEvents(_strKey);

	return pEvents->tCompleteEvent.m_Event;
}
std::function<void()>& CAnimator3D::EndEvent(const std::wstring _strKey)
{
	Events* pEvents = FindEvents(_strKey);

	return pEvents->tEndEvent.m_Event;
}


void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}