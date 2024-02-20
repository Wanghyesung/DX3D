#include "pch.h"
#include "CPxEvent.h"

CPxEvent::CPxEvent()
{

}

CPxEvent::~CPxEvent()
{

}

void CPxEvent::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void CPxEvent::onWake(PxActor** actors, PxU32 count)
{
    int a = 10;
}

void CPxEvent::onSleep(PxActor** actors, PxU32 count)
{
    int a = 10;
}

void CPxEvent::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    int a = 10;
}

void CPxEvent::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
    int a = 10;
}

void CPxEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++) 
    {
        const PxContactPair& cp = pairs[i];
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            // 충돌된 두 물체를 가져옵니다.
            PxRigidDynamic* object1 = static_cast<PxRigidDynamic*>(pairHeader.actors[0]);
            PxRigidDynamic* object2 = static_cast<PxRigidDynamic*>(pairHeader.actors[1]);

            // 겹친 양을 계산합니다.
            float overlapAmount = calculateOverlapAmount(cp); // 이 함수는 두 물체 간의 겹친 양을 계산합니다.

            // 겹친 양만큼 물체를 밀어냅니다.
            //PxVec3 separationVector = calculateSeparationVector(cp); // 겹침 해결을 위한 벡터 계산
            float pushForce = 1000.0f; // 밀어내는 힘의 크기

            // 밀어내는 힘을 적용합니다.
            //object1->addForce(separationVector * pushForce);
        }
    }
}

float CPxEvent::calculateOverlapAmount(const PxContactPair& _pxContac)
{
    float minPenetrationDepth = FLT_MAX;

    // 모든 접촉 지점에 대해 반복하며 최소 접촉 깊이를 찾습니다.
    for (PxU32 j = 0; j < _pxContac.contactCount; j++)
    {
        const PxU8* contactPoint = _pxContac.contactPoints;
   
        //float penetrationDepth = contactPoint.separation; // 접촉 깊이
        //
        //if (penetrationDepth < minPenetrationDepth) 
        //{
        //    minPenetrationDepth = penetrationDepth;
        //}
    }

    return minPenetrationDepth; // 최소 접촉 깊이 반환
}
