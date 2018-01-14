// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	///Get patrol points
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) { return EBTNodeResult::Failed; }
	APawn* Pawn = AIController->GetPawn();
	if (!ensure(Pawn)) { return EBTNodeResult::Failed; }
	UPatrolRoute* PatrolRouteComponent = Pawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRouteComponent)) { return EBTNodeResult::Failed; }
	TArray<AActor*> PatrolPoints = PatrolRouteComponent->GetPatrolPoints();
	if (PatrolPoints.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("%s is missing patrol points"), *Pawn->GetName());
		return EBTNodeResult::Failed;
	}


	///Set next waypoint
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent)) { return EBTNodeResult::Failed; }
	int32 Index = BlackboardComponent->GetValueAsInt(NextIndexKey.SelectedKeyName);
	BlackboardComponent->SetValueAsObject(NextWaypointKey.SelectedKeyName, PatrolPoints[Index]);

	///Cycle index
	int32 NewIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComponent->SetValueAsInt(NextIndexKey.SelectedKeyName, NewIndex);
	return EBTNodeResult::Succeeded;
}