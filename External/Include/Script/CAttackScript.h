#pragma once
#include <Engine\CScript.h>

class CAttackScript :
    public CScript
{
private:
    tAttack m_tAttack;

    CGameObject* m_pSpawnOwner;//객체를 소환한 오브젝트

    int m_iAttackCount; //공격 가능 횟수
    float m_fAttackTime; //다음 공격 가능 시간

    //내 범위에 들어온 몬스터들
    vector<UINT> m_vecAbleAttack;
    vector<float> m_vecTime;

private:
    void add_monster(UINT _ID);

    UINT find_monster(UINT _ID);
public:
    virtual void tick() override;
    virtual void begin()override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override; 

    void SetSpawnOwner(CGameObject* _pGameObj) { m_pSpawnOwner = _pGameObj; }

    void SetAttackValue(const tAttack& _tAttack) { m_tAttack = _tAttack; }
private:
    CLONE(CAttackScript);

public:
    CAttackScript();
    ~CAttackScript();
};

