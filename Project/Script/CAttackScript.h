#pragma once
#include <Engine\CScript.h>

class CAttackScript :
    public CScript
{
private:
    tAttack m_tAttack;

    CGameObject* m_pSpawnOwner;//��ü�� ��ȯ�� ������Ʈ

    int m_iAttackCount; //���� ���� Ƚ��
    float m_fAttackTime; //���� ���� ���� �ð�

    //�� ������ ���� ���͵�
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

