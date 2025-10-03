// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FString StableId;
	
	UPROPERTY()
	FName ActorName;


	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class GAMEPROJECT4_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FActorSaveData> SavedActors;
};