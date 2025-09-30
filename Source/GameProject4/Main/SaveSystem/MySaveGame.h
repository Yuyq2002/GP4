// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FString StableId;
	
	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT(BlueprintType)
struct FPlayerSaveRow
{
	GENERATED_BODY()
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RemainingSkillPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalSkillPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0;

	
};

UCLASS()
class GAMEPROJECT4_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FActorSaveData> SavedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FPlayerSaveRow> Players;
};