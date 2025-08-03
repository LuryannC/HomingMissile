#include "Components/ProjectileComponent.h"
#include "Interfaces/HomingProjectileInterface.h"
#include "HomingProjectileLog.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/HomingProjectileTargetInterface.h"

UProjectileComponent::UProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjectileMovement)
	{
		UE_LOG(HomingProjectileLog, Error, TEXT("UHomingMissileTargetingComponent::BeginPlay - Couldn't find projectile component."));
		return;
	}

	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 10000.f;	

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, [this]()
		{
			if (bShouldUpdateHomingDirection)
			{
				UpdateHomingDirection(GetWorld()->DeltaTimeSeconds);
			}
		}, UpdateRate, true);
	}
}

FVector UProjectileComponent::GetPredictedTargetLocation(const float DeltaTime) const
{
	if (!IsValid(TargetActor))
	{
		//UE_LOG(HomingProjectileLog, Error, TEXT("UProjectileComponent::GetPredictedTargetLocation - Invalid Target."));
		return FVector::ZeroVector;
	}

	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector TargetVelocity = TargetActor->GetVelocity();

	return TargetLocation + TargetVelocity * (DeltaTime * PredictionTime);
}

void UProjectileComponent::UpdateHomingDirection(const float DeltaTime)
{
	// ProjectileMovementComponent check as it is essential.
	check(ProjectileMovement);

	// Attempt to acquire initial target via interface
	if (!IsValid(TargetActor))
	{
		if (IsValid(GetOwner()))
		{
			if (GetOwner()->GetClass()->ImplementsInterface(UHomingProjectileInterface::StaticClass()))
			{
				TargetActor = IHomingProjectileInterface::Execute_GetProjectileTarget(GetOwner());
			}
		}
	}

	if (IsValid(TargetActor))
	{
		// Update Homing Target if changed
		USceneComponent* TargetRoot = TargetActor->GetRootComponent();
		if (TargetRoot && (!ProjectileMovement->HomingTargetComponent.IsValid() || ProjectileMovement->HomingTargetComponent != TargetRoot))
		{
			ProjectileMovement->bIsHomingProjectile = true;
			ProjectileMovement->HomingTargetComponent = TargetRoot;
		}
	}

	// Calculate the direction
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	const FVector PredictedLocation = GetPredictedTargetLocation(DeltaTime);

	FVector DesiredDirection;
	if (PredictedLocation == FVector::ZeroVector)
	{
		const FVector RandomDirection = FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0.f).GetSafeNormal();
		DesiredDirection = RandomDirection;
	}
	else
	{
		DesiredDirection = (PredictedLocation - CurrentLocation).GetSafeNormal();
	}

	const FVector CurrentDirection = ProjectileMovement->Velocity.GetSafeNormal();
	const FVector NewDirection = FMath::VInterpTo(CurrentDirection, DesiredDirection, DeltaTime, TurnSpeed);

	ProjectileMovement->Velocity = NewDirection * ProjectileMovement->InitialSpeed;

	// Attempt to find another target or end the purpose of this projectile.
	if (!IsValid(TargetActor))
	{
		if (bCanSearchForAnotherTarget)
		{
			TargetActor = FindAnotherTarget();
			
			if (!IsValid(TargetActor))
			{
				// This projectile doesn't have a purpose anymore
				if (bShouldCallEndGoalOnFailingToFindTarget)
				{
					OnEndPurpose.Broadcast();
					return;
				}
			}
		}
		else
		{
			OnEndPurpose.Broadcast();
			return;
		}
	}

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), CurrentLocation, PredictedLocation, FColor::Red, false, -1.f, 0, 2.f);
		DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + NewDirection * TurningUnits, FColor::Blue, false, 0.05f, 0, 2.f);
		DrawDebugSphere(GetWorld(), PredictedLocation, PredictionSphereRadius, 12, FColor::Green, false, -1.f);
	}
}

AActor* UProjectileComponent::FindAnotherTarget() const
{
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
		
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bFoundTarget = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		CurrentLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic),
		FCollisionShape::MakeSphere(SearchRadius),
		Params);

	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), CurrentLocation, SearchRadius, 12, FColor::Purple, false, -1.f);
	}
	
	if (!bFoundTarget)
	{
		return nullptr;
	}
	
	AActor* BestTarget = nullptr;
	float ClosestDistSq = MAX_FLT;

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* Candidate = Result.GetActor();
		if (!Candidate || Candidate == TargetActor) continue;

		if (Candidate->GetClass()->ImplementsInterface(UHomingProjectileInterface::StaticClass()))
		{
			bool bCanBeTargeted = false;
			bool bCanTarget = true;
			if (Candidate->GetClass()->ImplementsInterface(UHomingProjectileTargetInterface::StaticClass()))
			{
				bCanBeTargeted = IHomingProjectileTargetInterface::Execute_CanBeTargeted(Candidate);
				bCanTarget = IHomingProjectileTargetInterface::Execute_CanTarget(GetOwner(), Candidate);
			}
			
			const uint8 TargetTeam = IHomingProjectileTargetInterface::Execute_GetProjectileTeam(Candidate);
			const uint8 OwnerTeam = IHomingProjectileTargetInterface::Execute_GetProjectileTeam(GetOwner());
			
			if (bCanBeTargeted && OwnerTeam != TargetTeam && bCanTarget)
			{
				float DistSq = FVector::DistSquared(CurrentLocation, Candidate->GetActorLocation());
				if (DistSq < ClosestDistSq)
				{
					ClosestDistSq = DistSq;
					BestTarget = Candidate;
				}
			}
		}
	}
	
	if (bDrawDebug && BestTarget)
	{
		DrawDebugLine(GetWorld(), CurrentLocation, BestTarget->GetActorLocation(), FColor::Cyan, false, 2.f, 0, 2.f);
	}
	
	return BestTarget;
}