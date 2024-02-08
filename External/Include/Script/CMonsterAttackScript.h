#pragma once
#include <Engine\CScript.h>

class CMonsterAttackScript :
    public CScript
{
private:
    tAttack m_tAttack;

    CGameObject* m_pSpawnOwner;//객체를 소환한 오브젝트

    //내 범위에 들어온 몬스터들
    CGameObject* m_pTarget;//player

    float m_fCurAttackTime;
    float m_fAttackTime;
    bool m_bAttackOn;

private:
    void init_attack();

public:
    virtual void tick() override;
    virtual void begin()override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;

    void SetSpawnOwner(CGameObject* _pGameObj) { m_pSpawnOwner = _pGameObj; }
    void SetAttackValue(const tAttack& _tAttack) { m_tAttack = _tAttack; }
    const tAttack& GetAttackValue() { return m_tAttack; }

    bool IsAttackOn();
private:
    CLONE(CMonsterAttackScript);

public:
    CMonsterAttackScript();
    ~CMonsterAttackScript();
};

