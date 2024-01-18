#pragma once
#include "ComponentUI.h"

class CParticleSystemUI : public ComponentUI
{
public:
	CParticleSystemUI();
	virtual ~CParticleSystemUI();

public:
	virtual int render_update() override;

private:


};

