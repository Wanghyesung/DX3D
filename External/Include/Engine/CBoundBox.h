#pragma once
#include "CComponent.h"

class CBoundBox : public CComponent
{
private:
    float m_fBoundBox;

public:
    float GetBoundBox() { return m_fBoundBox; }
    void SetBoundBox(float _fBoundBox) { m_fBoundBox = _fBoundBox; }
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    CLONE(CBoundBox);
public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);

public:
	CBoundBox();
	virtual ~CBoundBox();
};

