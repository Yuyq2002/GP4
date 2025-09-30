// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorTransformUtility.generated.h"

UENUM(BlueprintType)
enum class EAlignment : uint8
{
	Origin,
	Top,
	Center,
	Bottom
};

/**
 * 
 */
UCLASS()
class GAMEPROJECT4_API UActorTransformUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void AlignActor(AActor* Actor, AActor* OtherActor, EAlignment ActorAlignment = EAlignment::Origin, EAlignment OtherActorAlignment = EAlignment::Origin);
};
