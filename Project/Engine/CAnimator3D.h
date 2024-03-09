#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;
class CAnimation3D;

struct Event
{
    void operator=(std::function<void()> _func)
    {
        m_Event = std::move(_func);
    }

    void operator()()
    {
        if (m_Event)
            m_Event();
    }
    std::function<void()> m_Event;
};


struct Events
{
    Event tStartEvent;
    Event tCompleteEvent;
    Event tEndEvent;
};

class CAnimator3D :
    public CComponent
{
private:
    //내 전체 뼈 행렬 
    const vector<tMTBone>* m_pVecBones;
    //애니메이션 클립
    const vector<tMTAnimClip>* m_pVecClip;

    //전체 애니메이션
    map<wstring, CAnimation3D*> m_mapAnim;
    map<wstring, Events*> m_mapAnimEvent;
    //map<wstring, void (*)()> m_mapAnimFun;

    CAnimation3D* m_pCurAnimation;
    UINT          m_iCurAnim;
    UINT          m_AnimCount;//전체 애니메이션
    UINT          m_iBoneCount; 

    vector<float>				m_vecClipUpdateTime;
    vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보


    int							m_iFrameCount; // 30
    double						m_dCurTime;
    int							m_iCurClip; // 클립 인덱스 (고정 1)

    int							m_iFrameIdx; // 클립의 현재 프레임
    int							m_iNextFrameIdx; // 클립의 다음 프레임
    float						m_fRatio;	// 프레임 사이 비율
    float                       m_fAnimDT;
    float                       m_fDivDT;

    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
    bool				        m_bFinalMatUpdate; // 최종행렬 연산 수행여부

    bool                        m_bStop;
    bool                        m_bRepeat;

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

    void CreateAnimationF(const wstring& _strAnimName, int _iStartFrame, int _iLastFrame);
    void CreateAnimationT(const wstring& _strAnimName, float _fStartTime, float _fLastTime);
    void AddAnimationF(const wstring& _strAnimName, int _iStartFrame, int _iLastFrame);

    CAnimation3D* FindAnimation(const wstring& _strName);
    void SetCurAnimation(const wstring& _strName);

    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    void ClearData();

    bool IsFinalMatUpdate(){return m_bFinalMatUpdate;}

    float GetAnimDT() { return m_fAnimDT; }
    void  SetAnimDivDT(float _fDiv) { m_fDivDT = _fDiv; }

    //animation에서 할당
    void SetNextFrame(int _iNextFrame) { m_iNextFrameIdx = _iNextFrame; }
    void SetCurFrame(int _iCurFame);
    void SetRatio(float _fRatio) { m_fRatio = _fRatio; }
    UINT GetCurAnimIdx() { return m_iCurAnim; }

    const vector<tMTAnimClip>* GetAnimClip() { return m_pVecClip; }

    Events* FindEvents(const std::wstring& _strName);
    void Play(const std::wstring& _strName, bool _bRepeat);
    void Stop(bool _bStop) { m_bStop = _bStop; }
    bool IsStop() { return m_bStop; }
    
    void Reset();

    CAnimation3D* GetCurAnim() { return m_pCurAnimation; }
    void SetCurIdx(int _iIdx);
    //animation event
    std::function<void()>& StartEvent(const std::wstring _strKey);
    std::function<void()>& CompleteEvent(const std::wstring _strKey);
    std::function<void()>& EndEvent(const std::wstring _strKey);



private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};