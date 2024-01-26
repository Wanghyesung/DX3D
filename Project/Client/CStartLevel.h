#pragma once
#include <Engine\CLevel.h>
class CStartLevel :
    public CLevel
{

private:


public:
    virtual void init();

    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    virtual void enter();
    virtual void exit();

public:
    CStartLevel();
    virtual ~CStartLevel();
};

