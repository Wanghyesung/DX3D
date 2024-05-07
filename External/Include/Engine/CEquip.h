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

    UINT m_iIndex;

    CGameObject* m_pChar; //내 캐릭터
private:
    bool check_matrix();

public:
    void SetDead(bool _bDelete = false);//, 삭제할지

    void SetIndex(int _iIndex) { m_iIndex = _iIndex; }
    int GetIndex() { return m_iIndex; }

    const Matrix& GetFinalMat() { return m_matFinalBone; }

    void SetChar(CGameObject* _pGameObj) 
    { 
        m_pChar = _pGameObj;
    }
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

