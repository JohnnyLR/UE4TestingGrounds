// Copyright Jose Lloret 2018

#include "ChooseWaypoint.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto OwnerController = OwnerComp.GetAIOwner();

	auto ControlledPawn = OwnerController->GetPawn();

	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (PatrolRoute == NULL) { return EBTNodeResult::Failed; }

	auto IndexKey = BlackboardComp->GetValueAsInt(IndexSelector.SelectedKeyName);

	auto Get = PatrolRoute->GetPatrolPoint(IndexKey);
	BlackboardComp->SetValueAsObject(NextWaypoint.SelectedKeyName, Get);

	auto Length = PatrolRoute->PatrolPointsLength();
	if(Length == 0) { return EBTNodeResult::Failed;	}

	auto CycleIndex = (IndexKey + 1) % Length;
	BlackboardComp->SetValueAsInt(IndexSelector.SelectedKeyName, CycleIndex);

	return EBTNodeResult::Succeeded;
}


