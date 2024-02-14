#pragma once
#include "CComponent.h"

class CStructuredBuffer;

class CEquip :
    public CComponent
{
private:
    CStructuredBuffer* m_pFinalMat;//�� ��ü ���� ��ġ
    vector<Matrix> m_vecFinalBone;
    Matrix m_matFinalBone;

    UINT m_iIndex;
public:
    void init();

    const Matrix& GetFinalMat() { return m_matFinalBone; }
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

