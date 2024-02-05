#pragma once
#include <Engine\CScript.h>

class CMonsterFSM;
class CMonsterState;

class CMonsterScript :
    public CScript
{
private:
    CMonsterFSM* m_pFSM;

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;


    virtual void begin();

    void Initialize(const wstring& _strFbxName);

    void AddAnimFrame(const wstring& _strAnimName, int _iStart, int _iEnd);
    CLONE(CMonsterScript);

public:
    void AddMonsterState(MONSTER_STATE_TYPE _eType, CMonsterState* _pState,
                         const wstring& _strAnimName, int _iStartFrame, int _iEndFrame);
    void AddMonsterState(MONSTER_STATE_TYPE _eType, CMonsterState* _pState, const wstring& _strAnimName);

    void AddMonsterAttack(int _iAttackNum, float _fForce, float _fRotate, float _fTime, int _iStartFrame, int _iEndFrame,
                          Vec3 _vAttackScale, float _fOffsetPos, Vec3 _vAttackRot);

public:
    CMonsterScript();
    ~CMonsterScript();
};


