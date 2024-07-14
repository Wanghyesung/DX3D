#pragma once
#include <Engine\CScript.h>
class CFSM;
class CGage;
class CPlayerScript :
    public CScript
{
private:
    float       m_fSpeed;

    int         m_iActive;
    CFSM*       m_pFSM;

    int         m_iBone;
    
    Vec3        m_vOffsetTransform;

    tActorInfo  m_tPlayerInfo;
    tHitInfo    m_tHitInfo;

    CGage*      m_pHP;
    CGage*      m_pHeal;

    vector<CGameObject*> m_vecWeapon;
    int                  m_iActiveWeaponIdx;
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;

    void SetHitInfo(const tHitInfo& _tHitInfo) { m_tHitInfo = _tHitInfo; }
    void SetPlayerInfo(const tActorInfo& _tActorInfo) { m_tPlayerInfo = _tActorInfo; }

    void SetActive(bool _bActive) {}

    void SetFSM(CFSM* _pFSM);
    CFSM* GetFSM() { return m_pFSM; }

    void Initialize();

    void Chanage_AnimDT(float _fDivDT);
private:
    void tick_gage();

    void set_ui();
    void set_attack();
    void rotate();

    void change_equip();
public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

