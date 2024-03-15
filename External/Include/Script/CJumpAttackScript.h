#pragma once
#include "CAttackScript.h"

class CJumpAttackScript :
    public CAttackScript
{
private:
    bool m_bBeginOn;

    Matrix m_matFinalBone;
    int m_iBoneIndex;

    Vec3 m_vOffsetTransform; //몬스터 잡기 위치
    

    CGameObject* m_pPlayer;
private:
    void check_bone_pos(CGameObject* _pMonster);

    bool check_pos(CGameObject* _pMonster);
public:
    virtual void tick() override;
    virtual void begin()override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;

private:
    CLONE(CJumpAttackScript);

public:
    CJumpAttackScript();
    ~CJumpAttackScript();
};



