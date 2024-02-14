#pragma once
#include <Engine\CScript.h>
class CFSM;

class CPlayerScript :
    public CScript
{
private:
    float       m_fSpeed;

    int         m_iActive;
    CFSM*       m_pFSM;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;

    void SetActive(bool _bActive) {}

    void SetFSM(CFSM* _pFSM);
    void Initialize();
private:

    void set_attack();
    void rotate();
public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

