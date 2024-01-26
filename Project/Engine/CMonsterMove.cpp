#include "pch.h"
#include "CMonsterMove.h"
#include "CNavMeshMgr.h"
CMonsterMove::CMonsterMove() :
	m_pTarget(nullptr)
{

}

CMonsterMove::~CMonsterMove()
{

}

void CMonsterMove::final_tick()
{
	CNavMeshMgr::GetInst()->GetTargetPath(GetOwner());
}

void CMonsterMove::Enter()
{
	//A* 
	//1초에 한번씩 탐색
	CNavMeshMgr::GetInst()->AddMonster(GetOwner(), 1.f);
}

void CMonsterMove::Exit()
{
	CNavMeshMgr::GetInst()->DeleteMonster(GetOwner());
}


