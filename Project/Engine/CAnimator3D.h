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
    //�� ��ü �� ��� 
    const vector<tMTBone>* m_pVecBones;
    //�ִϸ��̼� Ŭ��
    const vector<tMTAnimClip>* m_pVecClip;

    //��ü �ִϸ��̼�
    map<wstring, CAnimation3D*> m_mapAnim;
    map<wstring, Events*> m_mapAnimEvent;
    //map<wstring, void (*)()> m_mapAnimFun;

    CAnimation3D* m_pCurAnimation;
    UINT          m_iCurAnim;
    UINT          m_AnimCount;//��ü �ִϸ��̼�
    UINT          m_iBoneCount; 

    vector<float>				m_vecClipUpdateTime;
    vector<Matrix>				m_vecFinalBoneMat; // �ؽ��Ŀ� ������ ���� �������


    int							m_iFrameCount; // 30
    double						m_dCurTime;
    int							m_iCurClip; // Ŭ�� �ε��� (���� 1)

    int							m_iFrameIdx; // Ŭ���� ���� ������
    int							m_iNextFrameIdx; // Ŭ���� ���� ������
    float						m_fRatio;	// ������ ���� ����
    float                       m_fAnimDT;
    float                       m_fDivDT;

    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
    bool				        m_bFinalMatUpdate; // ������� ���� ���࿩��

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

    //animation���� �Ҵ�
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