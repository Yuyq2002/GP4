#include "ExperienceGranter.h"
#include "ExperienceContainer.h"

UExperienceGranter::UExperienceGranter()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UExperienceGranter::BeginPlay()
{
	Super::BeginPlay();

	if (bGrantOnDeath && GetOwner())
	{
		GetOwner()->OnDestroyed.AddDynamic(this, &UExperienceGranter::HandleDestroyedActor);
	}
}

void UExperienceGranter::HandleDestroyedActor(AActor* DestroyedActor)
{
	UWorld* World = GetWorld();
	if (!World) return;

	// the getowner hasauthority wrapper is for safeguarding xp gained in online multiplayer, otherwise when an xp is gained it potentially could be allocated twice
	if (GetOwner()->HasAuthority())
	{
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				if (APawn* PlayerPawn = PC->GetPawn())
				{
					GrantExperienceTo(PlayerPawn);
				}
			}
		}
	}
}

void UExperienceGranter::GrantExperienceTo(AActor* TargetActor)
{
	// if (UExperienceContainer* ExpComp = TargetActor->FindComponentByClass<UExperienceContainer>())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Giving %s %d XP"), *TargetActor->GetName(), RandomExperienceValue);
	// 	if (!ExpComp->OnLevelUp.IsAlreadyBound(this, &UExperienceGranter::HandleTargetLeveledUp))
	// 	{
	// 		ExpComp->OnLevelUp.AddDynamic(this, &UExperienceGranter::HandleTargetLeveledUp);
	// 	}
	// 	ExpComp->AddExperience(RandomExperienceValue);
	// }
	if (UExperienceContainer* ExpComp = TargetActor->FindComponentByClass<UExperienceContainer>())
	{
		const int32 RandomXP = FMath::RandRange(MinExperienceValue, MaxExperienceValue);

		UE_LOG(LogTemp, Warning, TEXT("Giving %s %d XP"), *TargetActor->GetName(), RandomXP);

		if (!ExpComp->OnLevelUp.IsAlreadyBound(this, &UExperienceGranter::HandleTargetLeveledUp))
		{
			ExpComp->OnLevelUp.AddDynamic(this, &UExperienceGranter::HandleTargetLeveledUp);
		}

		ExpComp->AddExperience(RandomXP);
	}
}

void UExperienceGranter::HandleTargetLeveledUp(AActor* LeveledActor, int32 NewLevel)
{
	// UE_LOG(LogTemp, Warning, TEXT("Target %s leveled up to level %d"), *GetOwner()->GetName(), NewLevel);
	// OnTargetLevelUp.Broadcast(GetOwner()); 
	// UExperienceContainer* ExpComp = GetOwner()->FindComponentByClass<UExperienceContainer>();
	// if (ExpComp)
	// {
	// 	ExpComp->OnLevelUp.RemoveDynamic(this, &UExperienceGranter::HandleTargetLeveledUp);
	// }
	if (!LeveledActor) return;

	UE_LOG(LogTemp, Warning, TEXT("Target %s leveled up to level %d"), *LeveledActor->GetName(), NewLevel);
	OnTargetLevelUp.Broadcast(LeveledActor);

	if (UExperienceContainer* ExpComp = LeveledActor->FindComponentByClass<UExperienceContainer>())
	{
		ExpComp->OnLevelUp.RemoveDynamic(this, &UExperienceGranter::HandleTargetLeveledUp);
	}
}

void UExperienceGranter::TickComponent(float DeltaTime, ELevelTick TickType,
									   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
