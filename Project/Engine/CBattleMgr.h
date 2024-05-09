#pragma once
#include "CSingleton.h"

struct tDamageFont
{
	float fTime;
	float fCurTime;
	int	  iDamage;

	Vec2  vPos;
};

class CBattleMgr :
	public CSingleton< CBattleMgr>
{
	SINGLE(CBattleMgr)

private:
	vector< tDamageFont> m_vecFont;
	
public:
	void ShowFont(int _iDamage, Vec2 _vPos);
	void AddFont(float _fTime, int _iDamage, Vec2 _vPos);

public:
	void Damage(float _fDamage, Vec3 vPos);

public:
	void tick();

private:

};

