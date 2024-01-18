#include "pch.h"
#include "CRunState.h"
#include "CKeyMgr.h"
#include "CRigidbody.h"

void CRunState::final_tick()
{
	CGameObject* pObj = GetOwner();


}

void CRunState::Enter()
{
	Chanage_Anim(GetName());
}

void CRunState::Exit()
{

}

CRunState::CRunState()
{

}

CRunState::~CRunState()
{

}
