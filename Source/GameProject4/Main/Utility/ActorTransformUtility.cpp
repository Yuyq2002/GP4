// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Utility/ActorTransformUtility.h"

void UActorTransformUtility::AlignActor(AActor* Actor, AActor* OtherActor, EAlignment ActorAlignment, EAlignment OtherActorAlignment)
{
	FVector AlignedLocation = GetAlignedLocation(Actor, OtherActor, ActorAlignment, OtherActorAlignment);

	Actor->SetActorLocation(AlignedLocation);
}

FVector UActorTransformUtility::GetAlignedLocation(AActor* Actor, AActor* OtherActor, EAlignment ActorAlignment, EAlignment OtherActorAlignment)
{
	float ActorAlignmentOffset;
	float OtherActorAlignmentOffset;

	FVector Origin;
	FVector Entent;
	Actor->GetActorBounds(false, Origin, Entent);

	switch (ActorAlignment)
	{
	case EAlignment::Origin:
		ActorAlignmentOffset = 0;
		break;
	case EAlignment::Top:
		ActorAlignmentOffset = (Entent.Z + Origin.Z - Actor->GetActorLocation().Z);
		break;
	case EAlignment::Center:
		ActorAlignmentOffset = Origin.Z - Actor->GetActorLocation().Z;
		break;
	case EAlignment::Bottom:
		ActorAlignmentOffset = -(Entent.Z - Origin.Z + Actor->GetActorLocation().Z);
		break;
	default:
		return FVector::ZeroVector;
	}

	OtherActor->GetActorBounds(false, Origin, Entent);
	switch (OtherActorAlignment)
	{
	case EAlignment::Origin:
		OtherActorAlignmentOffset = 0;
		break;
	case EAlignment::Top:
		OtherActorAlignmentOffset = (Entent.Z + Origin.Z - OtherActor->GetActorLocation().Z);
		break;
	case EAlignment::Center:
		OtherActorAlignmentOffset = Origin.Z - OtherActor->GetActorLocation().Z;
		break;
	case EAlignment::Bottom:
		OtherActorAlignmentOffset = -(Entent.Z - Origin.Z + OtherActor->GetActorLocation().Z);
		break;
	default:
		return FVector::ZeroVector;
	}

	return (OtherActor->GetActorLocation() + FVector(0, 0, OtherActorAlignmentOffset - ActorAlignmentOffset));
}

void UActorTransformUtility::AlignActorToLocation(AActor* Actor, FVector AlignToLocation, EAlignment ActorAlignment)
{
	FVector AlignedLocation = GetAlignmentToLocation(Actor, AlignToLocation, ActorAlignment);

	Actor->SetActorLocation(AlignedLocation);
}

FVector UActorTransformUtility::GetAlignmentToLocation(AActor* Actor, FVector AlignToLocation, EAlignment ActorAlignment)
{
	float ActorAlignmentOffset;

	FVector Origin;
	FVector Entent;
	Actor->GetActorBounds(false, Origin, Entent);

	switch (ActorAlignment)
	{
	case EAlignment::Origin:
		ActorAlignmentOffset = 0;
		break;
	case EAlignment::Top:
		ActorAlignmentOffset = (Entent.Z + Origin.Z - Actor->GetActorLocation().Z);
		break;
	case EAlignment::Center:
		ActorAlignmentOffset = Origin.Z - Actor->GetActorLocation().Z;
		break;
	case EAlignment::Bottom:
		ActorAlignmentOffset = -(Entent.Z - Origin.Z + Actor->GetActorLocation().Z);
		break;
	default:
		return FVector::ZeroVector;
	}

	return (AlignToLocation + FVector(0, 0, AlignToLocation.Z - ActorAlignmentOffset));
}
