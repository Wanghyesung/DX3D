#pragma once
#include <Engine\CScript.h>
class CAttackScript :
    public CScript
{
private:
    tAttack m_tAttack;

    CGameObject* m_pOwner;//��ü�� ��ȯ�� ������Ʈ
public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)override;
    virtual void OnOverlap(CCollider2D* _Other)override;
    virtual void EndOverlap(CCollider2D* _Other)override; 

private:
    CLONE(CAttackScript);

public:
    CAttackScript();
    ~CAttackScript();
};

