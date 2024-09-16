#include "pch.h"
#include "CPxCollisionEvent.h"
#include "CPhysxMgr.h"
#include "CCollider3D.h"

CPxCollisionEvent::CPxCollisionEvent()
{

}

CPxCollisionEvent::~CPxCollisionEvent()
{

}

void CPxCollisionEvent::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
	printf("onConstraintBreak\n");
}

void CPxCollisionEvent::onWake(PxActor** /*actors*/, PxU32 /*count*/)
{
	printf("onWake\n");
}

void CPxCollisionEvent::onSleep(PxActor** /*actors*/, PxU32 /*count*/)
{
	printf("onSleep\n");
}

void CPxCollisionEvent::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	
	while (count--)
	{
		const PxTriggerPair& current = *pairs++;
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			printf("Shape is entering trigger volume\n");
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			printf("Shape is leaving trigger volume\n");
	}
}

void CPxCollisionEvent::onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
{
	printf("onAdvance\n");
}

void CPxCollisionEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count)
{
	UINT iLeftID = pairs->shapes[0]->getSimulationFilterData().word0;
	UINT iRightID = pairs->shapes[1]->getSimulationFilterData().word0;

	CGameObject* pLeftObj = CPhysxMgr::GetInst()->FIndEventObj(iLeftID).pEventObj;
	CGameObject* pRightObj = CPhysxMgr::GetInst()->FIndEventObj(iRightID).pEventObj;

	
	if (pLeftObj == nullptr || pRightObj == nullptr)
		return;

	if (pLeftObj->IsDead() || pRightObj->IsDead())
		return;
	else
		CPhysxMgr::GetInst()->CollisionObjectCheck(pLeftObj, pRightObj);

}