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
	printf("onConstraintBreak\n");
}

void CPxEvent::onWake(PxActor** /*actors*/, PxU32 /*count*/)
{
	printf("onWake\n");
}

void CPxEvent::onSleep(PxActor** /*actors*/, PxU32 /*count*/)
{
	printf("onSleep\n");
}

void CPxEvent::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	//		printf("onTrigger: %d trigger pairs\n", count);
	while (count--)
	{
		const PxTriggerPair& current = *pairs++;
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			printf("Shape is entering trigger volume\n");
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			printf("Shape is leaving trigger volume\n");
	}
}

void CPxEvent::onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
{
	printf("onAdvance\n");
}

void CPxEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count)
{
	//		printf("onContact: %d pairs\n", count);

	while (count--)
	{
		const PxContactPair& current = *pairs++;

		// The reported pairs can be trigger pairs or not. We only enabled contact reports for
		// trigger pairs in the filter shader, so we don't need to do further checks here. In a
		// real-world scenario you would probably need a way to tell whether one of the shapes
		// is a trigger or not. You could e.g. reuse the PxFilterData like we did in the filter
		// shader, or maybe use the shape's userData to identify triggers, or maybe put triggers
		// in a hash-set and test the reported shape pointers against it. Many options here.

		if (current.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_CCD))
			printf("Shape is entering trigger volume\n");
		if (current.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			printf("Shape is leaving trigger volume\n");

		//if (isTriggerShape(current.shapes[0]) && isTriggerShape(current.shapes[1]))
		//	printf("Trigger-trigger overlap detected\n");
	}
}