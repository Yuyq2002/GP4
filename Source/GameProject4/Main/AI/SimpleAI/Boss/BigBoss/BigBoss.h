// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/AI/SimpleAI/Boss/MiniBoss.h"
#include "BigBoss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseChanged, int32, Phase);

UCLASS()
class GAMEPROJECT4_API ABigBoss : public AMiniBoss
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnBossPhaseChanged OnBossPhaseChanged;

public:
	// Sets default values for this character's properties
	ABigBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ChangePhase(const int i) const;


	UPROPERTY(BlueprintAssignable, Category="BossEvents")
	FOnBossPhaseChanged BossPhaseChanged;
};
