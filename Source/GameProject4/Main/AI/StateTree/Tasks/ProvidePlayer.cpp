#include "ProvidePlayer.h"

#include "StateTreeExecutionContext.h"
#include "Kismet/GameplayStatics.h"

EStateTreeRunStatus UProvidePlayer::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	auto controller = UGameplayStatics::GetPlayerController(Context.GetWorld(), 0);
	
	Player = controller->GetPawn().Get();

	if (Player == nullptr)
	{
		FinishTask(false);
		return EStateTreeRunStatus::Failed;
	}
	
	if (IsValid(AI))
		AI->SetAggroTarget(Player);
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI not valid from provide player"));
	}
	
	FinishTask(true);
	return EStateTreeRunStatus::Succeeded;

	
}
