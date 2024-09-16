#pragma once
#include <Engine\CScript.h>

class CGage;
class CEngineUI;
class CMonsterHPScript : public CScript
{
private:
    CEngineUI* m_pFrame;
    CGage* m_pHP;

    CGameObject* m_pMonster;

    wstring m_strMonsterName;
    wstring m_strHangleFontName;//한글 이름

    bool m_bBoss;
    bool m_bActive;

    Vec3 m_vOffsetPos;

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider3D* _Other)override {};
    virtual void OnOverlap(CCollider3D* _Other)override {};
    virtual void EndOverlap(CCollider3D* _Other)override {};

    void SetMonster(CGameObject* _pGameobj) { m_pMonster = _pGameobj; }
    void UpdateGage(float _fMaxGage, float _fCurGage);
    void SetActive(bool _bActive) { m_bActive = _bActive; }

    virtual void begin();

    virtual void Initialize(const wstring& _strTexName, const wstring& _strName, const wstring& _strHangleName,
        const Vec3& _vScale, bool _bBoss);

    CLONE(CMonsterHPScript);
private:
    void move();

public:
    CMonsterHPScript();
    virtual ~CMonsterHPScript();

};

