#pragma once

#include <Engine\CLevel.h>

class CStartLevel : public CLevel
{
private :

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

    virtual void enter() override;
    virtual void exit() override;

public:
    CStartLevel();
    virtual ~CStartLevel();
	
};

