#pragma once
#include "CLevel.h"

class CStartLevel : public CLevel
{
private:


public:
	CStartLevel();
	virtual ~CStartLevel();

public:
    virtual void init() override;

    virtual void begin()override;
    virtual void tick()override;
    virtual void finaltick()override;

    virtual void enter()override;
    virtual void exit()override;


    CLONE(CStartLevel);

};

