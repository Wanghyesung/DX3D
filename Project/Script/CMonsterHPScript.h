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
    bool m_bActive;
public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider3D* _Other)override {};
    virtual void OnOverlap(CCollider3D* _Other)override {};
    virtual void EndOverlap(CCollider3D* _Other)override {};

    void SetMonster(CGameObject* _pGameobj) { m_pMonster = _pGameobj; }
    void UpdateGage(float _fMaxGage, float _fCurGage);
    void SetActive(bool _bActive) { m_bActive = _bActive; }

    virtual void begin();

    virtual void Initialize(const wstring& _strTexName, const Vec3& _vScale,
        const wstring& _strName, bool _bBoss, const wstring& _strHangleName = L"");

    CLONE(CMonsterHPScript);
private:
    
public:
    CMonsterHPScript();
    virtual ~CMonsterHPScript();

};

