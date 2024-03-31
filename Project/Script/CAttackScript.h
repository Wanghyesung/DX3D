#pragma once
#include <Engine\CScript.h>

struct tCheckInfo
{
    int ID;
    float fAttackTime;
    float fCurTime;

    bool bAttackOn;
};

class CAttackScript :
    public CScript
{
private:
    tAttack m_tAttack;

    CGameObject* m_pSpawnOwner;//객체를 소환한 오브젝트

    //내 범위에 들어온 몬스터들
    vector< tCheckInfo> m_vecCheck;

private:
    void add_monster(int _ID);
    void erase_monster(int _ID);

protected:
    tCheckInfo& find_monster(int _ID);

public:
    virtual void tick() override;
    virtual void begin()override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override; 

    void SetSpawnOwner(CGameObject* _pGameObj) { m_pSpawnOwner = _pGameObj; }
    void SetAttackValue(const tAttack& _tAttack) { m_tAttack = _tAttack; }
    const tAttack& GetAttackValue() { return m_tAttack; }

    bool IsAttackOn(int _ID);
private:
    CLONE(CAttackScript);

public:
    CAttackScript();
    CAttackScript(SCRIPT_TYPE _eType);
    ~CAttackScript();
};

