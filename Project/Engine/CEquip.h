#pragma once
#include "CComponent.h"

class CStructuredBuffer;

class CEquip :
    public CComponent
{
private:
  //  CStructuredBuffer* m_pFinalMat;//내 물체 최종 위치
    vector<Matrix> m_vecFinalBone;

    Matrix m_matFinalBone;
    Matrix m_matStaticWorld;
    Vec3 m_vFixedPos;

    UINT m_iIndex;

    bool m_bActive;

    CGameObject* m_pChar; //내 캐릭터

    static class CEngineUI* m_pEquipIcon; //내 무기 아이콘
    static UINT m_iEquipIdx; //무기 인덱스 번호
    static vector<wstring> m_vecWeaponName;

private:
    bool check_matrix();

    void single_tick();
    void child_tick();
public:
    void SetDead(bool _bDelete = false);//, 삭제할지
    void SetIndex(int _iIndex) { m_iIndex = _iIndex; }
    void SetFixedPos(Vec3 _vPos);
    void SetActive(bool _bActive) { m_bActive = _bActive; }

    bool IsActive() { return m_bActive; }

    Vec3 GetFixedPos() { return m_vFixedPos; }
    int GetIndex() { return m_iIndex; }   
    const Matrix& GetFinalMat() { return m_matFinalBone; }

    void SetChar(CGameObject* _pGameObj)  {m_pChar = _pGameObj;}

    void SetEquipUI(int _iIdx);
public:
    virtual void begin()override;
    virtual void finaltick()override;
    
    CLONE(CEquip);

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);

public:
    CEquip();
    virtual ~CEquip();

};

