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
            // �浹�� �� ��ü�� �����ɴϴ�.
            PxRigidDynamic* object1 = static_cast<PxRigidDynamic*>(pairHeader.actors[0]);
            PxRigidDynamic* object2 = static_cast<PxRigidDynamic*>(pairHeader.actors[1]);

            // ��ģ ���� ����մϴ�.
            float overlapAmount = calculateOverlapAmount(cp); // �� �Լ��� �� ��ü ���� ��ģ ���� ����մϴ�.

            // ��ģ �縸ŭ ��ü�� �о���ϴ�.
            //PxVec3 separationVector = calculateSeparationVector(cp); // ��ħ �ذ��� ���� ���� ���
            float pushForce = 1000.0f; // �о�� ���� ũ��

            // �о�� ���� �����մϴ�.
            //object1->addForce(separationVector * pushForce);
        }
    }
}

float CPxEvent::calculateOverlapAmount(const PxContactPair& _pxContac)
{
    float minPenetrationDepth = FLT_MAX;

    // ��� ���� ������ ���� �ݺ��ϸ� �ּ� ���� ���̸� ã���ϴ�.
    for (PxU32 j = 0; j < _pxContac.contactCount; j++)
    {
        const PxU8* contactPoint = _pxContac.contactPoints;
   
        //float penetrationDepth = contactPoint.separation; // ���� ����
        //
        //if (penetrationDepth < minPenetrationDepth) 
        //{
        //    minPenetrationDepth = penetrationDepth;
        //}
    }

    return minPenetrationDepth; // �ּ� ���� ���� ��ȯ
}
