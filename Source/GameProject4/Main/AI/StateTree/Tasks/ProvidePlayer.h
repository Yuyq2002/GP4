#pragma once
#include "StateTreeTaskBase.h"
#include "GameFramework/Actor.h"
#include "Main/AI/DefaultEnemyAI.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "ProvidePlayer.generated.h"


UCLASS()
class UProvidePlayer : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	EStateTreeRunStatus EnterState ( FStateTreeExecutionContext& Context,
const FStateTreeTransitionResult& Transition
) override;

	UPROPERTY(EditAnywhere, Category="Context")
	ADefaultEnemyAI* AI;
	
	UPROPERTY(EditAnywhere, Category="Output")
	APawn* Player;
	
};

